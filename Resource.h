//
// Created by hjh on 18-5-27.
//

#include "ProcessControlBlock.h"

#ifndef OS_EXPR_RESOURCE_H
#define OS_EXPR_RESOURCE_H

/**
 * Every resource is identified by a unique int number.
 */

 /* Indicates how many types of resource exist in the system.*/
#ifndef RESOURCE_TYPE
#define RESOURCE_TYPE 4
#endif

 enum ResourceType {ReadOnly, ReadWrite, WriteOnly};

 class Resource {
 public:
     int        resourceID;
     Resource*  next;
     /*This list is used to store all processes occupied or waiting for this resource.*/
     ProcessControlBlockList processControlBlockList;
 public:
     Resource();
     ~Resource();

 };

 class ResourceList {
 public:
     Resource*      head;
     Resource*      tail;
     int            length;
 public:
     ResourceList();
     ~ResourceList();
 };

#endif //OS_EXPR_RESOURCE_H
