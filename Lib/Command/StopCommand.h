#pragma once

#include "ICommand.h"

class CStopCommand : public virtual ICommand {
public:
    static CStopCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CStopCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        throw VMStopInstructionException();
    }

private:
    CStopCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};