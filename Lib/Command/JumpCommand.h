#pragma once

#include "ICommand.h"

class CJumpCommand : public virtual ICommand {
public:
    static CJumpCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CJumpCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeShiftFlags(state);
        VMULong pos = GetFirstValue(state, commandInfo);
        state.rip = pos;
    }

private:
    CJumpCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};