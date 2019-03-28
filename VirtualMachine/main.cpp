#pragma once
#include "Run.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1], std::ios::binary);
    std::unique_ptr<VMState> state = std::make_unique<VMState>();
    in >> state->rax >>
        state->rbx >>
        state->rcx >>
        state->rdx >>
        state->rsp >>
        state->rbp >>
        state->rip >>
        state->interruptStart;
    uint64_t size;
    in >> size;
    state->memory.resize(size);
    in.read(reinterpret_cast<char*>(state->memory.data()), size);

    CRun runner;
    runner.Load(std::move(state));
    runner.Start();
    return 0;
}