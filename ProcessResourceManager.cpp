//
// Created by hjh on 18-5-27.
//

#include "ProcessResourceManager.h"
#include "error.h"
#include "Error.h"

/* System ID, reserved and can't be used by user. */
#ifndef SYS_UID
#define SYS_UID 0
#endif


ProcessResourceManager::ProcessResourceManager() {
    /* Initialize data structure.*/
    for (int loopi = 0; loopi < PROCESS_STATE_NUM; loopi++) {
        for (int loopj = 0; loopj < PRIOR_LEVEL_NUM; loopj++) {
            this->list[PROCESS_STATE_NUM][PRIOR_LEVEL_NUM] = ProcessControlBlockList();
        }
    }
    /* Initialize pid pool.*/
    for (int i = 0; i < MAX_PROCESS; ++i) {
        this->pidPool[i] = 0;
    }
}

ProcessResourceManager::~ProcessResourceManager() = default;


/**
 * init() will initialize data structure and launch init process,
 * which is the root of creation tree with process ID of zero and
 * should always be in ready list.
 * @return return -1 on error.
 */
int ProcessResourceManager::initialize(){
    this->init= new ProcessControlBlock(this->getProcessID(), 0, SYS_UID, System, "Init");
    /* set init as current process.*/
    setCurrent(init);
    return 0;
}

int ProcessResourceManager::getProcessID(){
    for (int i = 0; i < MAX_PROCESS; ++i) {
        if (this->pidPool[i] == ID_AVAILABLE) {
            this->pidPool[i] = ID_OCCUPIED;
            return i;
        }
    }
    return Error::NO_PROCESS_ID_AVAILABLE;
}

int ProcessResourceManager::printProcessInfo() {
    this->current->printInfo();
    for (int i = 0; i < PROCESS_STATE_NUM; i++) {
        for (int j = 0; j < PRIOR_LEVEL_NUM; j++) {
            this->list[i][j].printInfo();
        }
    }
}

int ProcessResourceManager::printProcessFullInfo() {
    this->current->printFullInfo();
    for (int i = 0; i < PROCESS_STATE_NUM; i++) {
        for (int j = 0; j < PRIOR_LEVEL_NUM; j++) {
            this->list[i][j].printFullInfo();
        }
    }
}

int ProcessResourceManager::setCurrent(ProcessControlBlock* _processControlBlock) {
    // todo: handle the last current pcb
    _processControlBlock->state = Current;
    this->current = _processControlBlock;
}

ProcessControlBlock* ProcessResourceManager::getNext(getNextAlgorithm _getNextAlgrithom) {
    switch (_getNextAlgrithom) {
        case FIFO:
            if (this->list[Ready][System].isNotEmpty()) {
                return this->list[Ready][System].popFirst();
            }
            if (this->list[Ready][User].isNotEmpty()) {
                return this->list[Ready][User].popFirst();
            }
            return nullptr;
            break;
        default:
            return nullptr;
    }
}

int ProcessResourceManager::createProcess(std::string _name, processPriority _processPriority) {
    int pid = this->getProcessID();
    if (pid < 0) {
        Error::printInfo(pid);
    }
    ProcessControlBlock* temp = new ProcessControlBlock(pid, this->current->processID, 0, _processPriority, _name);
    this->list[Ready][_processPriority].append(temp);
    /* complete its parent, child and brother info.*/
    /* its parent should be current process */
    temp->parent = this->current;
    temp->parent->addChild(temp);
    return 0;
}

int ProcessResourceManager::killProcess(std::string _name) {
    int ret = Error::UNKNOWN_ERROR;
    if (this->current->name == _name) {
        // todo: schedule and
        return ret;
    } else {
        for (int state = 0; state < PROCESS_STATE_NUM; state++) {
            for (int priority = 0; priority < PRIOR_LEVEL_NUM; priority++) {
                ret = this->list[state][priority].removeByName(_name);
                if (ret == 0 || ret == Error::CANNOT_REMOVE_INIT)
                    return ret;
            }
        }
        return Error::PROCESS_NOT_FOUND;
    }
}

int ProcessResourceManager::killProcess(int _processID) {
    int ret = Error::UNKNOWN_ERROR;
    if (this->current->processID == _processID) {
        // todo: when you want to kill current process
        return ret;
    } else {
        for (int state = 0; state < PROCESS_STATE_NUM; state++) {
            for (int priority = 0; priority < PRIOR_LEVEL_NUM; priority++) {
                ret = this->list[state][priority].removeByPid(_processID);
                if (ret == 0)
                    return ret;
                if (ret == Error::CANNOT_REMOVE_INIT)
                    return ret;
            }
        }
        return Error::PROCESS_NOT_FOUND;
    }
}

int ProcessResourceManager::schdule(schedule_algorithm _schdule_algorithm, processState _processState) {
    /* Put the PCB to the right list, if the process is init, don't do that.*/
    // todo :append or not?
    this->current->state = _processState;
    if (this->current != this->init) {
        this->list[_processState][this->current->priority].append(this->current);
    }
    /* Use get_next() to get the next process to run*/
    ProcessControlBlock* next = nullptr;
    if ((next = this->list[Ready][System].getNext(_schdule_algorithm)) != nullptr) {
        this->setCurrent(next);
        return 0;
    }
    if ((next = this->list[Ready][User].getNext(_schdule_algorithm)) != nullptr) {
        this->setCurrent(next);
        return 0;
    }
    next = this->init;
    this->setCurrent(next);
}

int ProcessResourceManager::clockInterrupt(schedule_algorithm _schedule_algorithm) {
    this->schdule(_schedule_algorithm, Ready);
}
