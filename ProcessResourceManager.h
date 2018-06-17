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
    ProcessControlBlock*        init;
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
    int initialize();
    int getProcessID(); /* Return the smallest available id in the id pool, return -1 on error.*/
    int setCurrent(ProcessControlBlock* _processControlBlock);
    ProcessControlBlock* getNext(getNextAlgorithm _getNextAlgrithom);
    int printProcessInfo();
    int printProcessFullInfo();
    int createProcess(std::string _name = "UnknownName", processPriority _processPriority = User);
    int killProcess(std::string _name);
    int killProcess(int _processID);
    /**
     * The way this function works:
     * 1.   When clock interrupt happens or the process get blocked, invoke this function
     * 2.   This function put the PCB to the right PCBList firstly
     * 3.   Then call getNext() to get the next process to run and set it as current process
     * 4.   When there is no process to run, run init process.
     * @return  0 on success. nonzero on error.
     */
    int schdule(schdule_algorithm, processState _processState);
    /* We handle clock interrupt with round robin algorithm */
    int clockInterrupt(schdule_algorithm _schdule_algorithm = rr);
};

#endif //OS_EXPR_PROCESSMANAGER_H
