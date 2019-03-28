#pragma once
#include <string>
#include <unordered_map>
#include <array>
#include <limits>
#include <memory>

#include "../Defines.h"

class ICommand;
static std::unordered_map<std::string, const ICommand*> TranslateName;
static std::array<ICommand*, InstructionsNumber> TranslateIndex;

class ICommand {
public:
    ICommand(VMCommandCode index, const std::string& name) : index(index), name(name) {
        TranslateName[name] = this;
        TranslateIndex[index] = this;
    }

    VMCommandCode GetIndex() const {
        return index;
    }

    std::string GetName() {
        return name;
    }

    virtual void DoCommand(VMState& state, const VMCommand& commandInfo) abstract;

    virtual ~ICommand() = default;

    template<class T>
    static inline T& Dereference(VMState& state, VMULong ptr) {
        if (ptr > state.memory.size() - sizeof(T)) {
            throw VMOutOfMemoryException();
        }

        if (ptr == 0) {
            throw VMNullPtrException();
        }
        return reinterpret_cast<T&>(state.memory[ptr]);
    }

    static inline VMULong GetShift(VMState& state, const VMCommand& commandInfo) {
        bool hasShiftÎverflow = false;
        return GetParameter(state,
            commandInfo,
            commandInfo.shiftRegister,
            commandInfo.command.commandInfo.shiftIsRegister,
            commandInfo.command.commandInfo.shiftIsReference,
            false,
            hasShiftÎverflow);
    }

    template <class T>
    static inline T GetFirstValue(VMState& state, const VMCommand& commandInfo) {
        VMULong first = GetFirst(state, commandInfo);
        if (commandInfo.command.commandInfo.firstIsReference) {
            first = Dereference<VMULong>(state, first);
        }
        return static_cast<T>(first);
    }

    static inline VMULong GetFirstValue(VMState& state, const VMCommand& commandInfo) {
        return GetFirstValue<VMULong>(state, commandInfo);
    }

    static inline VMULong GetFirst(VMState& state, const VMCommand& commandInfo) {
        bool hasShiftÎverflow = false;
        VMULong result = GetParameter(state,
            commandInfo,
            commandInfo.firstRegister,
            commandInfo.command.commandInfo.firstIsRegister,
            commandInfo.command.commandInfo.firstIsReference,
            commandInfo.command.commandInfo.firstHasShift,
            hasShiftÎverflow);
        state.flags.firstShiftÎverflow = hasShiftÎverflow;
        return result;
    }

    static inline VMULong GetSecond(VMState& state, const VMCommand& commandInfo) {
        bool hasShiftÎverflow = false;
        VMULong result = GetParameter(state,
            commandInfo,
            commandInfo.secondRegister,
            commandInfo.command.commandInfo.secondIsRegister,
            commandInfo.command.commandInfo.secondIsReference,
            commandInfo.command.commandInfo.secondHasShift,
            hasShiftÎverflow);
        state.flags.secondShiftÎverflow = hasShiftÎverflow;
        return result;
    }

    static inline VMULong GetParameter(VMState& state, const VMCommand& commandInfo, 
        VMRegister vmRegister, bool isRegister,
        bool isReference, bool hasShift, 
        bool& shiftÎverflow) {
        VMULong result = vmRegister;

        if (isRegister) {
            result = state.Register(result);
        }

        if (hasShift) {
            if (commandInfo.command.shiftSize == 0) {
                throw VMShiftSizeIsZeroException();
            }
            VMLong shift = GetShift(state, commandInfo);
            if (commandInfo.command.commandInfo.shiftIsReference) {
                shift = Dereference<VMLong>(state, shift);
            }
            VMULong positiveShift = std::abs(shift);

            if (positiveShift > std::numeric_limits<VMULong>::max() / commandInfo.command.shiftSize) {
                state.flags.shiftÎverflow = true;
                shiftÎverflow = true;
                if (isReference) {
                    throw VMShiftÎverflowException();
                }
            }

            positiveShift *= commandInfo.command.shiftSize;

            if (shift >= 0) {
                if (positiveShift > std::numeric_limits<VMULong>::max() - result) {
                    shiftÎverflow = true;
                    if (isReference) {
                        throw VMShiftÎverflowException();
                    }
                }
                result += shiftÎverflow;
            } else {
                if (positiveShift > result) {
                    shiftÎverflow = true;
                    if (isReference) {
                        throw VMShiftÎverflowException();
                    }
                }
                result -= shiftÎverflow;
            }
        }
        return result;
    }

    template <class T>
    static inline void SetParameter(VMState& state, const VMCommand& commandInfo,
        T value, VMLong addres = 0) {
        if (commandInfo.command.commandInfo.secondIsReference) {
            Dereference<T>(state, addres) = value;
        } else if (commandInfo.command.commandInfo.secondIsRegister) {
            reinterpret_cast<T&>(state.Register(commandInfo.secondRegister)) = value;
        } else {
            throw VMUnsupportedCommandException();
        }
    }

    template <class T>
    static inline void SetSecond(VMState& state, const VMCommand& commandInfo,
        T value) {
        if (commandInfo.command.commandInfo.secondIsReference) {
            VMULong second = GetSecond(state, commandInfo);
            Dereference<T>(state, second) = value;
        }
        else if (commandInfo.command.commandInfo.secondIsRegister) {
            reinterpret_cast<T&>(state.Register(commandInfo.secondRegister)) = value;
        }
        else {
            throw VMUnsupportedCommandException();
        }
    }

    static inline void FreeFlags(VMState& state) {
        state.flags = VMFlagRegister();
    }

    static inline void FreeShiftFlags(VMState& state) {
        state.flags.firstShiftÎverflow = false;
        state.flags.secondShiftÎverflow = false;
        state.flags.shiftÎverflow = false;
    }

private:
    const VMCommandCode index;
    const std::string name;
};