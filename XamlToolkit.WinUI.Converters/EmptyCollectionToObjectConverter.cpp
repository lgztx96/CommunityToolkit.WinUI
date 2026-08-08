// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "EmptyCollectionToObjectConverter.h"
#if __has_include("EmptyCollectionToObjectConverter.g.cpp")
#include "EmptyCollectionToObjectConverter.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#else
import winrt.Microsoft.UI.Xaml.Interop;
#endif

namespace winrt
{
    using namespace ::winrt::Windows::Foundation::Collections;
    using namespace ::winrt::Microsoft::UI::Xaml::Interop;
    using IBindableVector = ::winrt::Microsoft::UI::Xaml::Interop::IBindableVector;
    using IBindableVectorView = ::winrt::Microsoft::UI::Xaml::Interop::IBindableVectorView;
    using IBindableIterable = ::winrt::Microsoft::UI::Xaml::Interop::IBindableIterable;
}

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    bool EmptyCollectionToObjectConverter::CheckValueIsEmpty(winrt::IInspectable const& value) const
    {
        if (!value)
        {
            return true;
        }

        if (const auto vector = value.try_as<winrt::IVector<winrt::IInspectable>>())
        {
            return vector.Size() == 0;
        }
        else if (const auto vectorView = value.try_as<winrt::IVectorView<winrt::IInspectable>>())
        {
            return vectorView.Size() == 0;
        }
        else if (const auto iterable = value.try_as<winrt::IIterable<winrt::IInspectable>>())
        {
            return !iterable.First().HasCurrent();
        }
        else if (const auto bindableVector = value.try_as<winrt::IBindableVector>())
        {
            return bindableVector.Size() == 0;
        }
        else if (const auto bindableVectorView = value.try_as<winrt::IBindableVectorView>())
        {
            return bindableVectorView.Size() == 0;
        }
        else if (const auto bindableIterable = value.try_as<winrt::IBindableIterable>())
        {
            return !bindableIterable.First().HasCurrent();
        }

        throw winrt::hresult_invalid_argument(L"Argument 'value' is not a supported vector.");
    }
}