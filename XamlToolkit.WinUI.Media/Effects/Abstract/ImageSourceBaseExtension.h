// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ImageSourceBaseExtension.g.h"
#include "../Pipelines/PipelineBuilder.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation;
import winrt.XamlToolkit.WinUI.Media;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An image based effect that loads an image at the specified location
    /// </summary>
    struct ImageSourceBaseExtension : ImageSourceBaseExtensionT<ImageSourceBaseExtension>
    {
        ImageSourceBaseExtension() = default;

        /// <summary>
        /// Gets or sets the Uri for the image to load
        /// </summary>
        wil::single_threaded_rw_property<winrt::Windows::Foundation::Uri> Uri{ nullptr };

        /// <summary>
        /// Gets or sets the DPI mode used to render the image (the default is DisplayDpiWith96AsLowerBound)
        /// </summary>
        wil::single_threaded_rw_property<Media::DpiMode> DpiMode;

        /// <summary>
        /// Gets or sets the cache mode to use when loading the image (the default is Default)
        /// </summary>
        wil::single_threaded_rw_property<Media::CacheMode> CacheMode;

        virtual winrt::Windows::Foundation::IInspectable ProvideValue();

        virtual winrt::Windows::Foundation::IInspectable ProvideValue(winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider);
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct ImageSourceBaseExtension : ImageSourceBaseExtensionT<ImageSourceBaseExtension, implementation::ImageSourceBaseExtension>
    {
    };
}