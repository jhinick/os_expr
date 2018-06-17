//
// Created by hjh on 18-5-27.
//

#include "ProcessControlBlock.h"
#include "Resource.h"
#include "Error.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>


ProcessControlBlock::ProcessControlBlock() {

}
ProcessControlBlock::ProcessControlBlock(int _processID, int _parentID, int _userID,
                    processPriority _priority, std::string _name ) {
    this->processID     = _processID;
    this->parentID      = _parentID;
    this->userID        = _userID;
    this->state         = Ready;
    this->priority      = _priority;
    this->name          = _name;
    this->previous      = nullptr;
    this->next          = nullptr;
    this->parent        = nullptr;
    this->child         = nullptr;
    this->nextBrother   = nullptr;
    this->childrenNumber= 0;
    resourceList        = nullptr;
}

ProcessControlBlock::~ProcessControlBlock() {
    // todo: remove the pcb and release its resource
}

int ProcessControlBlock::removeListInfo() {
    this->previous = nullptr;
    this->next = nullptr;
}

void ProcessControlBlock::printInfo() {
    std::cout << "PID: " << this->processID << "\t";
    std::cout << "Name: " << this->name << "\t";
    std::cout << "State: ";
    if (this->state == Current)
        std::cout << "Current";
    if (this->state == Ready)
        std::cout << "Ready";
    if (this->state == Blocked)
        std::cout << "Blocked";
    if (this->state == Suspend)
        std::cout << "Suspend";
    std::cout << std::endl;
}

void ProcessControlBlock::printFullInfo(){
    printf("Process %d info:\n", this->processID);
    std::cout << "Process name:" << this->name << "\t";
    std::cout << "Process ID:" << this->processID << "\t";
    std::cout << "Parent ID:" << this->parentID << "\t";
    std::cout << "User ID" << this->userID << "\t";
    std::cout << "State: ";
    if (this->state == Current)
        std::cout << "Current";
    if (this->state == Ready)
        std::cout << "Ready";
    if (this->state == Blocked)
        std::cout << "Blocked";
    if (this->state == Suspend)
        std::cout << "Suspend";
    std::cout << "\t";
    std::cout << "Priority:";
    if (this->priority == Init)
        std::cout << "Init";
    if (this->priority == User)
        std::cout << "User";
    if (this->priority == System)
        std::cout << "System";
    std::cout << "\t";
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
    this->length = 0;
}

ProcessControlBlockList::~ProcessControlBlockList() = default;

int ProcessControlBlockList::append(ProcessControlBlock* _node) {
    this->tail->previous->next = _node;
    _node->previous = this->tail->previous;
    _node->next = this->tail;
    this->tail->previous = _node;
    this->length++;
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
        ProcessControlBlock* ret = this->head->next;
        this->head->next->next->previous = this->head;
        this->head->next = this->head->next->next;
        this->length--;
        return ret;
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
        ProcessControlBlock* ret = this->tail->previous;
        this->tail->previous->previous->next = this->tail;
        this->tail->previous = this->tail->previous->previous;
        this->length--;
        return ret;
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

ProcessControlBlock* ProcessControlBlockList::searchByPid(int _processId) {
    ProcessControlBlock* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->processID == _processId)
            return ret;
    }
    return nullptr;
}

ProcessControlBlock* ProcessControlBlockList::searchByName(std::string _processName) {
    ProcessControlBlock* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->name == _processName)
            return ret;
    }
    return nullptr;
}

bool ProcessControlBlockList::isEmpty() {
    return this->length == 0;
}

bool ProcessControlBlockList::isNotEmpty() {
    return this->length != 0;
}

int ProcessControlBlockList::remove(ProcessControlBlock *_pcb) {
    ProcessControlBlock* temp = this->head->next;
    while (temp != this->tail) {
        if (temp == _pcb) {
            if (_pcb->processID == 0)
                return Error::CANNOT_REMOVE_INIT;
            temp->previous->next = temp->next;
            temp->next->previous = temp->previous;
            delete _pcb;
            this->length--;
            return 0;
        }
    }
    return Error::PCB_NOT_FOUND_IN_LIST;
}

int ProcessControlBlockList::removeByName(std::string _name) {
    return this->remove(this->searchByName(_name));
}

int ProcessControlBlockList::removeByPid(int _processId) {
    return this->remove(this->searchByPid(_processId));
}






