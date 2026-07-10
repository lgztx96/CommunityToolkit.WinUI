// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BlendEffect.g.h"
#include "Abstract/PipelineEffect.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.Graphics.Canvas.Effects.h>
#include <winrt/XamlToolkit.WinUI.Media.h>
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation.Collections;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.Graphics.Canvas.Effects;
import winrt.XamlToolkit.WinUI.Media;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::Graphics::Canvas::Effects;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A blend effect that merges the current builder with an input one
    /// </summary>
    /// <remarks>This effect maps to the Win2D BlendEffect effect</remarks>
    struct BlendEffect : BlendEffectT<BlendEffect, implementation::PipelineEffect>
    {
        BlendEffect() = default;

        wil::single_threaded_rw_property<Pipelines::PipelineBuilder> Source{ nullptr };

        /// <summary>
        /// Gets or sets the effects to apply to the input to merge with the current instance
        /// </summary>
        wil::single_threaded_rw_property<winrt::IVector<IPipelineEffect>> Effects = winrt::single_threaded_vector<IPipelineEffect>();

        /// <summary>
        /// Gets or sets the blending mode to use (the default mode is Multiply)
        /// </summary>
        wil::single_threaded_rw_property<ImageBlendMode> Mode;

        /// <summary>
        /// Gets or sets the placement of the input builder with respect to the current one (the default is Foreground)
        /// </summary>
        wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Media::Placement> Placement = Placement::Foreground;

        Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
        {
            // Get or create source pipeline
            auto sourcePipeline = Source();
            if (!sourcePipeline)
            {
                sourcePipeline = Pipelines::PipelineBuilder::FromBackdrop();
            }

            // Apply effects
            for (const auto& effect : Effects())
            {
                sourcePipeline = effect.AppendToBuilder(sourcePipeline);
            }

            return builder.Blend(sourcePipeline, static_cast<winrt::BlendEffectMode>(Mode()), Placement());
        }

        void NotifyCompositionBrushInUse(winrt::CompositionBrush const& brush) override
        {
            PipelineEffect::NotifyCompositionBrushInUse(brush);

            for (const auto& effect : Effects())
            {
                effect.NotifyCompositionBrushInUse(brush);
            }
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BlendEffect : BlendEffectT<BlendEffect, implementation::BlendEffect>
    {
    };
}