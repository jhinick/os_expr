//
// Created by hjh on 18-6-17.
//

#include <iostream>
#include "Error.h"

int Error::UNKNOWN_ERROR                   = -100;
int Error::PROCESS_NOT_FOUND               = -101;
int Error::NO_PROCESS_ID_AVAILABLE         = -103;
int Error::CANNOT_REMOVE_INIT              = -104;
int Error::PCB_NOT_FOUND_IN_LIST           = -105;

void Error::printInfo(int _errorNum) {
    switch (_errorNum) {
        case -100: std::cout << "Error 100:Unknown error." << std::endl;
        case -101: std::cout << "Error 101:Process not found." << std::endl;
        case -103: std::cout << "Error 103:Processes number reaches the top, try to remove some and try again." << std::endl;
        case -104: std::cout << "Error 104:Try to remove init process" << std::endl;
        default: std::cout << "Unsorted error." << std::endl;
    }

}