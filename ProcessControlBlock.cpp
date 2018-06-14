//
// Created by hjh on 18-5-27.
//

#include "ProcessControlBlock.h"
#include "Resource.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>


ProcessControlBlock::ProcessControlBlock(int _processID, int _parentID, int _userID, processState _state,
                    processPriority _priority, std::string _info, std::string _name ) {
    this->processID     = _processID;
    this->parentID      = _parentID;
    this->userID        = _userID;
    this->state         = _state;
    this->priority      = _priority;
    this->info          = _info;
    this->name          = _name;
    this->previous      = nullptr;
    this->next          = nullptr;
    this->parent        = nullptr;
    this->child         = nullptr;
    this->nextBrother   = nullptr;
    this->preBrother    = nullptr;
    this->childrenNumber= 0;
}

ProcessControlBlock::~ProcessControlBlock() = default;

void ProcessControlBlock::printInfo() {
    std::cout << "ID: " << this->processID << "\t";
    std::cout << "State: " << this->state << "\t";
    std::cout << "Name: " << this->name << std::endl;
}

void ProcessControlBlock::printFullInfo(){
    printf("Process %d info:\n", this->processID);
    std::cout << "Process ID:" << this->processID << std::endl;
    std::cout << "Parent ID:" << this->parentID << std::endl;
    std::cout << "User ID" << this->userID << std::endl;
    std::cout << "State:" << this->state << std::endl;
    std::cout << "Priority:" << this->priority << std::endl;
    std::cout << "Process name:" << this->name << std::endl;
    std::cout << "Process info:" << this->info << std::endl;
    std::cout << "Children number:" << this->childrenNumber << std::endl;
}

/*====================ProcessControlBlockList=============================*/

ProcessControlBlockList::ProcessControlBlockList() {
    this->head = (ProcessControlBlock*)malloc(sizeof(ProcessControlBlock));
    this->tail = (ProcessControlBlock*)malloc(sizeof(ProcessControlBlock));
    this->head->previous = nullptr;
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->tail->next = nullptr;
    length = 0;
}

ProcessControlBlockList::~ProcessControlBlockList() = default;

int ProcessControlBlockList::append(ProcessControlBlock* _node) {
    this->tail->previous->next = _node;
    _node->previous = this->tail->previous;
    _node->next = this->tail;
    this->tail->previous = _node;
    return 0;
}

int ProcessControlBlockList::insert(ProcessControlBlock* _node) {
    _node->previous = this->head;
    _node->next = this->head->next;
    this->head->next->previous = _node;
    this->head->next = _node;
    this->length++;
    return 0;
}

ProcessControlBlock* ProcessControlBlockList::getFirst(){
    if (this->length == 0) {
        return nullptr;
    } else {
        return this->head->next;
    }
}
ProcessControlBlock* ProcessControlBlockList::popFirst(){
    if (this->length == 0) {
        return nullptr;
    } else {
        ProcessControlBlock* temp = this->head->next;
        this->head->next = this->head->next->next;
        this->head->next->previous = this->head;
        this->length--;
        return temp;
    }
}
ProcessControlBlock* ProcessControlBlockList::getLast(){
    if (this->length == 0) {
        return nullptr;
    } else {
        return this->tail->previous;
    }
}

ProcessControlBlock* ProcessControlBlockList::popLast(){
    if (this->length == 0) {
        return nullptr;
    } else {
        ProcessControlBlock* temp = this->tail->previous;
        this->tail->previous->previous->next = this->tail;
        this->tail->previous = this->tail->previous->previous;
        this->length--;
        return temp;
    }
}

int ProcessControlBlockList::printInfo() {
    if (this->length == 0) {
        return -1;
    }
    ProcessControlBlock* temp = this->head->next;
    for (int i = 0; i < this->length; i++) {
        temp->printInfo();
        temp = temp->next;
    }
    return 0;
}

int ProcessControlBlockList::printFullInfo(){
    if (this->length == 0) {
        return -1;
    }
    ProcessControlBlock* temp = this->head->next;
    for (int i = 0; i < this->length; i++) {
        temp->printFullInfo();
        temp = temp->next;
    }
    return 0;
}

ProcessControlBlock* ProcessControlBlockList::getByPid(int _processId) {
    ProcessControlBlock* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->processID == _processId)
            return ret;
    }
    return nullptr;
}

ProcessControlBlock* ProcessControlBlockList::getByName(std::string _processName) {
    ProcessControlBlock* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->name == _processName)
            return ret;
    }
    return nullptr;
}

































