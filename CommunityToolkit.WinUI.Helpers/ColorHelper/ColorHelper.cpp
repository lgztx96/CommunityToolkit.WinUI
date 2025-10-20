#include "pch.h"
#include "ColorHelper.h"
#if __has_include("ColorHelper.g.cpp")
#include "ColorHelper.g.cpp"
#endif
#include <ranges>

namespace winrt::CommunityToolkit::WinUI::Helpers::implementation
{
	template <typename charT>
	constexpr auto split(std::basic_string_view<charT> view, std::basic_string_view<charT> delim)
	{
		return std::views::split(view, delim) | std::views::transform([](auto&& range)
			{
				return std::basic_string_view<charT>(range);
			});
	}

	template<std::integral T>
	[[nodiscard]] constexpr std::optional<T> to_num(std::string_view s, int base = 10) noexcept {
		T value{};
		auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, base);
		if (ec == std::errc{} && ptr == s.data() + s.size())
			return value;
		return std::nullopt;
	}

	template<std::floating_point T>
	[[nodiscard]] constexpr std::optional<T> to_num(std::string_view s, std::chars_format fmt = std::chars_format::general) noexcept {
		T value{};
		auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), value, fmt);
		if (ec == std::errc{} && ptr == s.data() + s.size())
			return value;
		return std::nullopt;
	}

	constexpr uint32_t to_uint32(std::string_view s, int base = 10) noexcept {
		return to_num<uint32_t>(s, base).value_or(0);
	}

	constexpr uint16_t to_uint16(std::string_view s, int base = 10) noexcept {
		return to_num<uint16_t>(s, base).value_or(0);
	}

	constexpr double to_double(std::string_view s) noexcept {
		return to_num<double>(s).value_or(0.0);
	}

	static inline uint32_t to_uint32(std::wstring_view s, int base = 10)
	{
		return static_cast<uint32_t>(std::wcstoul(s.data(), nullptr, base));
	};

	static inline uint16_t to_uint16(std::wstring_view s, int base = 10)
	{
		return static_cast<uint16_t>(std::wcstoul(s.data(), nullptr, base));
	};

	static inline double to_double(std::wstring_view s)
	{
		return std::stod(std::wstring(s));
	};

	Color ColorHelper::ToColor(winrt::hstring const& colorString)
	{
		using namespace std::string_view_literals;
		std::wstring_view colorView{ colorString.data(), colorString.size() };
		if (colorView.empty())
		{
			throw winrt::hresult_invalid_argument(L"The parameter \"colorString\" must not be null or empty.");
		}

		static constexpr auto ThrowFormatException = []() -> Color
			{
				throw winrt::hresult_invalid_argument(L"The parameter \"colorString\" is not a recognized Color format.");
			};

		if (colorView[0] == L'#')
		{
			switch (colorView.size())
			{
			case 9:
			{
				auto cuint = to_uint32(colorView.substr(1), 16);
				auto a = (uint8_t)(cuint >> 24);
				auto r = (uint8_t)((cuint >> 16) & 0xff);
				auto g = (uint8_t)((cuint >> 8) & 0xff);
				auto b = (uint8_t)(cuint & 0xff);

				return Windows::UI::ColorHelper::FromArgb(a, r, g, b);
			}

			case 7:
			{
				auto cuint = to_uint32(colorView.substr(1), 16);
				auto r = (uint8_t)((cuint >> 16) & 0xff);
				auto g = (uint8_t)((cuint >> 8) & 0xff);
				auto b = (uint8_t)(cuint & 0xff);

				return Windows::UI::ColorHelper::FromArgb(255, r, g, b);
			}

			case 5:
			{
				auto cuint = to_uint16(colorView.substr(1), 16);
				auto a = (uint8_t)(cuint >> 12);
				auto r = (uint8_t)((cuint >> 8) & 0xf);
				auto g = (uint8_t)((cuint >> 4) & 0xf);
				auto b = (uint8_t)(cuint & 0xf);
				a = (uint8_t)(a << 4 | a);
				r = (uint8_t)(r << 4 | r);
				g = (uint8_t)(g << 4 | g);
				b = (uint8_t)(b << 4 | b);

				return Windows::UI::ColorHelper::FromArgb(a, r, g, b);
			}

			case 4:
			{
				auto cuint = to_uint16(colorView.substr(1), 16);
				auto r = (uint8_t)((cuint >> 8) & 0xf);
				auto g = (uint8_t)((cuint >> 4) & 0xf);
				auto b = (uint8_t)(cuint & 0xf);
				r = (uint8_t)(r << 4 | r);
				g = (uint8_t)(g << 4 | g);
				b = (uint8_t)(b << 4 | b);

				return Windows::UI::ColorHelper::FromArgb(255, r, g, b);
			}

			default: return ThrowFormatException();
			}
		}

		if (colorView.size() > 3 && colorView[0] == L's' && colorView[1] == L'c' && colorView[2] == L'#')
		{
			auto values = split(colorView, L","sv) | std::ranges::to<std::vector>();

			if (values.size() == 4)
			{
				auto scA = to_double(values[0].substr(3));
				auto scR = to_double(values[1]);
				auto scG = to_double(values[2]);
				auto scB = to_double(values[3]);

				return Windows::UI::ColorHelper::FromArgb((uint8_t)(scA * 255), (uint8_t)(scR * 255), (uint8_t)(scG * 255), (uint8_t)(scB * 255));
			}

			if (values.size() == 3)
			{
				auto scR = to_double(values[0].substr(3));
				auto scG = to_double(values[1]);
				auto scB = to_double(values[2]);

				return Windows::UI::ColorHelper::FromArgb(255, (uint8_t)(scR * 255), (uint8_t)(scG * 255), (uint8_t)(scB * 255));
			}

			return ThrowFormatException();
		}

		return ThrowFormatException();
	}

	Color ColorHelper::ToColor(std::string_view colorString)
	{
		using namespace std::string_view_literals;

		if (colorString.empty())
		{
			throw winrt::hresult_invalid_argument(L"The parameter \"colorString\" must not be null or empty.");
		}

		static constexpr auto ThrowFormatException = []() -> Color
			{
				throw winrt::hresult_invalid_argument(L"The parameter \"colorString\" is not a recognized Color format.");
			};

		if (colorString[0] == '#')
		{
			switch (colorString.size())
			{
			case 9:
			{
				auto cuint = to_uint32(colorString.substr(1), 16);
				auto a = (uint8_t)(cuint >> 24);
				auto r = (uint8_t)((cuint >> 16) & 0xff);
				auto g = (uint8_t)((cuint >> 8) & 0xff);
				auto b = (uint8_t)(cuint & 0xff);

				return Windows::UI::ColorHelper::FromArgb(a, r, g, b);
			}

			case 7:
			{
				auto cuint = to_uint32(colorString.substr(1), 16);
				auto r = (uint8_t)((cuint >> 16) & 0xff);
				auto g = (uint8_t)((cuint >> 8) & 0xff);
				auto b = (uint8_t)(cuint & 0xff);

				return Windows::UI::ColorHelper::FromArgb(255, r, g, b);
			}

			case 5:
			{
				auto cuint = to_uint16(colorString.substr(1), 16);
				auto a = (uint8_t)(cuint >> 12);
				auto r = (uint8_t)((cuint >> 8) & 0xf);
				auto g = (uint8_t)((cuint >> 4) & 0xf);
				auto b = (uint8_t)(cuint & 0xf);
				a = (uint8_t)(a << 4 | a);
				r = (uint8_t)(r << 4 | r);
				g = (uint8_t)(g << 4 | g);
				b = (uint8_t)(b << 4 | b);

				return Windows::UI::ColorHelper::FromArgb(a, r, g, b);
			}

			case 4:
			{
				auto cuint = to_uint16(colorString.substr(1), 16);
				auto r = (uint8_t)((cuint >> 8) & 0xf);
				auto g = (uint8_t)((cuint >> 4) & 0xf);
				auto b = (uint8_t)(cuint & 0xf);
				r = (uint8_t)(r << 4 | r);
				g = (uint8_t)(g << 4 | g);
				b = (uint8_t)(b << 4 | b);

				return Windows::UI::ColorHelper::FromArgb(255, r, g, b);
			}

			default: return ThrowFormatException();
			}
		}

		if (colorString.size() > 3 && colorString[0] == 's' && colorString[1] == 'c' && colorString[2] == '#')
		{
			auto values = split(colorString, ","sv) | std::ranges::to<std::vector>();

			if (values.size() == 4)
			{
				auto scA = to_double(values[0].substr(3));
				auto scR = to_double(values[1]);
				auto scG = to_double(values[2]);
				auto scB = to_double(values[3]);

				return Windows::UI::ColorHelper::FromArgb((uint8_t)(scA * 255), (uint8_t)(scR * 255), (uint8_t)(scG * 255), (uint8_t)(scB * 255));
			}

			if (values.size() == 3)
			{
				auto scR = to_double(values[0].substr(3));
				auto scG = to_double(values[1]);
				auto scB = to_double(values[2]);

				return Windows::UI::ColorHelper::FromArgb(255, (uint8_t)(scR * 255), (uint8_t)(scG * 255), (uint8_t)(scB * 255));
			}

			return ThrowFormatException();
		}

		return ThrowFormatException();
	}

	winrt::hstring ColorHelper::ToHex(Color const& color)
	{
		return winrt::format(L"#{:02X}{:02X}{:02X}{:02X}", color.A, color.R, color.G, color.B);
	}

	int ColorHelper::ToInt(Color const& color)
	{
		auto a = color.A + 1;
		auto col = (color.A << 24) | ((byte)((color.R * a) >> 8) << 16) | ((byte)((color.G * a) >> 8) << 8) | (byte)((color.B * a) >> 8);
		return col;
	}

	HslColor ColorHelper::ToHsl(Color const& color)
	{
		constexpr double toDouble = 1.0 / 255.0;
		const double r = toDouble * color.R;
		const double g = toDouble * color.G;
		const double b = toDouble * color.B;

		const double max = std::max({ r, g, b });
		const double min = std::min({ r, g, b });
		const double chroma = max - min;

		double h1 = 0.0;

		if (chroma == 0.0)
		{
			h1 = 0.0;
		}
		else if (max == r)
		{
			// ((g - b) / chroma + 6) % 6
			h1 = std::fmod(((g - b) / chroma) + 6.0, 6.0);
		}
		else if (max == g)
		{
			h1 = 2.0 + ((b - r) / chroma);
		}
		else
		{
			h1 = 4.0 + ((r - g) / chroma);
		}

		const double lightness = 0.5 * (max + min);
		const double saturation = (chroma == 0.0)
			? 0.0
			: chroma / (1.0 - std::abs((2.0 * lightness) - 1.0));

		return HslColor
		{
			.H = 60.0 * h1,
			.S = saturation,
			.L = lightness,
			.A = toDouble * color.A,
		};
	}

	HsvColor ColorHelper::ToHsv(Color const& color)
	{
		constexpr double toDouble = 1.0 / 255;
		double r = toDouble * color.R;
		double g = toDouble * color.G;
		double b = toDouble * color.B;

		auto max = std::max(std::max(r, g), b);
		auto min = std::min(std::min(r, g), b);

		double chroma = max - min;

		double h1;
		if (chroma == 0.0)
		{
			h1 = 0.0;
		}
		else if (max == r)
		{
			h1 = std::fmod(((g - b) / chroma) + 6.0, 6.0);
		}
		else if (max == g)
		{
			h1 = 2.0 + ((b - r) / chroma);
		}
		else
		{
			h1 = 4.0 + ((r - g) / chroma);
		}

		double saturation = (chroma == 0.0) ? 0.0 : chroma / max;

		HsvColor ret;
		ret.H = 60.0 * h1;
		ret.S = saturation;
		ret.V = max;
		ret.A = toDouble * color.A;
		return ret;
	}

	Color ColorHelper::FromHsl(double hue, double saturation, double lightness, double alpha)
	{
		if (hue < 0.0 || hue > 360.0)
		{
			throw winrt::hresult_invalid_argument(L"hue must be between 0 and 360");
		}

		const double chroma = (1.0 - std::abs((2.0 * lightness) - 1.0)) * saturation;
		const double h1 = hue / 60.0;
		const double x = chroma * (1.0 - std::abs(std::fmod(h1, 2.0) - 1.0));
		const double m = lightness - (0.5 * chroma);

		double r1 = 0.0, g1 = 0.0, b1 = 0.0;

		if (h1 < 1.0)
		{
			r1 = chroma;
			g1 = x;
		}
		else if (h1 < 2.0)
		{
			r1 = x;
			g1 = chroma;
		}
		else if (h1 < 3.0)
		{
			g1 = chroma;
			b1 = x;
		}
		else if (h1 < 4.0)
		{
			g1 = x;
			b1 = chroma;
		}
		else if (h1 < 5.0)
		{
			r1 = x;
			b1 = chroma;
		}
		else
		{
			r1 = chroma;
			b1 = x;
		}

		const auto to_byte = [](double v) -> uint8_t {
			v = std::clamp(v, 0.0, 1.0);
			return static_cast<uint8_t>(std::round(v * 255.0));
			};

		return Color
		{
			.A = to_byte(alpha),
			.R = to_byte(r1 + m),
			.G = to_byte(g1 + m),
			.B = to_byte(b1 + m),
		};
	}

	Color ColorHelper::FromHsv(double hue, double saturation, double value, double alpha)
	{
		if (hue < 0.0 || hue > 360.0)
		{
			throw winrt::hresult_invalid_argument(L"hue must be in [0, 360]");
		}

		double chroma = value * saturation;
		double h1 = hue / 60.0;
		double x = chroma * (1.0 - std::abs(std::fmod(h1, 2.0) - 1.0));
		double m = value - chroma;

		double r1, g1, b1;

		if (h1 < 1.0)
		{
			r1 = chroma; g1 = x; b1 = 0.0;
		}
		else if (h1 < 2.0)
		{
			r1 = x; g1 = chroma; b1 = 0.0;
		}
		else if (h1 < 3.0)
		{
			r1 = 0.0; g1 = chroma; b1 = x;
		}
		else if (h1 < 4.0)
		{
			r1 = 0.0; g1 = x; b1 = chroma;
		}
		else if (h1 < 5.0)
		{
			r1 = x; g1 = 0.0; b1 = chroma;
		}
		else
		{
			r1 = chroma; g1 = 0.0; b1 = x;
		}

		uint8_t r = static_cast<uint8_t>(std::round(255.0 * (r1 + m)));
		uint8_t g = static_cast<uint8_t>(std::round(255.0 * (g1 + m)));
		uint8_t b = static_cast<uint8_t>(std::round(255.0 * (b1 + m)));
		uint8_t a = static_cast<uint8_t>(std::round(255.0 * alpha));

		return winrt::Windows::UI::ColorHelper::FromArgb(a, r, g, b);
	}
}
