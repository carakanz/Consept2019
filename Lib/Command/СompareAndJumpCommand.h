#pragma once

#include "ICommand.h"

template <uint8_t flagIndex, bool expected>
class C�ompareAndJumpCommand : public virtual ICommand {
public:
    static C�ompareAndJumpCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static C�ompareAndJumpCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        bool flag;
        
        switch (flagIndex)
        {
        case (0):
            flag = state.flags.result�verflow;
        case (1):
            flag = state.flags.shift�verflow;
        case (2):
            flag = state.flags.firstShift�verflow;
        case (3):
            flag = state.flags.secondShift�verflow;
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
    C�ompareAndJumpCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};
