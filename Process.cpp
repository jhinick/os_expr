//
// Created by hjh on 18-5-27.
//

#include "Process.h"
#include "Resource.h"
#include "Error.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>

/*================================================================================*/
/*================================================================================*/
/*================================================================================*/
Process::Process(int _processID, int _parentID, int _userID,
                    processPriority _priority, std::string _name ) {
    this->processID     = _processID;
    this->parentID      = _parentID;
    this->userID        = _userID;
    this->state         = Ready;
    this->priority      = _priority;
    this->name          = _name;
    this->previous      = nullptr;
    this->next          = nullptr;
    this->parent        = nullptr;
    this->child         = nullptr;
    this->nextBrother   = nullptr;
    this->childNumber= 0;
    occupiedResourceList       = new ResourceList();
    waitResourceList           = new ResourceList();
}

Process::~Process() {
    // todo: remove the pcb and release its resource
}

int Process::addChild(Process* _childProcess) {
    if (this->child == nullptr) {
        this->child = _childProcess;
    } else {
        Process* tmp = this->child;
        while (tmp->nextBrother != nullptr)
            tmp = tmp->nextBrother;
        tmp->nextBrother = _childProcess;
    }
    this->childNumber++;
    return 0;
}

int Process::removeChild(Process *_childProcess) {
	if (this->child == _childProcess) {
		this->child = nullptr;
	} else {
		Process* tmp = this->child;
		while (tmp->nextBrother != _childProcess)
			tmp = tmp->nextBrother;
		tmp->nextBrother = _childProcess->nextBrother;
	}
	this->childNumber--;
	return 0;
}




void Process::printInfo() {
    std::cout << "PID: " << this->processID << "\t";
    std::cout << "Name: " << this->name << "\t";
    std::cout << "State: ";
    if (this->state == Current)
        std::cout << "Current";
    if (this->state == Ready)
        std::cout << "Ready";
    if (this->state == Blocked)
        std::cout << "Blocked";
    if (this->state == Suspend)
        std::cout << "Suspend";
    std::cout << std::endl;
}

void Process::printFullInfo(){
    std::cout << "ID:" << this->processID << " \t";
    std::cout << "Name:" << this->name << " \t";
    std::cout << "Parent:" << this->parentID << " \t";
    std::cout << "NextBro:";
    if (this->nextBrother == nullptr) {
        std::cout << "NA" << " \t";
    } else {
        std::cout << this->nextBrother->processID;
    }
    std::cout << "UserID:" << this->userID << " \t";
    std::cout << "State:";
    if (this->state == Current)
        std::cout << "Current";
    if (this->state == Ready)
        std::cout << "Ready";
    if (this->state == Blocked)
        std::cout << "Blocked";
    if (this->state == Suspend)
        std::cout << "Suspend";
    std::cout << " \t";
    std::cout << "Priority:";
    if (this->priority == User)
        std::cout << "User \t";
    if (this->priority == System)
        std::cout << "System \t";

    std::cout << "ChildNum:" << this->childNumber << " \t";
    if (this->childNumber > 0) {
        std::cout << "ChildList:";
        Process* temp = this->child;
        for (; temp != nullptr; temp = child->nextBrother) {
            std::cout << temp->processID << " \t";
        }
    }
    std::cout << std::endl;
}

int Process::addOccupiedResource(Resource *_resource) {
	this->occupiedResourceList->append(_resource);
	return 0;
}

int Process::removeOccupiedResource(Resource *_resource) {
	this->occupiedResourceList->remove(_resource);
	return 0;
}

int Process::addWaitResource(Resource *_resource) {
	this->waitResourceList->append(_resource);
	return 0;
}

int Process::removeWaitResource(Resource *_resource) {
	this->waitResourceList->remove(_resource);
	return 0;
}

int Process::setParent(Process *_process) {
	this->parent = _process;
	return 0;
}

/*====================ProcessControlBlockList=============================*/

ProcessControlBlockList::ProcessControlBlockList() {
    this->head = (Process*)malloc(sizeof(Process));
    this->tail = (Process*)malloc(sizeof(Process));
    this->head->previous = nullptr;
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->tail->next = nullptr;
    this->length = 0;
}

ProcessControlBlockList::~ProcessControlBlockList() = default;

