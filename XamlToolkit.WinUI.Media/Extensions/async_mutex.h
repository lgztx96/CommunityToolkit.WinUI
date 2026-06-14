#pragma once

#ifdef __INTELLISENSE__
#include <wil/resource.h>
#include <coroutine>
#include <utility>
#include <cassert>
#endif

import std;

namespace winrt
{
    class async_mutex
    {
    public:
        async_mutex() = default;

        async_mutex(async_mutex const&) = delete;
        async_mutex& operator=(async_mutex const&) = delete;

        ~async_mutex() = default;

        class async_lock_guard
        {
        public:
            async_lock_guard() noexcept = default;

            explicit async_lock_guard(async_mutex* mutex) noexcept
                : m_mutex(mutex)
            {
            }

            async_lock_guard(async_lock_guard const&) = delete;

            async_lock_guard& operator=(async_lock_guard const&) = delete;

            async_lock_guard(
                async_lock_guard&& other) noexcept
                : m_mutex(std::exchange(other.m_mutex, nullptr))
            {
            }

            async_lock_guard& operator=(async_lock_guard&& other) noexcept
            {
                if (this != &other)
                {
                    unlock();

                    m_mutex = std::exchange(other.m_mutex, nullptr);
                }

                return *this;
            }

            ~async_lock_guard()
            {
                unlock();
            }

            explicit operator bool() const noexcept
            {
                return m_mutex != nullptr;
            }

            void unlock() noexcept
            {
                if (!m_mutex)
                {
                    return;
                }

                m_mutex->unlock();
                m_mutex = nullptr;
            }

        private:
            async_mutex* m_mutex{};
        };

    private:
        struct waiter
        {
            std::coroutine_handle<> continuation{};
            waiter* next{};
        };

        struct lock_awaiter : waiter
        {
            explicit lock_awaiter(async_mutex& mutex) noexcept
                : m_mutex(mutex)
            {
            }

            bool await_ready() noexcept
            {
                auto guard = m_mutex.m_cs.lock();

                // Fair fast path:
                // acquire only if:
                // 1. lock free
                // 2. no queued waiter
                if (!m_mutex.m_locked && !m_mutex.m_waiters_head)
                {
                    m_mutex.m_locked = true;
                    return true;
                }

                return false;
            }

            bool await_suspend(std::coroutine_handle<> handle) noexcept
            {
                this->continuation = handle;

                this->next = nullptr;

                auto guard = m_mutex.m_cs.lock();

                // Double check:
                // lock may become available between
                // await_ready() and await_suspend()
                if (!m_mutex.m_locked && !m_mutex.m_waiters_head)
                {
                    m_mutex.m_locked = true;
                    return false;
                }

                // FIFO intrusive queue
                if (m_mutex.m_waiters_tail)
                {
                    m_mutex.m_waiters_tail->next = this;
                }
                else
                {
                    m_mutex.m_waiters_head = this;
                }

                m_mutex.m_waiters_tail = this;

                return true;
            }

            async_lock_guard await_resume() noexcept
            {
                return async_lock_guard{ &m_mutex };
            }

        private:
            async_mutex& m_mutex;
        };

    public:
        [[nodiscard]]
        auto lock_async() noexcept
        {
            return lock_awaiter{ *this };
        }

    private:
        void unlock() noexcept
        {
            waiter* next_waiter = nullptr;

            {
                auto guard = m_cs.lock();

#ifdef _DEBUG
                assert(m_locked);
#endif

                next_waiter = m_waiters_head;

                // No waiter:
                // release mutex
                if (!next_waiter)
                {
                    m_locked = false;
                    return;
                }

                // FIFO pop
                m_waiters_head = next_waiter->next;

                if (!m_waiters_head)
                {
                    m_waiters_tail = nullptr;
                }

                // ownership transfer
                // keep m_locked = true
            }

            // Resume outside lock
            next_waiter->continuation.resume();
        }

    private:
        wil::critical_section m_cs;

        bool m_locked{ false };

        waiter* m_waiters_head{};
        waiter* m_waiters_tail{};
    };
}