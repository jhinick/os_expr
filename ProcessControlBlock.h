//
// Created by hjh on 18-5-27.
//

#include "Resouce.h"


#include <string>

#ifndef OS_EXPR_PROCESSCONTROLBLOCK_H
#define OS_EXPR_PROCESSCONTROLBLOCK_H

#ifndef PROCESS_STATE_NUM
#define PROCESS_STATE_NUM 4  /*Indicates how many states can a process be in in the system.*/
#endif
enum    processState {Running, Blocked, Ready, Suspend};
#ifndef PRIOR_LEVEL_NUM
#define PRIOR_LEVEL_NUM   3 /*Indicates how many level exists in the system.*/
#endif
enum    processPriority {Init, User, System};

class ProcessControlBlock {
public:
    int                     processID;
    int                     parentID;
    int                     userID;
    processState            state;
    processPriority         priority;
    std::string             info;
    std::string             name;
    ProcessControlBlock*    next;
    /*Data structures to implement pcb creation tree.*/
    /*===================================================*/
    ProcessControlBlock*    parent;
    /*This is the very fist child in the children chain, and it may vary over time.*/
    ProcessControlBlock*    child;
    /**
     * With child and brother pointers, we can access all children processes.
     * With next and previous brother pointers, we can traverse all peer process with linear
     * time complexity.
     */
    ProcessControlBlock*    nextBrother;
    ProcessControlBlock*    preBrother;
    /*To count how many children in total*/
    int                     childrenNumber;

    /*Date structures to implement resources allocation and control.*/
    /*===================================================*/
    /*A linked list storing all resources occupied by this process.*/
    ResourceList            resourceList;
public:
    ProcessControlBlock();
    ~ProcessControlBlock();

};

class ProcessControlBlockList {
public:
    ProcessControlBlock*        head;
    ProcessControlBlock*        tail;
    ProcessControlBlockList*    next;
    int                         length;
};

#endif //OS_EXPR_PROCESSCONTROLBLOCK_H
