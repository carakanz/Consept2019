#pragma once

#include "ICommand.h"

template <class T>
class CPopCommand : public virtual ICommand {
public:
    static CPopCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CPopCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {

        if (state.rsp < state.rbp + sizeof(T)) {
            throw VMStackFreeException();
        }

        FreeShiftFlags(state);

        state.rsp -= sizeof(T);

        T firstValue = Dereference<T>(state, state.rsp);

        SetSecond<T>(state, commandInfo, firstValue);

        state.rip += sizeof(VMCommand);
    }

private:
    CPopCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
}; 