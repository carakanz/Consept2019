#pragma once

#include "ICommand.h"
#include <iostream>

template <class T>
class COutCommand : public virtual ICommand {
public:
    static COutCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static COutCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeShiftFlags(state);

        FreeShiftFlags(state);
        T firstValue = GetFirstValue<T>(state, commandInfo);

        std::cout << firstValue;

        state.rip += sizeof(VMCommand);
    }

private:
    COutCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};