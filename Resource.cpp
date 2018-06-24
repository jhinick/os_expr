//

// Created by hjh on 18-5-27.
//

#include <iostream>
#include "Resource.h"
#include "Error.h"
#include "Process.h"

/*==============         Resource         ====================================*/

Resource::Resource() {
    this->resourceID        = -1;
    this->resourceType      = resource0;
    this->resourceStatus    = Free;
    this->previous          = nullptr;
    this->next              = nullptr;
    this->processControlBlock = nullptr;
}

Resource::Resource(int _resourceID, ResourceType _resourceType){
    this->resourceID = _resourceID;
    this->resourceType = _resourceType;
    this->resourceStatus = Free;
    this->previous = nullptr;
    this->next = nullptr;
    this->processControlBlock = nullptr;
}

bool Resource::isFree() {
    return (this->resourceStatus == Free);
}

void Resource::printInfo() {

}

void Resource::printFullInfo() {
	std::cout << "ResourceID: " << this->resourceID << "\t";
	std::cout << "Status: ";
	if (this->resourceStatus == 0)
		std::cout << "Free\t";
	if (this->resourceStatus == 1)
		std::cout << "Occupied\t";
	std::cout << "Process: ";
	if (this->processControlBlock != nullptr) {
		std::cout << this->processControlBlock->processID;
	} else {
		std::cout << "NA";
	}
}

/*============         ResourceList         ===================================*/

int ResourceList::addResource(int _resourceID, ResourceType _resourceType) {
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
	std::cout << "ResourceType: " << this->head->next->resourceType << "\t";
	std::cout << "TotalNum: " << this->length << "\t"
				<< "FreeNum: " << this->freeNum << "\t";
	if (this->freeNum < 0)
		std::cout << "WaitNum: " << this->waitNum;
	std::cout << std::endl;
	Resource* temp = this->head;
	for (int i = 0; i < this->length; i++) {
		temp = temp->next;
		temp->printFullInfo();
		std::cout << std::endl;
	}
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
    this->waitNum = 0;
    this->sysRequestQueue = new ProcessControlBlockList();
    this->userRequestQueue = new ProcessControlBlockList();
}


Resource* ResourceList::remove(Resource *_resource) {
	Resource* tmp = this->findResource(_resource->resourceID);
	if (tmp == this->tail)
		return nullptr;
	tmp->previous->next = tmp->next;
	tmp->next->previous = tmp->previous;
	this->length--;
	return tmp;
}

Resource* ResourceList::findResource(int _resourceID) {
	Resource* temp = this->head;
	while (temp->resourceID != _resourceID && temp != this->tail)
		temp = temp->next;
	if (temp == this->tail)
		return nullptr;
	return temp;
}

Resource* ResourceList::getResource(Resource* _resource) {
	Resource* tmp = this->head;
	while (tmp != _resource && tmp != tail) {
		tmp = tmp->next;
	}
	if (tmp == this->tail)
		return nullptr;
	return tmp;
}



Resource *ResourceList::getResource(Process *_process) {
	Resource* ret = this->head;
	while (ret != this->tail && ret->processControlBlock != _process)
		ret = ret->next;
	if (ret != this->tail)
		return ret;
	return nullptr;
}

Resource* ResourceList::remove(ResourceType _resourceType) {
	Resource* tmp = this->getResource(_resourceType);
	if (tmp == nullptr)
		return nullptr;
	tmp->previous->next = tmp->next;
	tmp->next->previous = tmp->previous;
	return tmp;
}

Resource *ResourceList::getResource(ResourceType _resourceType) {
	this->tail->resourceType = _resourceType;
	Resource* ret = this->head;
	while (ret->resourceType != _resourceType)
		ret = ret->next;
	if (ret == this->tail)
		return nullptr;
	return ret;
}

Resource *ResourceList::getResource(int _processID) {
	Resource* temp = this->head;
	for (int i = 0; i < this->length; i++) {
		temp = temp->next;
		if (temp->processControlBlock->processID == _processID)
			return temp;
	}
	return nullptr;
}


/*==============         ResourceManager         ==============================*/
ResourceManager::ResourceManager() = default;

