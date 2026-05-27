#pragma once

#include "DoubleVector.g.h"

#ifdef __INTELLISENSE__
#include <vector>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct DoubleVector : DoubleVectorT<DoubleVector, winrt::Windows::Foundation::Collections::IVectorView<double>>, winrt::vector_base<DoubleVector, double>
    {
        DoubleVector() = default;

        DoubleVector(winrt::Windows::Foundation::Collections::IIterable<double> const& values);

        DoubleVector(std::vector<double>&& values) : _values(values) { }

        auto& get_container() const noexcept
        {
            return _values;
        }

        auto& get_container() noexcept
        {
            return _values;
        }

        static winrt::XamlToolkit::Labs::WinUI::DoubleVector CreateFromString(winrt::hstring const& value);

    private:
        std::vector<double> _values;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct DoubleVector : DoubleVectorT<DoubleVector, implementation::DoubleVector>
    {
    };
}
