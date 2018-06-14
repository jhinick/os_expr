//
// Created by hjh on 18-5-28.
//

#include "Resource.h"
#include "ProcessControlBlock.h"
#include "ProcessResourceManager.h"
#include "UserInterface.h"

#include <cstdio>




int main(int argc, char** argv) {
    ProcessResourceManager processResourceManager;
    UserInterface userInterface = UserInterface(&processResourceManager);
    userInterface.run();
    return 0;
}

