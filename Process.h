//
// Created by hjh on 18-5-27.
//


#ifndef OS_EXPR_PROCESSCONTROLBLOCK_H
#define OS_EXPR_PROCESSCONTROLBLOCK_H

class ResourceList;
class Resource;
class ResourceManager;

#include <string>
#include "Resource.h"

#define PROCESS_STATE_NUM 4  /*Indicates how many states can a process be in in the system.*/
enum    processState {Current, Ready, Blocked, Suspend};
#define PRIOR_LEVEL_NUM   2 /*Indicates how many level exists in the system.*/
enum    processPriority {User, System};

enum    schedule_algorithm {fifo, rr};

/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/
class Process {
public:
    int                     processID;
    int                     parentID;
    int                     userID;
    processState            state;
    processPriority         priority;
    std::string             name;
    Process*                previous;
    Process*                next;
    Process*                parent;
    Process*                child;
    Process*                nextBrother;
    int                     childNumber;
    ResourceList*           occupiedResourceList;
    ResourceList*           waitResourceList;



public:
    Process(int _processID, int _parentID, int _userID, processPriority _priority, std::string name);
    Process(int _processID);
    ~Process();
    int setParent(Process* _process);
    int addChild(Process* _childProcess);
    /* assume that the child is this process' child process. */
    /* this node is no longer its parent's child, but its' parent keep the same*/
    int removeChild(Process* _childPcb);
    int addOccupiedResource(Resource* resource);
    int removeOccupiedResource(Resource* _resource);
    int addWaitResource(Resource* _resource);
    int removeWaitResource(Resource* _resource);
    void printInfo();
    void printFullInfo();
};

/*===============================================================================================*/
/*===============================================================================================*/
/*===============================================================================================*/
class ProcessControlBlockList {
public:
    Process*        head;
    Process*        tail;
    int                         length;
public:
    ProcessControlBlockList();
    ~ProcessControlBlockList();
    int     append(Process* _node);
    /* Insert a node to the head.*/
    int     insert(Process* _node);
    /* Return the first node, nullptr on error.*/
    Process*    getFirst();
    /* Return the first node and remove it from the list, nullptr on error.*/
    Process*    popFirst();
    /* Return the last node, nullptr on error. */
    Process*    getLast();
    /* Return the last node and remove it from the lis, nullptr on error.*/
    Process*    popLast();
    /* Return the pointer to the pcb, return nullptr on error or not found.*/
    Process* searchByPid(int _processId);
    Process* searchByName(std::string _processName);
    bool isEmpty();
    bool isNotEmpty();
    /* Return -1 when the list is empty.*/
    int printInfo();
    int printFullInfo();
    int remove(Process* _pcb);
    int removeByPid(int _processId);
    int removeByName(std::string _name);
    bool existProcess(Process* _process);
    Process* getProcess(Process* _process);
    Process* takeOutProcess(Process* _process);
    /* return value may be nullptr */
    Process* getNext(schedule_algorithm _schedule_algorithm);
};



#define MAX_PROCESS 128
enum getNextAlgorithm {FIFO};


/*==================================================================================*/
/*==================================================================================*/
/*==================================================================================*/


class ProcessManager {
public:
	/*Current, Ready, Blocked, Suspended*/      /* User, System*/
    ProcessControlBlockList     list[PROCESS_STATE_NUM][PRIOR_LEVEL_NUM];
    Process*        current;
    Process*        init;
    ResourceManager* resourceManager;
#define ID_AVAILABLE 0
#define ID_OCCUPIED 1
    int pidPool[MAX_PROCESS];
public:
    ProcessManager();
    ~ProcessManager();
    /* Launch init process, which is the root of creation tree, it's a virtual process
     * and can never be blocked.
     * init process' PID and parentID are both 0.*/
    int initialize();
    void setResourceManager(ResourceManager* _resourceManager);
    int getProcessID(); /* Return the smallest available id in the id pool, return -1 on error.*/
    int releaseProcessID(int _pid);
    int setCurrent(Process* _processControlBlock);
    Process* getProcess(Process* _process);
    Process* getProcess(int _processID);
    Process* getProcess(std::string _name);
    Process* getNext(getNextAlgorithm _getNextAlgrithom);
    ProcessControlBlockList* existProcess(Process* _process);
    int printProcessInfo();
    int printProcessFullInfo();
    int createProcess(std::string _name = "UnknownName", processPriority _processPriority = User);
    int killProcess(std::string _name);
    int killProcess(int _processID);
    int killProcess(Process* _process);
    int blockProcess(Process* _process);
    int unBlockProcess(Process* _process);
    int deWaitResource(Process* _process, ResourceType _resourceType);
    int releaseResource(Process* _process, ResourceType _resourceType);

    /**
     * The way this function works:
     * 1.   When clock interrupt happens or the process get blocked, invoke this function
     * 2.   This function put the PCB to the right PCBList firstly
     * 3.   Then call getNext() to get the next process to run and set it as current process
     * 4.   When there is no process to run, run init process.
     * @return  0 on success. nonzero on error.
     */
    int schedule(schedule_algorithm, processState _processState);
    /* We handle clock interrupt with round robin algorithm */
    int clockInterrupt(schedule_algorithm _schdule_algorithm = rr);
};


#endif //OS_EXPR_PROCESSCONTROLBLOCK_H
