#pragma once

#ifdef __INTELLISENSE__
#include <string_view>
#include <winrt/Windows.Foundation.h>
#endif

template <typename TValue>
struct TypedResourceKey
{
    constexpr TypedResourceKey(std::wstring_view key) : key(key) {};
    operator winrt::param::hstring() const noexcept { return key; }
    std::wstring_view Key() const noexcept { return key; }

private:
    std::wstring_view key;
};