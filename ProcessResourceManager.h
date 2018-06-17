//
// Created by hjh on 18-5-27.
//

/**
 * The class of ProcessResourceManager exist only one in a system,
 * it manage all processes and resources.
 * Users call it's function shell, the userID is transmitted from shell.
 */

#ifndef OS_EXPR_PROCESSMANAGER_H
#define OS_EXPR_PROCESSMANAGER_H
#include "ProcessControlBlock.h"
#include "Resource.h"


#ifndef MAX_PROCESS
#define MAX_PROCESS 128
#endif

enum getNextAlgorithm {FIFO};




class ProcessResourceManager {
public:
    ProcessControlBlockList     list[PROCESS_STATE_NUM][PRIOR_LEVEL_NUM];
    ProcessControlBlock*        current;
#ifndef ID_AVAILABLE
#define ID_AVAILABLE 0
#endif
#ifndef ID_OCCUPIED
#define ID_OCCUPIED 1
#endif
    int pidPool[MAX_PROCESS];
public:
    ProcessResourceManager();
    ~ProcessResourceManager();
    /* Launch init process, which is the root of creation tree, it's a virtual process
     * and can never be blocked.
     * init process' PID and parentID are both 0.*/
    int init();
    int getProcessID(); /* Return the smallest available id in the id pool, return -1 on error.*/
    int process_create(int _parentID, int _userID, processState _state, processPriority _priority,
                        std::string info, std::string name);
    void listProcessByPid(int _processId);
    int setCurrent(ProcessControlBlock* _processControlBlock);
    ProcessControlBlock* getNext(getNextAlgorithm _getNextAlgrithom);
    int printProcessInfo();
    int printProcessFullInfo();
    int createProcess(std::string _name = "UnknownName", processPriority _processPriority = User);
    int killProcess(std::string _name);
    int killProcess(int _processID);

};

#endif //OS_EXPR_PROCESSMANAGER_H
