#pragma once

#include "ICommand.h"

class CRetCommand : public virtual ICommand {
public:
    static CRetCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CRetCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {

        if (state.rsp > state.memory.size() - 2 * sizeof(VMRegister)) {
            throw VMStackÎverflowException();
        }

        FreeShiftFlags(state);

        VMULong firstValue = GetFirstValue<VMULong>(state, commandInfo);

        Dereference<VMRegister>(state, state.rsp) = state.rip + sizeof(VMCommand);
        state.rsp += sizeof(VMRegister);
        Dereference<VMRegister>(state, state.rsp) = state.rbp;
        state.rsp += sizeof(VMRegister);
        state.rbp = state.rsp;

        state.rip = firstValue;
    }

private:
    CRetCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};