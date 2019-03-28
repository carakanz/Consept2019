#pragma once

#include "ICommand.h"
#include <iostream>

template <class T>
class CInCommand : public virtual ICommand {
public:
    static CInCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CInCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeShiftFlags(state);

        T value;
        std::cin >> value;

        SetSecond<T>(state, commandInfo, value);

        state.rip += sizeof(VMCommand);
    }

private:
    CInCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};