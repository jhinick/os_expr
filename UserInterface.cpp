
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
#include <fstream>
#include "error.h"
#include "Error.h"
#include "Process.h"

UserInterface::UserInterface(ProcessManager* _processManager, ResourceManager* _resourceManager) {
    this->processManager = _processManager;
    this->resourceManager = _resourceManager;
    this->processManager->setResourceManager(_resourceManager);
    this->resourceManager->setProcessManager(_processManager);
}

UserInterface::~UserInterface() = default;

void UserInterface::run() {
    /* Initialize the system*/
    this->init();
    /* A infinite loop to get user command*/
    while (true) {
        char* cmd = nullptr;
        size_t len = 128;
        std::cout << std::endl << "%" << this->processManager->current->name << "% ";
        getline(&cmd, &len, stdin);
        cmd[strlen(cmd) - 1] = '\0';        // change the terminate char to
        runCommand(cmd);
    }
}



void UserInterface::printUsage() {
    int fd;
    char filename[] = "Usage.txt";
    fd = open(USAGEFILEDIR, O_RDONLY);
    if (fd == -1) {
        std::cout << "Error: Could not find usage text file." << std::endl;
        return;
    }
    char buf[BUF_SIZE];
    ssize_t temp;
    while ((temp = read(fd, buf, BUF_SIZE)) != 0) {
        fprintf(stdout, buf, temp);
    }
    return;
}

int UserInterface::runCommand(char* _cmd) {
    std::string _cmdbk = _cmd;
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
     *  #string                         ; print the string
     *  source sourceFile               ; run command in the file
     *  req resourceType                ; call resource
     *  rel resourceType                ; release resource
     *  lr                              ; print resource info
     *  ls -a                           ; print full resource info
     * */

    /* Print string. */
    if (_cmdbk[0] == '#') {
    	std::cout << _cmdbk << std::endl;
        return 0;
    }
    /* cr -n processName */
    if (tokens.size() == 3 && strcmp(tokens[0], "cr") == 0 && strcmp(tokens[1], "-n") == 0) {
        this->processManager->createProcess(tokens[2]);
        return 0;
    }
    /* cr -n processName -p priority */
    if (tokens.size() == 5 && strcmp(tokens[0], "cr") == 0 && strcmp(tokens[1], "-n") == 0 && strcmp(tokens[3], "-p") == 0) {
        if (strcmp(tokens[4], "System") == 0 ||strcmp(tokens[4], "system") == 0 ) {
            this->processManager->createProcess(tokens[2], System);
            return 0;
        }
        if (strcmp(tokens[4], "User") == 0 || strcmp(tokens[4], "user") == 0) {
            this->processManager->createProcess(tokens[2], User);
            return 0;
        }
        return -1;
    }
    /* kill pid */
    if (tokens.size() == 2 && strcmp(tokens[0], "kill") == 0) {
        state = this->processManager->killProcess(atoi(tokens[1]));
        if (state != 0)
            Error::printInfo(state);
        return 0;
    }
    /* kill -n processName */
    if (tokens.size() == 3 && strcmp(tokens[0], "kill") == 0 && strcmp(tokens[1], "-n") == 0) {
        state = this->processManager->killProcess(tokens[2]);
        if (state != 0)
            Error::printInfo(state);
        return 0;
    }
    /* ls */
    if (tokens.size() == 1 && strcmp(tokens[0], "ls") == 0) {
        this->processManager->printProcessInfo();
        return 0;
    }
    /* ls -l */
    if (tokens.size() == 2 && strcmp(tokens[0], "ls") == 0 && strcmp(tokens[1], "-l") == 0) {
        this->processManager->printProcessFullInfo();
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
        this->processManager->clockInterrupt(rr);
        return 0;
    }
    /* i scheduleAlgorithm */
    if (tokens.size() == 2 && strcmp(tokens[0], "i") == 0
        && (strcmp(tokens[1], "rr") == 0 || strcmp(tokens[1], "fifo") == 0)) {
        if (strcmp(tokens[1], "rr") == 0) {
            this->processManager->clockInterrupt(rr);
            return 0;
        }
        if (strcmp(tokens[1], "fifo") == 0) {
            this->processManager->clockInterrupt(fifo);
            return 0;
        }
    }
    /* lr */
    if (tokens.size() == 1 && strcmp(tokens[0], "lr") == 0) {
        this->resourceManager->printInfo();
        return 0;
    }
    /* lr -a*/
    if (tokens.size() == 2 && strcmp(tokens[0], "lr") == 0 && strcmp(tokens[1], "-a") == 0) {
        this->resourceManager->printFullInfo();
        return 0;
    }
    /* source sourceFileName*/
    if (tokens.size() == 2 && strcmp(tokens[0], "source") == 0) {
    	this->source(tokens[1]);
        return 0;
    }
    if (tokens.size() == 2 && strcmp(tokens[0], "req") == 0) {
        ResourceType type;
        if (strcmp(tokens[1], "0") == 0) {
            type = resource0;
        } else if (strcmp(tokens[1], "1") == 0) {
            type = resource1;
        } else if (strcmp(tokens[1], "2") == 0) {
            type = resource2;
        } else if (strcmp(tokens[1], "3") == 0) {
            type = resource3;
        } else {
            std::cout << "Bad arg." << std::endl;
            return -1;
        }
        this->resourceManager->requireResource(type, this->processManager->current);
        return 0;
    }
    if (tokens.size() == 2 && strcmp(tokens[0], "rel") == 0) {
        ResourceType type;
        if (strcmp(tokens[1], "0") == 0) {
            type = resource0;
        } else if (strcmp(tokens[1], "1") == 0) {
            type = resource1;
        } else if (strcmp(tokens[1], "2") == 0) {
            type = resource2;
        } else if (strcmp(tokens[1], "3") == 0) {
            type = resource3;
        } else {
            std::cout << "Bad arg." << std::endl;
            return -1;
        }
        this->resourceManager->releaseResource(type, this->processManager->current);
        return 0;
    }
    if (tokens.size() == 0)
        return 0;
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
    if (this->processManager->initialize() == 0) {
        std::cout << "[OK]" << std::endl;
    } else {
        std::cout << "[FAILED]" << std::endl;
        perror("Something goes wrong with this system.\n");
        exit(1);
    }
}

int UserInterface::source(char* _path) {
	FILE* file = fopen(_path, "r");
	if (file == nullptr) {
	    std::cout << "Open file error" << std::endl;
        return -1;
	}
    char* cmd = nullptr;
    size_t len = 128;
    while (getline(&cmd, &len, file) != -1) {
        cmd[strlen(cmd) - 1] = '\0';
        runCommand(cmd);
    }
	return 0;
}














