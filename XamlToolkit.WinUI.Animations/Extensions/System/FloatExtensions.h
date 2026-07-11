#pragma once

#ifdef __INTELLISENSE__
#include <array>
#include <charconv>
#include <format>
#include <string>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An extension class for the <see cref="float"/> type
    /// </summary>
    class FloatExtensions
    {
    public:
        /// <summary>
        /// Returns a <see cref="string"/> representation of a <see cref="float"/> that avoids scientific notation, which is not compatible with the composition expression animations API
        /// </summary>
        /// <param name="number">The input <see cref="float"/> to process</param>
        /// <returns>A <see cref="string"/> representation of <paramref name="number"/> that can be used in a expression animation</returns>
        static winrt::hstring ToCompositionString(float number)
        {
            std::array<char, 64> buf;

            auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), number, std::chars_format::fixed);

            if (ec != std::errc{})
            {
                throw std::runtime_error(std::format("std::to_chars failed for value {}", number));
            }

            return winrt::to_hstring(std::string_view(buf.data(), ptr - buf.data()));
        }
    };
}
