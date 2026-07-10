// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ImageSourceExtension.g.h"
#include "../Abstract/ImageSourceBaseExtension.h"
#include "../Pipelines/PipelineBuilder.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An image effect, which displays an image loaded as a Win2D surface
    /// </summary>
    struct ImageSourceExtension : ImageSourceExtensionT<ImageSourceExtension, ImageSourceBaseExtension>
    {
        ImageSourceExtension() = default;

        winrt::Windows::Foundation::IInspectable ProvideValue() const
        {
            auto uri = Uri();
            if (!uri)
            {
                throw winrt::hresult_invalid_argument(L"Uri cannot be null");
            }
            return Pipelines::PipelineBuilder::FromImage(uri, DpiMode(), CacheMode());
        }

        winrt::Windows::Foundation::IInspectable ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider) const
        {
            return ProvideValue();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct ImageSourceExtension : ImageSourceExtensionT<ImageSourceExtension, implementation::ImageSourceExtension>
    {
    };
}