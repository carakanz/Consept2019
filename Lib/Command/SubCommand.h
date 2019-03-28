#pragma once

#include "ICommand.h"

template <class T>
class CSubCommand : public virtual ICommand {
public:
    static CSubCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CSubCommand instance(index, name);
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

        SetParameter<T>(state, commandInfo, secondValue - firstValue, second);
        state.rip += sizeof(VMCommand);
    }

private:
    CSubCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};
