#pragma once

#include "ICommand.h"

template <class T>
class CMovCommand : public virtual ICommand {
public:
    static CMovCommand& GetInstance(VMCommandCode index, const std::string& name) {
        static CMovCommand instance(index, name);
        return instance;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) {
        FreeShiftFlags(state);
        T firstValue = GetFirstValue<T>(state, commandInfo);

        SetSecond<T>(state, commandInfo, firstValue);

        state.rip += sizeof(VMCommand);
    }

private:
    CMovCommand(VMCommandCode index, const std::string name) : ICommand(index, name) {};
};