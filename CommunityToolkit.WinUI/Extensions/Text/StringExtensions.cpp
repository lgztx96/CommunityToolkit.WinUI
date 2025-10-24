#pragma once

#include "pch.h"
#include "StringExtensions.h"
#include <ranges>

using std::operator""sv;

namespace winrt::CommunityToolkit::WinUI
{
	static constexpr std::wstring_view Unbracket(std::wstring_view text)
	{
		if (text.size() >= 2 && text[0] == '<' && text[text.size() - 1] == '>')
		{
			return text.substr(1, static_cast<std::size_t>(text.size()) - 2);
		}

		return text;
	}

	float2 ToVector2(std::wstring_view text)
	{
		if (text.empty())
		{
			float2::zero();
		}

		text = Unbracket(text);
		if (text.find(',') == std::wstring_view::npos)
		{
			float result = std::wcstof(text.data(), nullptr);

			return winrt::Windows::Foundation::Numerics::float2{ result };
		}
		else
		{
			auto vec = std::views::split(text, L","sv)
				| std::views::transform([](auto&& range) { return std::wstring_view(range); })
				| std::ranges::to<std::vector>();

			if (vec.size() == 2)
			{
				float result2 = std::wcstof(vec[0].data(), nullptr);
				float result3 = std::wcstof(vec[1].data(), nullptr);
				return winrt::Windows::Foundation::Numerics::float2(result2, result3);
			}
		}

		throw winrt::hresult_invalid_argument(winrt::format(L"Cannot convert {} to Vector3. Use the format \"float, float\"", text));
	}

	float3 ToVector3(std::wstring_view text)
	{
		if (text.empty())
		{
			return winrt::Windows::Foundation::Numerics::float3::zero();
		}

		text = Unbracket(text);
		if (text.find(',') == std::wstring_view::npos)
		{
			float result = std::wcstof(text.data(), nullptr);

			return float3{ result };
		}
		else
		{
			auto vec = std::views::split(text, L","sv)
				| std::views::transform([](auto&& range) { return std::wstring_view(range); })
				| std::ranges::to<std::vector>();

			if (vec.size() == 3)
			{
				float result2 = std::wcstof(vec[0].data(), nullptr);
				float result3 = std::wcstof(vec[1].data(), nullptr);
				float result4 = std::wcstof(vec[2].data(), nullptr);
				return float3(result2, result3, result4);
			}
			else if (vec.size() == 2)
			{
				return float3(ToVector2(text), 0.0f);
			}
		}

		throw winrt::hresult_invalid_argument(winrt::format(L"Cannot convert {} to Vector3. Use the format \"float, float\"", text));
	}

	winrt::hstring ToString(float2 const& value)
	{
		return winrt::format(L"<{}, {}>", value.x, value.y);
	}

	winrt::hstring ToString(float3 const& value)
	{
		return winrt::format(L"<{}, {}, {}>", value.x, value.y, value.z);
	}
}