#pragma once

#ifdef __INTELLISENSE__
#include <wil/resource.h>
#include <coroutine>
#include <utility>
#include <cassert>
#else
import std;
#endif

namespace winrt
{
    class async_mutex
    {
    private:
        struct waiter;

    public:
        async_mutex() = default;
        async_mutex(async_mutex const&) = delete;
        async_mutex& operator=(async_mutex const&) = delete;

        ~async_mutex()
        {
            assert(m_locked == false);
            assert(m_head == nullptr);
            assert(m_tail == nullptr);
        }

        class async_lock_guard
        {
        public:
            async_lock_guard() noexcept = default;

            explicit async_lock_guard(async_mutex* m) noexcept
                : m_mutex(m)
            {}

            async_lock_guard(async_lock_guard&& other) noexcept
                : m_mutex(std::exchange(other.m_mutex, nullptr))
            {}

            async_lock_guard& operator=(async_lock_guard&& other) noexcept
            {
                if (this != &other)
                {
                    unlock();
                    m_mutex = std::exchange(other.m_mutex, nullptr);
                }
                return *this;
            }

            async_lock_guard(async_lock_guard const&) = delete;
            async_lock_guard& operator=(async_lock_guard const&) = delete;

            ~async_lock_guard()
            {
                unlock();
            }

            void unlock() noexcept
            {
                if (m_mutex)
                {
                    m_mutex->unlock();
                    m_mutex = nullptr;
                }
            }

            explicit operator bool() const noexcept
            {
                return m_mutex != nullptr;
            }

        private:
            async_mutex* m_mutex{};
        };

        auto lock_async() noexcept
        {
            return lock_awaiter{ *this };
        }

    private:
        struct waiter
        {
            std::coroutine_handle<> continuation{};
            waiter* next{ nullptr };
        };

        class lock_awaiter
        {
        public:
            explicit lock_awaiter(async_mutex& m) noexcept
                : m_mutex(m)
            {}

            bool await_ready() noexcept
            {
                auto guard = m_mutex.m_cs.lock();

                if (!m_mutex.m_locked && !m_mutex.m_head)
                {
                    m_mutex.m_locked = true;
                    return true;
                }

                return false;
            }

            bool await_suspend(std::coroutine_handle<> h) noexcept
            {
                m_node.continuation = h;
                m_node.next = nullptr;

                auto guard = m_mutex.m_cs.lock();

                if (!m_mutex.m_locked && !m_mutex.m_head)
                {
                    m_mutex.m_locked = true;
                    return false;
                }

                if (m_mutex.m_tail)
                {
                    m_mutex.m_tail->next = &m_node;
                }
                else
                {
                    m_mutex.m_head = &m_node;
                }

                m_mutex.m_tail = &m_node;

                return true;
            }

            async_lock_guard await_resume() noexcept
            {
                return async_lock_guard{ &m_mutex };
            }

        private:
            async_mutex& m_mutex;
            waiter m_node;
        };

    private:
        void unlock() noexcept
        {
            waiter* next = nullptr;

            {
                auto guard = m_cs.lock();

                assert(m_locked);

                next = m_head;

                if (!next)
                {
                    m_locked = false;
                    return;
                }

                m_head = next->next;

                if (!m_head)
                {
                    m_tail = nullptr;
                }
            }

            next->continuation.resume();
        }

    private:
        wil::critical_section m_cs;

        bool m_locked{ false };

        waiter* m_head{ nullptr };
        waiter* m_tail{ nullptr };
    };
}