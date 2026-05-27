// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "EmptyCollectionToObjectConverter.h"
#if __has_include("EmptyCollectionToObjectConverter.g.cpp")
#include "EmptyCollectionToObjectConverter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Converters::implementation
{
    bool EmptyCollectionToObjectConverter::CheckValueIsEmpty(winrt::IInspectable const& value) const
    {
        bool isEmpty = true;

        if (value)
        {
            if (auto iterable = value.try_as<winrt::Windows::Foundation::Collections::IIterable<winrt::IInspectable>>())
            {
                auto iterator = iterable.First();
                isEmpty = !iterator.HasCurrent();
            }
        }

        return isEmpty;
    }
}