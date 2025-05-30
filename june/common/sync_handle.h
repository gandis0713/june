#pragma once

namespace june
{

class SyncHandle
{
public:
#if defined(_WIN32)
    using Handle = void*;
#else
    using Handle = int;
#endif

    static SyncHandle duplicate(Handle handle);

public:
    SyncHandle();
    SyncHandle(Handle handle);
    virtual ~SyncHandle() = default;

    SyncHandle(const SyncHandle&) = delete;
    SyncHandle& operator=(const SyncHandle&) = delete;

    SyncHandle(SyncHandle&&);
    SyncHandle& operator=(SyncHandle&&);

public:
    SyncHandle duplicate();
    Handle getHandle() const;
    void close();

private:
    Handle m_handle{};
};

} // namespace june