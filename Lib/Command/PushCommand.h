#pragma once

#include "ICommand.h"

template <class T>
class CPushCommand : public virtual ICommand {
public:
    static CPushCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CPushCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {

        if (state.rsp > state.memory.size() - sizeof(T)) {
            throw VMStackÎverflowException();
        }

        FreeShiftFlags(state);

        T firstValue = GetFirstValue<T>(state, commandInfo);
        Dereference<T>(state, state.rsp) = firstValue;

        state.rsp += sizeof(T);      
        state.rip += sizeof(VMCommand);
    }

private:
    CPushCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};