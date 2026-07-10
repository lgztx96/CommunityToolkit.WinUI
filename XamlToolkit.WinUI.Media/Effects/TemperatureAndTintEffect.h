// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "TemperatureAndTintEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import std;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A temperature and tint effect
    /// </summary>
    /// <remarks>This effect maps to the Win2D TemperatureAndTintEffect effect</remarks>
    struct TemperatureAndTintEffect : TemperatureAndTintEffectT<TemperatureAndTintEffect, implementation::PipelineEffect>
    {
        TemperatureAndTintEffect() = default;

        /// <summary>
        /// Gets or sets the value of the temperature for the current effect (defaults to 0, should be in the [-1, 1] range)
        /// </summary>
        double Temperature() const noexcept
        {
			return _temperature;
        }
        /// <summary>
        /// Gets or sets the value of the temperature for the current effect (defaults to 0, should be in the [-1, 1] range)
        /// </summary>
        void Temperature(double value)
        {
            _temperature = std::clamp(value, -1.0, 1.0);
        }

        /// <summary>
        /// Gets or sets the value of the tint for the current effect (defaults to 0, should be in the [-1, 1] range)
        /// </summary>
        double Tint() const noexcept
        {
            return _tint;
        }
        /// <summary>
        /// Gets or sets the value of the tint for the current effect (defaults to 0, should be in the [-1, 1] range)
        /// </summary>
        void Tint(double value)
        {
            _tint = std::clamp(value, -1.0, 1.0);
        }

        /// <inheritdoc/>
        Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
        {
            return builder.TemperatureAndTint(static_cast<float>(Temperature()), static_cast<float>(Tint()));
        }

    private:
		double _temperature{ 0 };
		double _tint{ 0 };
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct TemperatureAndTintEffect : TemperatureAndTintEffectT<TemperatureAndTintEffect, implementation::TemperatureAndTintEffect>
    {
    };
}