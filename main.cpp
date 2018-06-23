//
// Created by hjh on 18-5-28.
//

#include "Resource.h"
#include "Process.h"
#include "UserInterface.h"

int main(int argc, char** argv) {
    ProcessManager processResourceManager;
    ResourceManager resourceManager;
    UserInterface userInterface = UserInterface(&processResourceManager, &resourceManager);
    userInterface.run();
    return 0;
}

