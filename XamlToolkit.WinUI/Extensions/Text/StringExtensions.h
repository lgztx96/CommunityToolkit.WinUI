#pragma once

#ifdef __INTELLISENSE__
#include <string_view>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI
{
	winrt::float2 ToVector2(std::wstring_view text);

	winrt::float3 ToVector3(std::wstring_view text);

	winrt::hstring ToString(winrt::float2 const& value);

	winrt::hstring ToString(winrt::float3 const& value);
}