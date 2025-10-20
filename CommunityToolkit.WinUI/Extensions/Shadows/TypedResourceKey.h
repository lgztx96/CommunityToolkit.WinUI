#pragma once
#include <winrt/base.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

template <typename TValue>
struct TypedResourceKey
{
    constexpr TypedResourceKey(std::wstring_view key) : key(key) {};
    operator winrt::param::hstring() const noexcept { return key; }
    std::wstring_view Key() const noexcept { return key; }

private:
    std::wstring_view key;
};