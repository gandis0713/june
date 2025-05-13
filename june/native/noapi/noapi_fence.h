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
    static NoApiFence* create(NoApiContext* context, JuneFenceCreateDescriptor const* descriptor);

public:
    NoApiFence() = delete;
    NoApiFence(NoApiContext* context, JuneFenceCreateDescriptor const* descriptor);
    ~NoApiFence() override = default;

    NoApiFence(const NoApiFence&) = delete;
    NoApiFence& operator=(const NoApiFence&) = delete;

public: // June API
    void reset(JuneFenceResetDescriptor const* descriptor) override;

public:
    int getSyncFD() const override;

private:
    mutable std::mutex m_mutex;
    int m_signalFd{ -1 };
};

} // namespace june