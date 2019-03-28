#pragma once

#include <cstdint>
#include <type_traits>
#include <vector>

using VMCommandCode = uint16_t;
using VMShiftSize = uint32_t;
using VMULong = uint64_t;
using VMLong = int64_t;
using VMUInt64 = uint64_t;
using VMUInt32 = uint32_t;
using VMUInt16 = uint16_t;
using VMUInt8 = uint8_t;
using VMRegister = VMULong;

constexpr size_t InstructionsNumber = 100;

#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

PACK(struct VMFlagRegister {
    bool resultОverflow : 1;
    bool shiftОverflow : 1;
    bool firstShiftОverflow : 1;
    bool secondShiftОverflow : 1;
    bool compare : 1;
    bool equal : 1;
    uint8_t reserve[7];
});
static_assert(sizeof(VMFlagRegister) == 8, "sizeof(VMFlagRegister) != 8");

PACK(struct VMParameterInfo {
    bool firstIsRegister : 1; // true - регистр, false число
    bool firstIsReference : 1; // true - значение, false ссылка
    bool firstHasShift : 1; // нужно ли прибовлять сдвиг
    bool secondIsRegister : 1; // true - регистр, false число
    bool secondIsReference : 1; // true - значение, false ссылка
    bool secondHasShift : 1; // нужно ли прибовлять сдвиг
    bool shiftIsRegister : 1; // true - регистр, false число
    bool shiftIsReference : 1; // true - значение, false ссылка
});
static_assert(sizeof(VMParameterInfo) == 1, "sizeof(VMParameterInfo) != 1");

PACK(struct VMCommandInfo {
    VMCommandCode commandIndex;
    VMParameterInfo commandInfo;
    uint8_t reserved;
    VMShiftSize shiftSize;
});
static_assert(sizeof(VMCommandInfo) == 8, "sizeof(VMCommandInfo) != 8");

PACK(struct VMCommand {
public:
    VMCommandInfo command;
    VMRegister firstRegister;
    VMRegister secondRegister;
    VMRegister shiftRegister;
});
static_assert(sizeof(VMCommand) == 32, "sizeof(VMCommand) != 32");

using VMInterruptIndex = uint32_t;
class VMIRuntimeException : virtual public std::exception {
public:
    virtual VMInterruptIndex GetIndex() const abstract;
    ~VMIRuntimeException() = default;
};

template <VMInterruptIndex index>
class VMRuntimeException : virtual public VMIRuntimeException {
public:
    virtual VMInterruptIndex GetIndex() const {
        return index;
    }
    ~VMRuntimeException() = default;
};

using VMStopInstructionException = VMRuntimeException<0>;
using VMRegisterIndexException = VMRuntimeException<1>;
using VMUnsupportedCommandException = VMRuntimeException<2>;
using VMNullPtrException = VMRuntimeException<3>;
using VMOutOfMemoryException = VMRuntimeException<4>;
using VMShiftОverflowException = VMRuntimeException<5>;
using VMShiftSizeIsZeroException = VMRuntimeException<6>;
using VMStackОverflowException = VMRuntimeException<6>;
using VMStackFreeException = VMRuntimeException<7>;

struct VMState
{
    VMRegister rax;
    VMRegister rbx;
    VMRegister rcx;
    VMRegister rdx;

    VMRegister rbp;
    VMRegister rsp;

    VMRegister rip;

    VMFlagRegister flags;
    VMRegister interruptStart;

    std::vector<std::byte> memory;

    VMRegister& Register(VMULong index) {
        switch (index)
        {
        case 0:
            return rax;
        case 1:
            return rbx;
        case 2:
            return rax;
        case 3:
            return rcx;
        case 4:
            return rdx;
        case 5:
            return rbp;
        case 6:
            return rsp;
        case 7:
            return rip;
        case 8:
            return interruptStart;
        default:
            throw VMRegisterIndexException();
        }
    }
};
