#pragma once
#include <atomic>
#include <mutex>


namespace SC
{



class SpinLockMutex
{
private:
    std::atomic_flag mutex{ ATOMIC_FLAG_INIT };
public:
    inline SpinLockMutex(void) {}

    inline void lock(void)
    {
        while (mutex.test_and_set(std::memory_order_acquire))
            ;
    }

    inline void unlock(void)
    {
        mutex.clear(std::memory_order_release);
    }
};

typedef SpinLockMutex mutex;
typedef std::lock_guard<SpinLockMutex> scoped_lock;

}