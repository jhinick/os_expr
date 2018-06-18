
//
// Created by jhin on 18-5-31.
//


#define USAGEFILEDIR "~/CLionProjects/os_expr/Usage"

#include "UserInterface.h"

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <fcntl.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include "error.h"
#include "Error.h"

UserInterface::UserInterface(ProcessResourceManager* _processResourceManager) {
    this->processResourceManager = _processResourceManager;
}

UserInterface::~UserInterface() = default;

void UserInterface::run() {
    /* initialize the system*/
    this->init();
    /* A infinite loop to get user command*/
    while (true) {
        char* cmd = nullptr;
        size_t len = 128;
        std::cout << std::endl << "%" << this->processResourceManager->current->name << "% ";
        getline(&cmd, &len, stdin);
        cmd[strlen(cmd) - 1] = '\0';        // change the terminate char to
        runCommand(cmd);
    }
}



void UserInterface::printUsage() {
//    int fd;
//    char filename[] = "/Usage";
//    fd = open(USAGEFILEDIR, O_RDONLY);
//    if (fd == -1) {
//        std::cout << "Error: Could not find usage text file." << std::endl;
//        return;
//    }
//    char buf[BUF_SIZE];
//    ssize_t temp;
//    while ((temp = read(fd, buf, BUF_SIZE)) != 0) {
//        fprintf(stdout, buf, temp);
//    }
    std::cout << "Usage: ...." << std::endl;
    return;
}

int UserInterface::getAndAnalyseCommand(){
    char* cmd = nullptr;
    size_t len = 128;
    getline(&cmd, &len, stdin);
    char* tok;
    std::vector<char*> tokens;
    tokens.clear();
    tok = strtok(cmd, " ");
    while (tok != nullptr) {
        tokens.push_back(tok);
        tok = strtok(nullptr, " ");
    }
//    for (int i = 0; i < tokens.size(); i++) {
//        printf("%s\t", tokens[i]);
//    }
    /*  All available command
     *  cr -n processName               ; create a process with default priority
     *  cr -n processName -p priority   ; create a process with specified priority
     *  kill processNum                 ; kill a process by pid
     *  kill -n processName             ; kill a process by name
     *  ls                              ; list all processes' name, pid and state
     *  ls -l                           ; list all processes' full information
     *  todo:
     *  source fileName                 ; run source file script
     *  todo:
     *  source                          ; run default file in work dir
     *  todo:
     *  echo string                     ; print string
     *  req sourceType                  ; request for system resource
     *  rel -n sourceType               ; release resource
     *  rel -a                          ; release all resource
     *  lr
     *  lr -l                           ; list resource full status
     * */
    // cr -n processName
    char cr[] = "cr";
    char _n[] = "-n";
    char _p[] = "-p";
    char kill[] = "kill";
    char ls[] = "ls";
    char _l[] = "-l";

    if (tokens.size() == 3) {
        if (strcmp(tokens[0], cr) == 0 && strcmp(tokens[1], _n) == 0) {
            // cr -n processName
        }
    }
    if (tokens.size() == 5 && strcmp(tokens[0], cr) == 0 && strcmp(tokens[1], _n) == 0 && strcmp(tokens[3], _p) == 0) {
            // cr -n name -p prior
    }
    if (tokens.size() == 2 && strcmp(tokens[0], kill) == 0) {
            // kill pid
    }
    if (tokens.size() == 3 && strcmp(tokens[0], kill) == 0 && strcmp(tokens[1], _n) == 0) {
        // kill -n name
    }
    if (tokens.size() == 1 && strcmp(tokens[0], ls) == 0) {
        // ls
    }
    if (tokens.size() == 2 && strcmp(tokens[0], ls) == 0 && strcmp(tokens[1], _l) == 0) {
        // ls -l
    }
}

