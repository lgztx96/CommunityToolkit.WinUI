// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "ShadeEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <winrt/Windows.UI.h>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import std;
import winrt.Windows.UI;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An effect that overlays a color layer over the current builder, with a specified intensity
    /// </summary>
    struct ShadeEffect : ShadeEffectT<ShadeEffect, implementation::PipelineEffect>
    {
        ShadeEffect() = default;

        /// <summary>
        /// Gets or sets the color to use
        /// </summary>
        wil::single_threaded_rw_property<winrt::Windows::UI::Color> Color;

        /// <summary>
        /// Gets or sets the intensity of the color layer (default to 0.5, should be in the [0, 1] range)
        /// </summary>
		double Intensity() const noexcept
        {
            return _intensity;
        }

        /// <summary>
        /// Gets or sets the intensity of the color layer (default to 0.5, should be in the [0, 1] range)
        /// </summary>
        void Intensity(double value)
        {
            _intensity = std::clamp(value, 0.0, 1.0);
        }

        /// <inheritdoc/>
        Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
        {
            return builder.Shade(Color(), static_cast<float>(Intensity()));
        }

    private:
        double _intensity{ 0.0 };
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct ShadeEffect : ShadeEffectT<ShadeEffect, implementation::ShadeEffect>
    {
    };
}