int ProcessControlBlockList::append(Process* _node) {
    this->tail->previous->next = _node;
    _node->previous = this->tail->previous;
    _node->next = this->tail;
    this->tail->previous = _node;
    this->length++;
    return 0;
}

int ProcessControlBlockList::insert(Process* _node) {
    _node->previous = this->head;
    _node->next = this->head->next;
    this->head->next->previous = _node;
    this->head->next = _node;
    this->length++;
    return 0;
}

Process* ProcessControlBlockList::getFirst(){
    if (this->length == 0) {
        return nullptr;
    } else {
        return this->head->next;
    }
}

Process* ProcessControlBlockList::popFirst(){
    if (this->length == 0) {
        return nullptr;
    } else {
        Process* ret = this->head->next;
        this->head->next->next->previous = this->head;
        this->head->next = this->head->next->next;
        this->length--;
        return ret;
    }
}
Process* ProcessControlBlockList::getLast(){
    if (this->length == 0) {
        return nullptr;
    } else {
        return this->tail->previous;
    }
}

Process* ProcessControlBlockList::popLast(){
    if (this->length == 0) {
        return nullptr;
    } else {
        Process* ret = this->tail->previous;
        this->tail->previous->previous->next = this->tail;
        this->tail->previous = this->tail->previous->previous;
        this->length--;
        return ret;
    }
}

int ProcessControlBlockList::printInfo() {
    if (this->length == 0) {
        return -1;
    }
    Process* temp = this->head->next;
    for (int i = 0; i < this->length; i++) {
        temp->printInfo();
        temp = temp->next;
    }
    return 0;
}

int ProcessControlBlockList::printFullInfo(){
    if (this->length == 0) {
        return -1;
    }
    Process* temp = this->head->next;
    for (int i = 0; i < this->length; i++) {
        temp->printFullInfo();
        temp = temp->next;
    }
    return 0;
}

Process* ProcessControlBlockList::searchByPid(int _processId) {
    Process* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->processID == _processId)
            return ret;
    }
    return nullptr;
}

Process* ProcessControlBlockList::searchByName(std::string _processName) {
    Process* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->name == _processName)
            return ret;
    }
    return nullptr;
}

bool ProcessControlBlockList::isEmpty() {
    return this->length == 0;
}

bool ProcessControlBlockList::isNotEmpty() {
    return this->length != 0;
}

int ProcessControlBlockList::remove(Process *_pcb) {
    Process* temp = this->head->next;
    while (temp != this->tail) {
        if (temp == _pcb) {
            if (_pcb->processID == 0)
                return Error::CANNOT_REMOVE_INIT;
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
            delete _pcb;
            this->length--;
            return 0;
        }
    }
    return Error::PCB_NOT_FOUND_IN_LIST;
}

int ProcessControlBlockList::removeByName(std::string _name) {
    return this->remove(this->searchByName(_name));
}

int ProcessControlBlockList::removeByPid(int _processId) {
    return this->remove(this->searchByPid(_processId));
}

Process* ProcessControlBlockList::getNext(schedule_algorithm _schedule_algorithm) {
    Process* ret = nullptr;
    switch (_schedule_algorithm) {
        case fifo:
            ret = this->popFirst();
            break;
        case rr:
            ret = this->popFirst();
        default:
            break;
    }
    return ret;
}

bool ProcessControlBlockList::existProcess(Process *_process) {
    return this->getProcess(_process) != nullptr;
}

Process *ProcessControlBlockList::getProcess(Process *_process) {
	Process* ret = this->head->next;
	while (ret != this->tail) {
	    if (ret == _process)
            return ret;
        ret = ret->next;
	}
    return nullptr;
}

Process *ProcessControlBlockList::takeOutProcess(Process *_process) {
	Process* ret = this->head;
	while (ret != this->tail) {
	    if (ret == _process) {
	        ret->previous->next = ret->next;
	        ret->next->previous = ret->previous;
	    }
        ret = ret->next;
	}
    return nullptr;
}


#ifndef SYS_UID
#define SYS_UID 0
#endif


