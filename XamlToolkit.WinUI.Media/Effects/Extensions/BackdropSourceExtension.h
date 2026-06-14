// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropSourceExtension.g.h"
#include "../Pipelines/PipelineBuilder.h"

import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A backdrop effect that can sample from a specified source
    /// </summary>
    struct BackdropSourceExtension : BackdropSourceExtensionT<BackdropSourceExtension>
    {
        BackdropSourceExtension() = default;

        winrt::Windows::Foundation::IInspectable ProvideValue() const
        {
            return Pipelines::PipelineBuilder::FromBackdrop();
        }

        winrt::Windows::Foundation::IInspectable ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider) const
        {
            return ProvideValue();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropSourceExtension : BackdropSourceExtensionT<BackdropSourceExtension, implementation::BackdropSourceExtension>
    {
    };
}