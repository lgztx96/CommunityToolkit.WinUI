// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "UIElementExtensions.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Xaml.Hosting;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Numerics;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    struct UIElementExtensions
    {
        static winrt::fire_and_forget OnVisualFactoryPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static AttachedVisualFactoryBase GetVisualFactory(winrt::UIElement const& element);

        static void SetVisualFactory(winrt::UIElement const& element, AttachedVisualFactoryBase const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> VisualFactoryProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct UIElementExtensions : UIElementExtensionsT<UIElementExtensions, implementation::UIElementExtensions>
    {
    };
}
