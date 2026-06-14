// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "UIElementExtensions.g.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    struct UIElementExtensions
    {
    public:
        static winrt::fire_and_forget OnVisualFactoryPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static AttachedVisualFactoryBase GetVisualFactory(UIElement const& element);

        static void SetVisualFactory(UIElement const& element, AttachedVisualFactoryBase const& value);

        static const wil::single_threaded_property<DependencyProperty> VisualFactoryProperty;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct UIElementExtensions : UIElementExtensionsT<UIElementExtensions, implementation::UIElementExtensions>
    {
    };
}
