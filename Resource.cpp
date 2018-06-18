//
// Created by hjh on 18-5-27.
//

#include "Resource.h"

/*================================================================================*/

Resource::Resource() {
    this->resourceID        = NULL;
    this->resourceType      = NULL;
    this->resourceStatus    = NULL;
    this->previous          = nullptr;
    this->next              = nullptr;
    this->processControlBlock = nullptr;
}

Resource::Resource(int _resourceID, ResourceType _resourceType){
    this->resourceID = _resourceID;
    this->resourceType = _resourceType;
    this->resourceStatus = free;
    this->previous = nullptr;
    this->next = nullptr;
    this->processControlBlock = nullptr;
}

bool Resource::isFree() {
    return (this->resourceStatus == free);
}

int Resource::requireResource(ProcessControlBlock* _processControlBlock) {
    if (this->resourceStatus == occupied)
        return -1;
    this->resourceStatus = occupied;
    this->processControlBlock = _processControlBlock;
    return 0;
}

int Resource::releaseResource() {
    this->resourceStatus = free;
    this->processControlBlock = nullptr;
    return 0;
}

/*================================================================================*/

int ResourceList::addResource(int _resourceID, ResourceType _resourceType) {
    // todo: unblock some process
    Resource* temp = new Resource(_resourceID, _resourceType);
    this->append(temp);
    return 0;
}

int ResourceList::append(Resource* _resource) {
    _resource->previous = this->tail->previous;
    _resource->next = this->tail;
    this->tail->previous->next = _resource;
    this->tail->previous = _resource;
    this->length++;
    this->freeNum++;
    return 0;
}

Resource* ResourceList::requireResource(ProcessControlBlock* _processControlBlock) {
    if (this->freeNum <= 0) {
        if (_processControlBlock->priority == User)
            this->userRequestQueue->append(_processControlBlock);
        if (_processControlBlock->priority == System)
            this->sysRequestQueue->append(_processControlBlock);
        this->freeNum--;
        // todo: block the process
        return nullptr;
    }
    Resource* ret = this->firstFree();
    ret->requireResource(_processControlBlock);
    this->freeNum--;
    return ret;
}

Resource* ResourceList::firstFree(){
    if (this->freeNum == 0)
        return nullptr;
    Resource* ret = this->head->next;
    while (ret != this->tail) {
        if (ret->isFree())
            return ret;
    }
}



void ResourceList::printInfo() {
    // todo;
}

void ResourceList::printFullInfo() {
    // todo;
}

ResourceList::ResourceList() {
    this->head = new Resource();
    this->tail = new Resource();
    this->head->previous = nullptr;
    this->head->next = this->tail;
    this->tail->previous = this->head;
    this->tail->next = nullptr;
    this->length = 0;
    this->freeNum = 0;
    this->sysRequestQueue = new ProcessControlBlockList();
    this->userRequestQueue = new ProcessControlBlockList();
}

int ResourceList::releaseResource(ProcessControlBlock *_processControlBlock) {
    Resource* temp = this->head->next;
    while (true) {
        if (temp->processControlBlock == _processControlBlock)
            break;
        temp = temp->next;
    }
    temp->releaseResource();
    this->freeNum++;
    // todo: unblock some processes
    return 0;
}

int ResourceList::checkFree() {
    if (this->freeNum <= 0)
        return 0;
    if (this->sysRequestQueue->length == 0 && this->userRequestQueue->length == 0)
        return 0;
    //todo:unblock some processes
    return 0;
}

/*================================================================================*/
ResourceManger::ResourceManger() = default;

int ResourceManger::init(int _0_num, int _1_num, int _2_num, int _3_num) {
    for (int i = 0; i < MAX_RESOURCE_NUM; ++i) {
        this->resourceIdPool[i] = 0;
    }
    for (int i = 0; i < _0_num; ++i) {
        this->addResource(resource0);
    }
    for (int j = 0; j < _1_num; ++j) {
        this->addResource(resource1);
    }
    for (int k = 0; k < _2_num; ++k) {
        this->addResource(resource2);
    }
    for (int l = 0; l < _3_num; ++l) {
        this->addResource(resource3);
    }
}

int ResourceManger::addResource(ResourceType _resourceType) {
    this->resourceList[_resourceType].addResource(this->getResourceID(), _resourceType);
}

int ResourceManger::getResourceID() {
    for (int i = 0; i < MAX_RESOURCE_NUM; ++i) {
        if (this->resourceIdPool[i] == 0) {
            this->resourceIdPool[i] = 1;
            return i;
        }
    }
    return -1;
}

Resource* ResourceManger::requireResource(ResourceType _resourceType, ProcessControlBlock* _processControlBlock){
    return this->resourceList[_resourceType].requireResource(_processControlBlock);
}

int ResourceManger::releaseResource(ResourceType _resourceType, ProcessControlBlock *_processControlBlock) {
    this->resourceList[_resourceType].requireResource(_processControlBlock);
    return 0;
}



