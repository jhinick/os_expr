//
// Created by hjh on 18-5-27.
//

#include "ProcessResourceManager.h"

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
    for (int i = 0; i < MAX_PROCESS; ++i) {
        this->pidPool[i] = 0;
    }
    pidPool[MAX_PROCESS + 1] = -1;
}

ProcessResourceManager::~ProcessResourceManager() = default;


/**
 * init() will initialize data structure and launch init process,
 * which is the root of creation tree with process ID of zero and
 * should always be in ready list.
 * @return return -1 on error.
 */
int ProcessResourceManager::init() {
    ProcessControlBlock* temp = new ProcessControlBlock(this->getProcessID(), 0, SYS_UID, Ready, System, "Init process", "Init");
    this->list[Ready][System].insert(temp); // Assume that the function completed successfully.
    return 0;
}

int ProcessResourceManager::getProcessID(){
    for (int i = 0; i < MAX_PROCESS; ++i) {
        if (this->pidPool[i] == ID_AVAILABLE) {
            this->pidPool[i] = ID_OCCUPIED;
            return i;
        }
    }
    return -1;
}

int ProcessResourceManager::process_create(int _parentID, int _userID, processState _state, processPriority _priority,
                                           std::string info, std::string name) {}
