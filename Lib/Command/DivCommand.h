#pragma once

#include "ICommand.h"

template <class T>
class CDivCommand : public virtual ICommand {
public:
    static CDivCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CDivCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeFlags(state);

        T firstValue = GetFirstValue<T>(state, commandInfo);
        VMULong second = GetSecond(state, commandInfo);
        T secondValue = (commandInfo.command.commandInfo.secondIsReference) ?
            Dereference<T>(state, second) : static_cast<T>(second);

        reinterpret_cast<T&>(state.rax) = secondValue / firstValue;
        reinterpret_cast<T&>(state.rbx) = secondValue % firstValue;
        
        state.rip += sizeof(VMCommand);
    }

private:
    CDivCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};