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
	float2 ToVector2(std::wstring_view text);

	float3 ToVector3(std::wstring_view text);

	winrt::hstring ToString(float2 const& value);

	winrt::hstring ToString(float3 const& value);
}