#pragma once

#include "ICommand.h"

template <class T>
class CAddCommand : public virtual ICommand {
public:
    static CAddCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CAddCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeFlags(state);

        T firstValue = GetFirstValue<T>(state, commandInfo);
        VMULong second = GetSecond(state, commandInfo);
        T secondValue = (commandInfo.command.commandInfo.secondIsReference) ?
            Dereference<T>(state, second) : static_cast<T>(second);

        if (secondValue > std::numeric_limits<T>::max() - firstValue) {
            state.flags.resultÎverflow = true;
        }

        SetParameter<T>(state, commandInfo, firstValue + secondValue, second);
        ++state.rip;
    }

private:
    CAddCommand(VMCommandCode index, const std::string& name) : ICommand(index, name) {};
};