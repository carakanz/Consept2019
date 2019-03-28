#pragma once

#include "ICommand.h"

template <uint8_t flagIndex, bool expected>
class CÑompareAndJumpCommand : public virtual ICommand {
public:
    static CÑompareAndJumpCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CÑompareAndJumpCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        bool flag;
        
        switch (flagIndex)
        {
        case (0):
            flag = state.flags.resultÎverflow;
        case (1):
            flag = state.flags.shiftÎverflow;
        case (2):
            flag = state.flags.firstShiftÎverflow;
        case (3):
            flag = state.flags.secondShiftÎverflow;
        case (4):
            flag = state.flags.compare;
        case (5):
            flag = state.flags.equal;
        default:
            throw VMUnsupportedCommandException();
        }

        if (flag != expected) {
            state.rip += sizeof(VMCommand);
            return;
        }

        FreeShiftFlags(state);

        VMULong pos = GetFirstValue(state, commandInfo);
        state.rip = pos;
    }

private:
    CÑompareAndJumpCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};
