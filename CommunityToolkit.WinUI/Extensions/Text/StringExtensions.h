#pragma once

#include <winrt/base.h>
#include <string_view>

namespace winrt::CommunityToolkit::WinUI 
{
	using namespace winrt::Windows::Foundation::Numerics;

	float2 ToVector2(std::wstring_view text);

	float3 ToVector3(std::wstring_view text);

	winrt::hstring ToString(float2 const& value);

	winrt::hstring ToString(float3 const& value);
}