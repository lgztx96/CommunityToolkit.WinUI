// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "TileSourceExtension.g.h"
#include "../Abstract/ImageSourceBaseExtension.h"
#include "../Pipelines/PipelineBuilder.h"

import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An effect that loads an image and replicates it to cover all the available surface area
    /// </summary>
    /// <remarks>This effect maps to the Win2D BorderEffect effect</remarks>
    struct TileSourceExtension : TileSourceExtensionT<TileSourceExtension, ImageSourceBaseExtension>
    {
        TileSourceExtension() = default;

        winrt::Windows::Foundation::IInspectable ProvideValue() const
        {
            const auto& uri = Uri();
            if (!uri)
            {
                throw winrt::hresult_invalid_argument(L"Uri cannot be null");
            }
            return Pipelines::PipelineBuilder::FromTiles(uri, DpiMode(), CacheMode());
        }

        winrt::Windows::Foundation::IInspectable ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider) const
        {
            return ProvideValue();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct TileSourceExtension : TileSourceExtensionT<TileSourceExtension, implementation::TileSourceExtension>
    {
    };
}