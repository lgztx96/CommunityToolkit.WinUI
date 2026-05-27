#pragma once

#ifdef __INTELLISENSE__
#include <cmath>
#include <sstream>
#include <string>
#endif

#include <iomanip>

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
        static std::wstring ToCompositionString(float number)
        {
            std::wostringstream ss;
            ss << std::fixed << number;

            std::wstring result = ss.str();

            // Check if scientific notation would be used
            double absNumber = std::abs(number);
            if (absNumber >= 1e6 || (absNumber < 1e-4 && absNumber > 0))
            {
                // For very large or very small numbers, use fixed notation
                ss.str(L"");
                ss.clear();
                if (absNumber >= 1e6)
                {
                    ss << std::fixed << std::setprecision(0) << number;
                }
                else
                {
                    // For very small numbers, use enough decimal places
                    int decimalPlaces = static_cast<int>(-std::log10(absNumber)) + 6;
                    ss << std::fixed << std::setprecision(decimalPlaces) << number;
                }
                result = ss.str();
            }

            // Remove trailing zeros after decimal point
            size_t dotPos = result.find(L'.');
            if (dotPos != std::wstring::npos)
            {
                size_t lastNonZero = result.find_last_not_of(L'0');
                if (lastNonZero != std::wstring::npos && lastNonZero > dotPos)
                {
                    result = result.substr(0, lastNonZero + 1);
                }
                // Remove trailing decimal point if no decimals left
                if (result.back() == L'.')
                {
                    result.pop_back();
                }
            }

            return result;
        }
    };
}
