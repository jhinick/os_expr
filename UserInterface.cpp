
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
#include <stdlib.h>
#include <sstream>
#include <cstring>

UserInterface::UserInterface(ProcessResourceManager* _processResourceManager) {
    this->processResourceManager = _processResourceManager;
}

UserInterface::~UserInterface() = default;

void UserInterface::run() {
    /* initialize the system*/
    this->init();
    /* A infinite loop to get user command*/
    int i = 0;
    while (true) {
        char* cmd = nullptr;
        size_t len = 128;
        std::cout << "%%";
        getline(&cmd, &len, stdin);
        cmd[strlen(cmd) - 1] = '\0';        // change the terminate char to
        int runState = runCommand(cmd);
        switch (runState) {
            // todo: check the return value of runCommand()
            case (1):
                break;
            default:
                break;
        }
        if (i) break;
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
     *
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
    char* tok = nullptr;
    std::vector<char*> tokens;
    tokens.clear();
    tok = strtok(_cmd, " ");
    while (tok != nullptr) {
        tokens.push_back(tok);
        tok = strtok(nullptr, " ");
    }
    char cr[] = "cr";
    char _n[] = "-n";
    char _p[] = "-p";
    char kill[] = "kill";
    char ls[] = "ls";
    char _l[] = "-l";
    /*  All available command
     *  cr -n processName               ; create a process with default priority
     *  cr -n processName -p priority   ; create a process with specified priority
     *  kill processNum                 ; kill a process by pid
     *  kill -n processName             ; kill a process by name
     *  ls                              ; list all processes' name, pid and state
     *  ls -l                           ; list all processes' full information
     *  exit or Exit                    ; terminate the system
     * */
    if (tokens.size() == 3 && strcmp(tokens[0], cr) == 0 && strcmp(tokens[1], _n) == 0) {
        //todo
        // cr -n processName
    }
    if (tokens.size() == 5 && strcmp(tokens[0], cr) == 0 && strcmp(tokens[1], _n) == 0 && strcmp(tokens[3], _p) == 0) {
        //todo
        // cr -n name -p prior
    }
    if (tokens.size() == 2 && strcmp(tokens[0], kill) == 0) {
        //todo
        // kill pid
    }
    if (tokens.size() == 3 && strcmp(tokens[0], kill) == 0 && strcmp(tokens[1], _n) == 0) {
        //todo
        // kill -n name
    }
    if (tokens.size() == 1 && strcmp(tokens[0], ls) == 0) {
        this->processResourceManager->printProcessInfo();
        // ls
    }
    if (tokens.size() == 2 && strcmp(tokens[0], ls) == 0 && strcmp(tokens[1], _l) == 0) {
        this->processResourceManager->printProcessFullInfo();
        // ls -l
    }
    if (strcmp(_cmd, "exit") == 0 || strcmp(_cmd, "Exit") == 0) {
        //todo
        // exit the system
        this->exitSys();
    }

}

void UserInterface::exitSys() {
    std::cout << "Sure to exit? Press 1 to exit, otherwise continue.";
    int opt;
    std::cin >> opt;
    if (opt == 1)
        exit(0);
}

void UserInterface::init() {
    std::cout << "Initializing system...";
    if (this->processResourceManager->init() == 0) {
        std::cout << "[OK]" << std::endl;
    } else {
        std::cout << "[FAILED]" << std::endl;
        perror("Something goes wrong with this system.\n");
        exit(1);
    }
}