int ResourceManager::init(int _0_num, int _1_num, int _2_num, int _3_num) {
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

int ResourceManager::addResource(ResourceType _resourceType) {
    this->resourceList[_resourceType].addResource(this->getResourceID(), _resourceType);
    if (resourceList[_resourceType].freeNum > 0 && resourceList[_resourceType].waitNum > 0)
    	this->assignResource(_resourceType);
	return 0;
}

int ResourceManager::getResourceID() {
    for (int i = 0; i < MAX_RESOURCE_NUM; ++i) {
        if (this->resourceIdPool[i] == 0) {
            this->resourceIdPool[i] = 1;
            return i;
        }
    }
    return Error::NO_RESOURCE_ID_AVAILABLE;
}

Resource* ResourceManager::requireResource(ResourceType _resourceType, Process* _process){
	if (this->resourceList[_resourceType].freeNum <= 0) {
		if (_process->priority == User)
			this->resourceList[_resourceType].userRequestQueue->append(new Process(_process->processID));
		if (_process->priority == System)
			this->resourceList[_resourceType].sysRequestQueue->append(new Process(_process->processID));
		this->resourceList[_resourceType].freeNum--;
		this->resourceList[_resourceType].waitNum++;
		_process->addWaitResource(this->resourceList[_resourceType].head);
		this->processManager->blockProcess(_process);
		return nullptr;
	} else {
		Resource* tmp = this->resourceList[_resourceType].firstFree();
		tmp->processControlBlock = new Process(_process->processID);
		tmp->resourceStatus = Occupied;
		this->resourceList[_resourceType].freeNum--;
		_process->addOccupiedResource(tmp);
		return tmp;
	}
}

int ResourceManager::releaseResource(ResourceType _resourceType, Process *_process) {
	Resource* tmp = this->resourceList[_resourceType].getResource(_process->processID);
	if (tmp == nullptr)
		return -1;
	_process->removeOccupiedResource(tmp);
	tmp->resourceStatus = Free;
	tmp->processControlBlock = nullptr;
	this->resourceList[_resourceType].freeNum++;
	if (this->resourceList[_resourceType].freeNum > 0 && this->resourceList[_resourceType].waitNum > 0) {
		this->assignResource(_resourceType);
	}
    return 0;
}

int ResourceManager::assignResource(ResourceType _resourceType) {
	if (!(this->resourceList[_resourceType].freeNum > 0 && this->resourceList[_resourceType].waitNum > 0))
		return -1;
	if (this->resourceList[_resourceType].sysRequestQueue->length > 0) {
		this->requireResource(_resourceType, this->processManager->getProcess(this->resourceList[_resourceType].sysRequestQueue->popFirst()->processID));
	} else {
		this->requireResource(_resourceType, this->processManager->getProcess(this->resourceList[_resourceType].userRequestQueue->popFirst()->processID));
	}
	return 0;
}

int ResourceManager::assignResource(ResourceType _resourceType, Process* _process) {
	Resource* tmp = this->resourceList[_resourceType].firstFree();
	if (tmp == nullptr)
		return -1;
	tmp->resourceStatus = Occupied;
	tmp->processControlBlock = _process;
	this->resourceList[_resourceType].freeNum--;
	if (_process->priority == User) {
		this->resourceList[_resourceType].userRequestQueue->remove(_process);
	} else {
		this->resourceList[_resourceType].sysRequestQueue->remove(_process);
	}
	_process->waitResourceList->remove(_resourceType);
	if (_process->waitResourceList->length == 0)
		this->processManager->unBlockProcess(_process);
	return 0;
}



int ResourceManager::setProcessManager(ProcessManager *_processManager) {
	this->processManager = _processManager;
	return 0;
}

int ResourceManager::deWaitResource(ResourceType _resourceType, Process *_process) {
	if (_process->priority == User) {
		this->resourceList[_resourceType].userRequestQueue->remove(_process);
	}
	if (_process->priority == System) {
		this->resourceList[_resourceType].sysRequestQueue->remove(_process);
	}
	this->resourceList[_resourceType].waitNum--;
	return 0;
}

void ResourceManager::printFullInfo() {
	for (int i = 0; i < RESOURCETYPENUM; i++) {
		this->resourceList[i].printFullInfo();
	}
}

void ResourceManager::printInfo() {
	for (int i = 0; i < RESOURCETYPENUM; i++) {
		this->resourceList[i].printInfo();
	}
}





