//
// Created by hjh on 18-5-27.
//

#ifndef RESOURCE_H
#define RESOURCE_H

#include <vector>
#include "ProcessControlBlock.h"

class ProcessControlBlockList;

#define MAX_RESOURCE_NUM 1024
#define RESOUCETYPENUM  4

enum ResourceType {resource0, resource1, resource2, resource3};
enum ResourceStatus {free, occupied};


/*==========================================================================*/
/*Every resource has one Resource struct */
class Resource {
public:
    int             resourceID;
    ResourceType    resourceType;
    ResourceStatus  resourceStatus;
    Resource*       previous;
    Resource*       next;
    ProcessControlBlock*    processControlBlock;
public:
    Resource();
    int Resource(int _resourceID, ResourceType _resourceType);
    int requireResource(ProcessControlBlock* _processControlBlock);
    int releaseResource();
    bool isFree();
};
/*==========================================================================*/
/* Every kind of resource linked together and form a ResourceList*/
class ResourceList {
public:
    Resource*       head;
    Resource*       tail;
    int             length;
    int             freeNum;
    ProcessControlBlockList*    sysRequestQueue;
    ProcessControlBlockList*    userRequestQueue;
public:
    ResourceList();
    int addResource(int _resourceID, ResourceType _resourceType);
    int append(Resource* _resource);
    Resource* requireResource(ProcessControlBlock* _processControlBlock);
    int releaseResource(ProcessControlBlock* _processControlBlock);
    Resource* firstFree();
    int checkFree();
    void printInfo();
    void printFullInfo();
};

/*==========================================================================*/
/* This class interact with exterior processes.*/
class ResourceManger {
public:
    ResourceList        resourceList[RESOUCETYPENUM];
    /* 0 for free and 1 for used*/
    int                 resourceIdPool[MAX_RESOURCE_NUM];
public:
    ResourceManger();
    /* add resource to ResourceManager*/
    int init(int _0_num, int _1_num, int _2_num, int _3_num);
    int addResource(ResourceType _resourceType);
    /* return -1 on error*/
    int getResourceID();
    Resource* requireResource(ResourceType _resourceType, ProcessControlBlock* _processControlBlockList);
    int releaseResource(ResourceType _resourceType, ProcessControlBlock* _processControlBlock);
};



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
}











#endif
