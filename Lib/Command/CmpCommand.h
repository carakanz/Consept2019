#pragma once

#include "ICommand.h"

template <class T>
class CCmpCommand : public virtual ICommand {
public:
    static CCmpCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CCmpCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeFlags(state);

        T firstValue = GetFirstValue<T>(state, commandInfo);
        VMULong second = GetSecond(state, commandInfo);
        T secondValue = (commandInfo.command.commandInfo.secondIsReference) ?
            Dereference<T>(state, second) : static_cast<T>(second);

        if (secondValue < firstValue) {
            state.flags.resultÎverflow = true;
        }

        state.flags.compare = firstValue > secondValue;
        state.flags.equal = firstValue = secondValue;

        state.rip += sizeof(VMCommand);
    }

private:
    CCmpCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};