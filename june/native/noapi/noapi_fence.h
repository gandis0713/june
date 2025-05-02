#pragma once

#include "june/june.h"
#include "june/native/fence.h"

#include <mutex>
#include <unordered_map>

namespace june
{

class NoApiContext;
class NoApiFence : public Fence
{
public:
    static NoApiFence* create(NoApiContext* context, JuneFenceDescriptor const* descriptor);

public:
    NoApiFence() = delete;
    NoApiFence(NoApiContext* context, JuneFenceDescriptor const* descriptor);
    ~NoApiFence() override = default;

    NoApiFence(const NoApiFence&) = delete;
    NoApiFence& operator=(const NoApiFence&) = delete;

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