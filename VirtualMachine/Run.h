#pragma once
#include <atomic>
#include <memory>
#include <CommandList.h>

class CRun {
public:
    void Load(std::unique_ptr<VMState> state);
    void Start();
    void Stop();
private:
    void CheckRip();

    std::unique_ptr<VMState> state;
    std::atomic_bool isWork{ false };
};