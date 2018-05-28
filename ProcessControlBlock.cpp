//
// Created by hjh on 18-5-27.
//

#include "ProcessControlBlock.h"

#include <cstdlib>


ProcessControlBlock::ProcessControlBlock(int _processID, int _parentID, int _userID, processState _state,
                    processPriority _priority, std::string info, std::string name,
                    ProcessControlBlock _parent) {
    this->processID     = _processID;
    this->parentID      = _parentID;
    this->userID        = _userID;
    this->state         = _state;
    this->priority      = _priority;
    this->info          = _info;
    this->name          = _name;
    this->previous      = nullptr;
    this->next          = nullptr;
    this->parent        = _parent;
    this->child         = nullptr;
    this->nextBrother   = nullptr;
    this->preBrother    = nullptr;
    this->childrenNumber= 0;
}

ProcessControlBlockList::ProcessControlBlockList() {
    this->head = malloc(sizeof(ProcessControlBlock));
    this->tail = malloc(sizeof(ProcessControlBlock));
    this->head->previous = nullptr;
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->tail->next = nullptr;
    length = 0;
    next = nullptr;         // Unused.
}

ProcessControlBlockList::~ProcessControlBlockList() {}

int ProcessControlBlockList::append(ProcessControlBlock* _node) {
//    this->tail->previous
}

int ProcessControlBlockList::insert(ProcessControlBlock* _node) {
    _node->next = head->next;
    head->next = _node;
    length++;
    return 0;   // On success.
}

ProcessControlBlock* ProcessControlBlockList::getHead() {
    if (this->length == 0) {
        return nullptr;
    } else {
        return this->head->next;
    }
}
ProcessControlBlock* ProcessControlBlockList::popHead() {
    if (this->length == 0) {
        return nullptr;
    } else {
        ProcessControlBlock* temp = this->head->next;
        this->head->next = this->head->next->next;
        return this->head->next;

    }
}
ProcessControlBlock* ProcessControlBlockList::getTail() {}
ProcessControlBlock* ProcessControlBlockList::popTail() {}

































