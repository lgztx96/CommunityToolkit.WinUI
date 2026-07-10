#pragma once

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#include <format>
#include <string>
#endif

namespace winrt::XamlToolkit::WinUI::Animations
{
    /// <summary>
    /// An extension class for the <see cref="Guid"/> type
    /// </summary>
    class GuidExtensions
    {
    public:
        /// <summary>
        /// Returns a <see cref="string"/> representation of a <see cref="Guid"/> only made of uppercase letters
        /// </summary>
        /// <param name="guid">The input <see cref="Guid"/> to process</param>
        /// <returns>A <see cref="string"/> representation of <paramref name="guid"/> only made up of letters in the [A-Z] range</returns>
        static std::wstring ToUppercaseAsciiLetters(winrt::guid const& guid)
        {
            // Composition IDs must only be composed of characters in the [A-Z0-9_] set,
            // and also have the restriction that the initial character cannot be a digit.
            // Because of this, we need to prepend an underscore to a serialized guid to
            // avoid cases where the first character is a digit.
            return std::format(
                L"_{:08X}{:04X}{:04X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                guid.Data1,
                guid.Data2,
                guid.Data3,
                guid.Data4[0], guid.Data4[1],
                guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5],
                guid.Data4[6], guid.Data4[7]);
        }
    };
}
