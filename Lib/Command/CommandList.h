#pragma once

#include <memory>
#include <array>
#include <unordered_map>

#include "ICommand.h"
#include "MovCommand.h"
#include "AddCommand.h"
#include "SubCommand.h"
#include "MulCommand.h"
#include "DivCommand.h"
#include "CmpCommand.h"
#include "JumpCommand.h"
#include "—ompareAndJumpCommand.h"
#include "PopCommand.h"
#include "PushCommand.h"
#include "CallCommand.h"
#include "RetCommand.h"
#include "InCommnad.h"
#include "OutCommand.h"
#include "StopCommand.h"

namespace CommandList {

    static int CommandListInit() {
        // 0 ¡ÂÁ ÍÓÏ‡Ì‰˚. «‡˘ËÚ‡ ÓÚ ‰Û‡Í‡.
        CMovCommand<VMUInt64>::GetInstance(1, "movq");
        CMovCommand<VMUInt32>::GetInstance(2, "movl");
        CMovCommand<VMUInt16>::GetInstance(3, "movw");
        CMovCommand<VMUInt8>::GetInstance(4, "movb");

        CAddCommand<VMUInt64>::GetInstance(5, "addq");
        CAddCommand<VMUInt32>::GetInstance(6, "addl");
        CAddCommand<VMUInt16>::GetInstance(7, "addw");
        CAddCommand<VMUInt8>::GetInstance(8, "addb");

        CSubCommand<VMUInt64>::GetInstance(9, "subq");
        CSubCommand<VMUInt32>::GetInstance(10, "subl");
        CSubCommand<VMUInt16>::GetInstance(11, "subw");
        CSubCommand<VMUInt8>::GetInstance(12, "subb");

        CDivCommand<VMUInt64>::GetInstance(13, "mulq");
        CDivCommand<VMUInt32>::GetInstance(14, "mull");
        CDivCommand<VMUInt16>::GetInstance(15, "mulw");
        CDivCommand<VMUInt8>::GetInstance(16, "mulb");

        CDivCommand<VMUInt64>::GetInstance(17, "divq");
        CDivCommand<VMUInt32>::GetInstance(18, "divl");
        CDivCommand<VMUInt16>::GetInstance(19, "divw");
        CDivCommand<VMUInt8>::GetInstance(20, "divb");

        CCmpCommand<VMUInt64>::GetInstance(21, "cmpq");
        CCmpCommand<VMUInt32>::GetInstance(21, "cmpl");
        CCmpCommand<VMUInt16>::GetInstance(23, "cmpw");
        CCmpCommand<VMUInt8>::GetInstance(24, "cmpb");

        CJumpCommand::GetInstance(25, "jmp");

        C—ompareAndJumpCommand<0, true>::GetInstance(26, "jro");
        C—ompareAndJumpCommand<0, false>::GetInstance(27, "jnro");
        C—ompareAndJumpCommand<1, true>::GetInstance(28, "jsho");
        C—ompareAndJumpCommand<1, false>::GetInstance(29, "jnsho");
        C—ompareAndJumpCommand<2, true>::GetInstance(30, "jfo");
        C—ompareAndJumpCommand<2, false>::GetInstance(31, "jnfo");
        C—ompareAndJumpCommand<3, true>::GetInstance(31, "jso");
        C—ompareAndJumpCommand<3, false>::GetInstance(33, "jnso");
        C—ompareAndJumpCommand<4, true>::GetInstance(34, "ja");
        C—ompareAndJumpCommand<4, false>::GetInstance(35, "jna");
        C—ompareAndJumpCommand<5, true>::GetInstance(36, "je");
        C—ompareAndJumpCommand<5, false>::GetInstance(37, "jne");

        CPopCommand<VMUInt64>::GetInstance(38, "popq");
        CPopCommand<VMUInt32>::GetInstance(39, "popl");
        CPopCommand<VMUInt16>::GetInstance(40, "popw");
        CPopCommand<VMUInt8>::GetInstance(41, "popb");

        CPushCommand<VMUInt64>::GetInstance(42, "pushq");
        CPushCommand<VMUInt32>::GetInstance(43, "pushl");
        CPushCommand<VMUInt16>::GetInstance(44, "pushw");
        CPushCommand<VMUInt8>::GetInstance(45, "pushb");

        CCallCommand::GetInstance(46, "call");
        CRetCommand::GetInstance(47, "ret");

        CInCommand<VMUInt64>::GetInstance(48, "inq");
        CInCommand<VMUInt32>::GetInstance(49, "inl");
        CInCommand<VMUInt16>::GetInstance(50, "inw");
        CInCommand<VMUInt8>::GetInstance(51, "inb");

        COutCommand<VMUInt64>::GetInstance(52, "outq");
        COutCommand<VMUInt32>::GetInstance(53, "outl");
        COutCommand<VMUInt16>::GetInstance(54, "outw");
        COutCommand<VMUInt8>::GetInstance(55, "outb");

        CStopCommand::GetInstance(56, "stop");

        return 0;
    }
    const auto IsInit = CommandListInit();
}