#pragma once
#include <winrt/Windows.Foundation.Numerics.h>

namespace winrt
{
	using namespace winrt::Windows::Foundation::Numerics;
}

namespace winrt
{

	[[nodiscard]] inline winrt::float2 string_to_float2(winrt::hstring const& str)
	{
		float x{ 0.0f }, y{ 0.0f };
		auto s = winrt::to_string(str);

		std::size_t comma = s.find(',');

		if (comma != std::string::npos)
		{
			x = std::stof(s.substr(0, comma));
			y = std::stof(s.substr(comma + 1));
		}
		return { x, y };
	}

	[[nodiscard]] inline winrt::float3 string_to_float3(winrt::hstring const& str)
	{
		float x{ 0.0f }, y{ 0.0f }, z{ 0.0f };
		auto s = winrt::to_string(str);

		std::size_t firstComma = s.find(',');
		std::size_t secondComma = s.rfind(',');

		if (firstComma != std::string::npos && secondComma != std::string::npos && firstComma != secondComma)
		{
			x = std::stof(s.substr(0, firstComma));
			y = std::stof(s.substr(firstComma + 1, secondComma - firstComma - 1));
			z = std::stof(s.substr(secondComma + 1));
		}
		return { x, y, z };
	}

	[[nodiscard]] inline winrt::hstring float2_to_string(winrt::float2 const& v)
	{
		std::wostringstream ss;
		ss << v.x << "," << v.y;
		return winrt::hstring(ss.str());
	}

	[[nodiscard]] inline winrt::hstring float3_to_string(winrt::float3 const& v)
	{
		std::wostringstream ss;
		ss << v.x << "," << v.y << "," << v.z;
		return winrt::hstring(ss.str());
	}
}