int UserInterface::runCommand(char* _cmd) {
    int state;
    char* tok = nullptr;
    std::vector<char*> tokens;
    tokens.clear();
    tok = strtok(_cmd, " ");
    while (tok != nullptr) {
        tokens.push_back(tok);
        tok = strtok(nullptr, " ");
    }
    /*  All valid commands
     *  cr -n processName               ; create a process with default (User) priority
     *  cr -n processName -p priority   ; create a process with specified priority, priority
     *                                    can be "System" "system" or "User" "user"
     *  kill processID                  ; kill a process by pid
     *  kill -n processName             ; kill a process by name
     *  ccp processID                   ; change current process
     *  ccp -n processName              ; change current process by name
     *  ls                              ; list all processes' name, pid and state
     *  ls -l                           ; list all processes' full information
     *  exit or Exit                    ; terminate the system
     *  h or help                       ; print help info
     *  i                               ; interrupt for scheduler
     *  i scheduleAlgorithm             ; schedule in fifo mode
     * */

    /* cr -n processName */
    if (tokens.size() == 3 && strcmp(tokens[0], "cr") == 0 && strcmp(tokens[1], "-n") == 0) {
        this->processResourceManager->createProcess(tokens[2]);
        return 0;
    }
    /* cr -n processName -p priority */
    if (tokens.size() == 5 && strcmp(tokens[0], "cr") == 0 && strcmp(tokens[1], "-n") == 0 && strcmp(tokens[3], "-p") == 0) {
        if (strcmp(tokens[4], "System") == 0 ||strcmp(tokens[4], "system") == 0 ) {
            this->processResourceManager->createProcess(tokens[2], System);
            return 0;
        }
        if (strcmp(tokens[4], "User") == 0 || strcmp(tokens[4], "user") == 0) {
            this->processResourceManager->createProcess(tokens[2], User);
            return 0;
        }
        return -1;
    }
    /* kill pid */
    if (tokens.size() == 2 && strcmp(tokens[0], "kill") == 0) {
        state = this->processResourceManager->killProcess(atoi(tokens[1]));
        if (state != 0)
            Error::printInfo(state);
        return 0;
    }
    /* kill -n processName */
    if (tokens.size() == 3 && strcmp(tokens[0], "kill") == 0 && strcmp(tokens[1], "-n") == 0) {
        state = this->processResourceManager->killProcess(tokens[2]);
        if (state != 0)
            Error::printInfo(state);
        return 0;
    }
    /* ls */
    if (tokens.size() == 1 && strcmp(tokens[0], "ls") == 0) {
        this->processResourceManager->printProcessInfo();
        return 0;
    }
    /* ls -l */
    if (tokens.size() == 2 && strcmp(tokens[0], "ls") == 0 && strcmp(tokens[1], "-l") == 0) {
        this->processResourceManager->printProcessFullInfo();
        return 0;
    }
    /* exit */
    if (strcmp(_cmd, "exit") == 0 || strcmp(_cmd, "Exit") == 0) {
        this->exitSys();
    }
    if (strcmp(_cmd, "h") == 0 || strcmp(_cmd, "help") == 0) {
        // todo: printHelp
    }
    /* i */
    if (strcmp(_cmd, "i") == 0) {
        this->processResourceManager->clockInterrupt(rr);
        return 0;
    }
    /* i scheduleAlgorithm */
    if (tokens.size() == 2 && strcmp(tokens[0], "i") == 0
        && (strcmp(tokens[1], "rr") == 0 || strcmp(tokens[1], "fifo") == 0)) {
        if (strcmp(tokens[1], "rr") == 0) {
            this->processResourceManager->clockInterrupt(rr);
            return 0;
        }
        if (strcmp(tokens[1], "fifo") == 0) {
            this->processResourceManager->clockInterrupt(fifo);
            return 0;
        }
    }
    std::cout << "Can't resolve command, input \"h\" for help." << std::endl;
}

void UserInterface::exitSys() {
    std::cout << "Sure to exit? Press 1 to exit, otherwise continue.";
    char opt;
    opt = getchar();
    if (opt == '1')
        exit(0);
}

void UserInterface::init() {
    std::cout << "Initializing system...";
    if (this->processResourceManager->initialize() == 0) {
        std::cout << "[OK]" << std::endl;
    } else {
        std::cout << "[FAILED]" << std::endl;
        perror("Something goes wrong with this system.\n");
        exit(1);
    }
}














