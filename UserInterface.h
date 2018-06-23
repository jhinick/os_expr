//
// Created by jhin on 18-5-31.
//


#ifndef MAIN_USERINTERFACE_H
#define MAIN_USERINTERFACE_H

#include <vector>
#include "Process.h"

#ifndef BUF_SIZE
#define BUF_SIZE 128
#endif

/*  UserInterface doesn't do anything but interacting with users
 *  All tasks are finished by invoking functions of ProcessManager
 *  But it will check the return value of these functions and print some info.
 * */
class UserInterface {
public:
    ProcessManager* processManager;
    ResourceManager* resourceManager;
public:
    UserInterface(ProcessManager* _processManager, ResourceManager* _resourceManager);
    ~UserInterface();
    void run();
    void init();
    void printUsage();              // print usage of the system
    int runCommand(char* _cmd);     // run the command of _cmd
    void exitSys();                 // exit the system
};



#endif //MAIN_USERINTERFACE_H