ProcessManager::ProcessManager() {
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

ProcessManager::~ProcessManager() = default;


/**
 * init() will initialize data structure and launch init process,
 * which is the root of creation tree with process ID of zero and
 * should always be in ready list.
 * @return return -1 on error.
 */
int ProcessManager::initialize(){
    this->init= new Process(this->getProcessID(), 0, SYS_UID, System, "Init");
    /* set init as current process.*/
    setCurrent(init);
    this->resourceManager->init(3, 4, 5, 6);
    return 0;
}

int ProcessManager::getProcessID(){
    for (int i = 0; i < MAX_PROCESS; ++i) {
        if (this->pidPool[i] == ID_AVAILABLE) {
            this->pidPool[i] = ID_OCCUPIED;
            return i;
        }
    }
    return Error::NO_PROCESS_ID_AVAILABLE;
}

int ProcessManager::printProcessInfo() {
    this->current->printInfo();
    for (int i = 0; i < PROCESS_STATE_NUM; i++) {
        for (int j = 0; j < PRIOR_LEVEL_NUM; j++) {
            this->list[i][j].printInfo();
        }
    }
}

int ProcessManager::printProcessFullInfo() {
    this->current->printFullInfo();
    for (int i = 0; i < PROCESS_STATE_NUM; i++) {
        for (int j = 0; j < PRIOR_LEVEL_NUM; j++) {
            this->list[i][j].printFullInfo();
        }
    }
}

int ProcessManager::setCurrent(Process* _processControlBlock) {
    // todo: handle the last current pcb
    _processControlBlock->state = Current;
    this->current = _processControlBlock;
}

Process* ProcessManager::getNext(getNextAlgorithm _getNextAlgrithom) {
    switch (_getNextAlgrithom) {
        case FIFO:
            if (this->list[Ready][System].isNotEmpty()) {
                return this->list[Ready][System].popFirst();
            }
            if (this->list[Ready][User].isNotEmpty()) {
                return this->list[Ready][User].popFirst();
            }
            return nullptr;
        default:
            return nullptr;
    }
}

int ProcessManager::createProcess(std::string _name, processPriority _processPriority) {
    int pid = this->getProcessID();
    if (pid < 0) {
        Error::printInfo(pid);
    }
    Process* temp = new Process(pid, this->current->processID, 0, _processPriority, _name);
    this->list[Ready][_processPriority].append(temp);
    /* complete its parent, child and brother info.*/
    /* its parent should be current process */
	temp->setParent(this->current);
    temp->parent->addChild(temp);
    return 0;
}

int ProcessManager::killProcess(std::string _name) {
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

int ProcessManager::killProcess(int _processID) {
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

int ProcessManager::schdule(schedule_algorithm _schdule_algorithm, processState _processState) {
    /* Put the PCB to the right list, if the process is init, don't do that.*/
    // todo :append or not?
    this->current->state = _processState;
    if (this->current != this->init) {
        this->list[_processState][this->current->priority].append(this->current);
    }
    /* Use get_next() to get the next process to run*/
    Process* next = nullptr;
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

int ProcessManager::clockInterrupt(schedule_algorithm _schedule_algorithm) {
    this->schdule(_schedule_algorithm, Ready);
}

int ProcessManager::blockProcess(Process *_process) {
	if (_process->state == Current) {
		schdule(fifo, Blocked);
	}
	if (_process->state == Blocked)
		return -1;
	if (_process->state == Suspend)
		return -1;
	if (_process->state == Ready) {
		this->existProcess(_process)->remove(_process);
		_process->state = Suspend;
		this->list[Suspend][_process->state].append(_process);
	}
}

int ProcessManager::unBlockProcess(Process *_process) {
	this->list[Blocked][_process->priority].remove(_process);
	_process->state = Ready;
	this->list[Ready][_process->priority].append(_process);
	return 0;
}

Process *ProcessManager::getProcess(Process *_process) {

	return nullptr;
}

ProcessControlBlockList *ProcessManager::existProcess(Process *_process) {
	for (int i = 0; i < PROCESS_STATE_NUM; i++) {
		for (int j = 0; j < PRIOR_LEVEL_NUM; j++) {
			if (this->list[i][j].existProcess(_process))
				return &this->list[i][j];
		}
	}
	return nullptr;
}

void ProcessManager::setResourceManager(ResourceManager *_resourceManager) {
	this->resourceManager = _resourceManager;
}





