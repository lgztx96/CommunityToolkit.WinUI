#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <ranges>
#include <string>
#include <string_view>
#endif
#include "DoubleVector.h"
#if __has_include("DoubleVector.g.cpp")
#include "DoubleVector.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    DoubleVector::DoubleVector(winrt::Windows::Foundation::Collections::IIterable<double> const& values)
    {
        for (double v : values)
        {
            _values.push_back(v);
        }
    }

    winrt::XamlToolkit::Labs::WinUI::DoubleVector DoubleVector::CreateFromString(winrt::hstring const& value)
    {
        std::vector<double> doubles;

        for (const auto part : value | std::views::split(','))
        {
            std::wstring_view token(part);

            const auto start = token.find_first_not_of(L" \t\r\n");
            if (start == std::wstring_view::npos)
            {
                continue;
            }

            token.remove_prefix(start);

            const auto end = token.find_last_not_of(L" \t\r\n");
            token.remove_suffix(token.size() - end - 1);

            doubles.push_back(std::stod(std::wstring{ token }));
        }

        return winrt::make<DoubleVector>(std::move(doubles));
    }
}
