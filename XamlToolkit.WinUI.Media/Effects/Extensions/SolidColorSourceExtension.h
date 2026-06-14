// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "SolidColorSourceExtension.g.h"
#include "../Pipelines/PipelineBuilder.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An effect that renders a standard 8bit SDR color on the available surface
    /// </summary>
    struct SolidColorSourceExtension : SolidColorSourceExtensionT<SolidColorSourceExtension>
    {
        SolidColorSourceExtension() = default;

        /// <summary>
        /// Gets or sets the color to display
        /// </summary>
        wil::single_threaded_rw_property<winrt::Windows::UI::Color> Color;

        /// <inheritdoc/>
        winrt::Windows::Foundation::IInspectable ProvideValue() const
        {
            return Pipelines::PipelineBuilder::FromColor(Color());
        }

        winrt::Windows::Foundation::IInspectable ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider) const
        {
			return ProvideValue();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct SolidColorSourceExtension : SolidColorSourceExtensionT<SolidColorSourceExtension, implementation::SolidColorSourceExtension>
    {
    };
}