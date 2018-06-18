//
// Created by hjh on 18-5-27.
//



#ifndef OS_EXPR_PROCESSCONTROLBLOCK_H
#define OS_EXPR_PROCESSCONTROLBLOCK_H

class ResourceList;


#include <string>

#define PROCESS_STATE_NUM 4  /*Indicates how many states can a process be in in the system.*/
enum    processState {Current, Blocked, Ready, Suspend};

#define PRIOR_LEVEL_NUM   2 /*Indicates how many level exists in the system.*/
enum    processPriority {User, System};

enum    schedule_algorithm {fifo, rr};

class ProcessControlBlock {
public:
    int                     processID;
    int                     parentID;
    int                     userID;
    processState            state;
    processPriority         priority;
    std::string             name;
    ProcessControlBlock*    previous;
    ProcessControlBlock*    next;
    /*Implement a process creation tree.*/
    ProcessControlBlock*    parent;
    ProcessControlBlock*    child;      // the first child
    ProcessControlBlock*    nextBrother;
    int                     childNumber;

    ResourceList*           resourceList;
public:
    ProcessControlBlock();
    ProcessControlBlock(int _processID, int _parentID, int _userID, processPriority _priority, std::string name);
    ~ProcessControlBlock();
    int removeListInfo();
    int addChild(ProcessControlBlock* _childPcb);
    /* assume that the child is this process' child process. */
    int removeChild(ProcessControlBlock* _childPcb);
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
    /* Return the last node, nullptr on error. */
    ProcessControlBlock*    getLast();
    /* Return the last node and remove it from the lis, nullptr on error.*/
    ProcessControlBlock*    popLast();
    /* Return the pointer to the pcb, return nullptr on error or not found.*/
    ProcessControlBlock* searchByPid(int _processId);
    ProcessControlBlock* searchByName(std::string _processName);
    bool isEmpty();
    bool isNotEmpty();
    /* Return -1 when the list is empty.*/
    int printInfo();
    int printFullInfo();
    int remove(ProcessControlBlock* _pcb);
    int removeByPid(int _processId);
    int removeByName(std::string _name);
    /* return value may be nullptr */
    ProcessControlBlock* getNext(schedule_algorithm _schedule_algorithm);
};


#endif //OS_EXPR_PROCESSCONTROLBLOCK_H
