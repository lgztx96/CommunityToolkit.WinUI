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
        for (auto const& v : values)
        {
            _values.emplace_back(v);
        }
    }

    winrt::XamlToolkit::Labs::WinUI::DoubleVector DoubleVector::CreateFromString(winrt::hstring const& value)
    {
        using namespace std::string_view_literals;

        std::vector<double> doubles;

        for (auto part : value | std::views::split(L","sv))
        {
            std::wstring token(part.begin(), part.end());

            token.erase(0, token.find_first_not_of(L" \t\r\n"));
            token.erase(token.find_last_not_of(L" \t\r\n") + 1);

            if (!token.empty())
            {
                double val = std::stod(token);
                doubles.push_back(val);
            }
        }

        return winrt::make<DoubleVector>(std::move(doubles));
    }
}
