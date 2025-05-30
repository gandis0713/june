#include "sync_handle.h"
#include <algorithm>

#include <unistd.h>

namespace june
{

namespace
{

#if defined(_WIN32)
// TODO
#else
SyncHandle::Handle duplicateHandle(SyncHandle::Handle handle)
{
    if (handle == -1)
    {
        return -1; // Return an invalid handle if the input is invalid
    }

    return dup(handle);
}

void closeHandle(SyncHandle::Handle handle)
{
    if (handle != -1)
    {
        ::close(handle); // Close the file descriptor
    }
}
#endif

} // namespace

SyncHandle SyncHandle::duplicate(Handle handle)
{
    SyncHandle newSyncHandle;
#if defined(_WIN32)
    // TODO
#else
    if (newSyncHandle.m_handle != -1)
    {
        newSyncHandle.m_handle = duplicateHandle(handle);
    }
    else
    {
        newSyncHandle.m_handle = -1; // Set to an invalid handle if duplication fails
    }
#endif

    return std::move(newSyncHandle);
}

SyncHandle::SyncHandle()
#if defined(_WIN32)
    : m_handle(nullptr)
#else
    : m_handle(-1)
#endif
{
}
SyncHandle::SyncHandle(Handle handle)
    : m_handle(handle)
{
}

SyncHandle::SyncHandle(SyncHandle&& rhs)
{
    // Move constructor
    // Transfer ownership of the handle
#if defined(_WIN32)
    // TODO
#else
    m_handle = std::exchange(rhs.m_handle, -1);
#endif
}

SyncHandle& SyncHandle::operator=(SyncHandle&& rhs)
{
    // Move assignment operator
    // Transfer ownership of the handle
#if defined(_WIN32)
// TODO
#else
    m_handle = std::exchange(rhs.m_handle, -1);
#endif
    return *this;
}

SyncHandle SyncHandle::duplicate()
{
    SyncHandle newHandle;
#if defined(_WIN32)
    // TODO
#else
    newHandle.m_handle = duplicateHandle(m_handle);
#endif
    return std::move(newHandle);
}

SyncHandle::Handle SyncHandle::getHandle() const
{
    return m_handle;
}

void SyncHandle::close()
{
#if defined(_WIN32)
    // TODO
#else
    if (m_handle != -1)
    {
        closeHandle(m_handle);
        m_handle = -1;
    }
#endif
}

} // namespace june