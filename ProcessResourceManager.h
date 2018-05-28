//
// Created by hjh on 18-5-27.
//

#include "Resource.h"
#include "ProcessControlBlock.h"

#ifndef OS_EXPR_PROCESSMANAGER_H
#define OS_EXPR_PROCESSMANAGER_H


class ProcessResourceManager {
public:
    /* All lists needed to maintain processes.*/
    ProcessControlBlockList     list[PROCESS_STATE_NUM][PRIOR_LEVEL_NUM];
    /* All lists needed to maintain all resources.*/
public:
    ProcessResourceManager();
    ~ProcessResourceManager();
    /* Launch init process, which is the root of creation tree, it's a virtual process
     * and can never be blocked.*/
    int init();
    int process_create();
    int process_destroy();

};

#endif //OS_EXPR_PROCESSMANAGER_H
