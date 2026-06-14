// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once
#include "PipelineEffect.g.h"
#include "../Pipelines/PipelineBuilder.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A base pipeline effect.
    /// </summary>
    struct PipelineEffect : PipelineEffectT<PipelineEffect>
    {
        PipelineEffect() = default;

        /// <summary>
        /// Gets the current CompositionBrush instance, if one is in use.
        /// </summary>
        wil::single_threaded_rw_property<winrt::Microsoft::UI::Composition::CompositionBrush> Brush{ nullptr };

        /// <summary>
        /// Gets or sets a value indicating whether the effect can be animated.
        /// </summary>
        wil::single_threaded_rw_property<bool> IsAnimatable;

        /// <inheritdoc/>
        virtual Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder)
        {
            return builder;
        }

        virtual void NotifyCompositionBrushInUse(winrt::Microsoft::UI::Composition::CompositionBrush const& brush)
        {
            Brush = brush;
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct PipelineEffect : PipelineEffectT<PipelineEffect, implementation::PipelineEffect>
    {
    };
}