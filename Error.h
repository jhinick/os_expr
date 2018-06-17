//
// Created by hjh on 18-6-17.
//

#ifndef MAIN_ERROR_H
#define MAIN_ERROR_H


class Error {
public:
    static int PROCESS_NOT_FOUND;
    static int UNKNOWN_ERROR;
    static int NO_PROCESS_ID_AVAILABLE;
    static int CANNOT_REMOVE_INIT;
    static int PCB_NOT_FOUND_IN_LIST;
public:
    static void printInfo(int _errorNum);
};




#endif //MAIN_ERROR_H
