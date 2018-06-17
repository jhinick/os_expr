//
// Created by hjh on 18-6-17.
//

#include "Error.h"

int Error::UNKNOWN_ERROR                   = -100;
int Error::PROCESS_NOT_FOUND               = -101;
int Error::NO_PROCESS_ID_AVAILABLE         = -103;
int Error::CANNOT_REMOVE_INIT              = -104;
int Error::PCB_NOT_FOUND_IN_LIST           = -105;

void Error::printInfo(int _errorNum) {

}