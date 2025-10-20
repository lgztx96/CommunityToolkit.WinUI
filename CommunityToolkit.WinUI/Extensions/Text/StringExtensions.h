#pragma once

#include <winrt/base.h>
#include <string_view>

namespace winrt::CommunityToolkit::WinUI 
{
	winrt::Windows::Foundation::Numerics::float2 ToVector2(std::wstring_view text);

	winrt::Windows::Foundation::Numerics::float3 ToVector3(std::wstring_view text);
}