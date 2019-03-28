#include "Run.h"
#include <iostream>

void CRun::Load(std::unique_ptr<VMState> state) {
    this->state = std::move(state);
}

void CRun::Start() {
    isWork.store(true);
    std::cout << "LOG(info): start" << std::endl;
    while (isWork.load()) {
        try {
            CheckRip();
            VMCommand& commandInfo =
                reinterpret_cast<VMCommand&>(state->memory[state->rip]);
            TranslateIndex[commandInfo.command.commandIndex]->
                DoCommand(*state, commandInfo);
        }
        catch (const VMStopInstructionException& exeption) {
            std::cout << "LOG(info): stop" << std::endl;
            isWork.store(false);
        }
        catch (const VMIRuntimeException& exeption) {
            std::cout << "LOG(error): " << exeption.GetIndex() << std::endl;
            state->rip = state->interruptStart +
                exeption.GetIndex() * sizeof(VMCommand);
        }
    }
}

void CRun::Stop() {
    isWork.store(false);
}

void CRun::CheckRip() {
    if (state->memory.size() - sizeof(VMCommand) > state->rip) {
        throw VMOutOfMemoryException();
    }
}
