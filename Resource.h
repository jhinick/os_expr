//
// Created by hjh on 18-5-27.
//

#ifndef RESOURCE_H
#define RESOURCE_H



class ProcessControlBlockList;
class ProcessManager;
class Process;

#define MAX_RESOURCE_NUM 1024
#define RESOURCETYPENUM   4


enum ResourceType {resource0, resource1, resource2, resource3};
enum ResourceStatus {Free, Occupied};


/*==========================================================================*/
/*Every resource has one Resource struct */
class Resource {
public:
    int             resourceID;
    ResourceType    resourceType;
    ResourceStatus  resourceStatus;
    Resource*       previous;
    Resource*       next;
    Process*        processControlBlock;
public:
    Resource();
    Resource(int _resourceID, ResourceType _resourceType);
    bool isFree();
    void printInfo();
    void printFullInfo();
};
/*==========================================================================*/
/* Every kind of resource linked together and form a ResourceList*/
class ResourceList {
public:
    Resource*       head;
    Resource*       tail;
    int             length;
    int             freeNum;
    int             waitNum;
    ProcessControlBlockList*    sysRequestQueue;
    ProcessControlBlockList*    userRequestQueue;
public:
    ResourceList();
    int addResource(int _resourceID, ResourceType _resourceType);
    int append(Resource* _resource);
    /* Remoce but not delete*/
    int remove(Resource* _resource);
    Resource* remove(ResourceType _resourceType);
    Resource* getResource(Resource* _resource);
    Resource* getResource(Process* _process);
    Resource* getResource(ResourceType _resourceType);
    Resource* firstFree();
    void printInfo();
    void printFullInfo();
};

/*==========================================================================*/
/* This class interact with exterior processes.*/
class ResourceManager {
public:
    ResourceList        resourceList[RESOURCETYPENUM];
    /* 0 for free and 1 for used*/
    int                 resourceIdPool[MAX_RESOURCE_NUM];
    ProcessManager*     processManager;
public:
    ResourceManager();
    int setProcessManager(ProcessManager* _processManager);
    /* add resource to ResourceManager*/
    int init(int _0_num, int _1_num, int _2_num, int _3_num);
    int addResource(ResourceType _resourceType);
    /* return -1 on error*/
    int getResourceID();
    Resource* requireResource(ResourceType _resourceType, Process* _processControlBlockList);
    int releaseResource(ResourceType _resourceType, Process* _processControlBlock);
    int assignResource(ResourceType _resourceType);
    /* Assign a free resource to a waiting process*/
    int assignResource(ResourceType _resourceType, Process* _process);
};


#endif
