//
// Created by hjh on 18-5-27.
//



#ifndef OS_EXPR_PROCESSCONTROLBLOCK_H
#define OS_EXPR_PROCESSCONTROLBLOCK_H

class ResourceList;


#include <string>

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
    /* 15 attributes in total.*/
    int                     processID;
    int                     parentID;
    int                     userID;
    processState            state;
    processPriority         priority;
    std::string             info;
    std::string             name;
    /* Form a dual direction linked list.*/
    ProcessControlBlock*    previous;
    ProcessControlBlock*    next;
    /*Implement a process creation tree.*/
    ProcessControlBlock*    parent;
    ProcessControlBlock*    child;      // the first child
    ProcessControlBlock*    nextBrother;
    ProcessControlBlock*    preBrother;
    int                     childrenNumber;

    ResourceList*           resourceList;
public:
    ProcessControlBlock(int _processID, int _parentID, int _userID, processState _state, processPriority _priority, std::string info, std::string name);
    ~ProcessControlBlock();
    void printInfo();
    void printFullInfo();
};

/**
 * Head and tail store no node but indicates the head and tail of the list merely.
 */
class ProcessControlBlockList {
public:
    ProcessControlBlock*        head;
    ProcessControlBlock*        tail;
    int                         length;
public:
    ProcessControlBlockList();
    ~ProcessControlBlockList();
    /*Insert a node to the tail.*/
    int     append(ProcessControlBlock* _node);
    /* Insert a node to the head.*/
    int     insert(ProcessControlBlock* _node);
    /* Return the first node, nullptr on error.*/
    ProcessControlBlock*    getFirst();
    /* Return the first node and remove it from the list, nullptr on error.*/
    ProcessControlBlock*    popFirst();
    /* Return the last nod, nullptr on error. */
    ProcessControlBlock*    getLast();
    /* Return the last node and remove it from the lis, nullptr on error.*/
    ProcessControlBlock*    popLast();
    ProcessControlBlock* getByPid(int _processId);
    ProcessControlBlock* getByName(std::string _processName);
    /* Return -1 when the list is empty.*/
    int printInfo();
    int printFullInfo();
};


#endif //OS_EXPR_PROCESSCONTROLBLOCK_H
