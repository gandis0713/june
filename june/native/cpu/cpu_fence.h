#pragma once

#include "june/june.h"
#include "june/native/fence.h"

#include <mutex>
#include <unordered_map>

namespace june
{

class CPUContext;
class CPUFence : public Fence
{
public:
    static CPUFence* create(CPUContext* context, JuneFenceDescriptor const* descriptor);

public:
    CPUFence() = delete;
    CPUFence(CPUContext* context, JuneFenceDescriptor const* descriptor);
    ~CPUFence() override = default;

    CPUFence(const CPUFence&) = delete;
    CPUFence& operator=(const CPUFence&) = delete;

public:
    void refresh() override;

public:
    int getFd() const;

private:
    void createFd();

private:
    mutable std::mutex m_mutex;

    int m_signalFd{ -1 };
};

} // namespace june