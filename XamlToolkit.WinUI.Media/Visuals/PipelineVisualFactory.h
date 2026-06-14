// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "PipelineVisualFactory.g.h"
#include "PipelineVisualFactoryBase.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A builder type for <see cref="SpriteVisual"/> instance to apply to UI elements.
    /// </summary>
    struct PipelineVisualFactory : PipelineVisualFactoryT<PipelineVisualFactory, PipelineVisualFactoryBase>
    {
        PipelineVisualFactory() = default;

        /// <summary>
        /// Gets or sets the source for the current pipeline.
        /// </summary>
        wil::single_threaded_rw_property<Pipelines::PipelineBuilder> Source{ nullptr };

        /// <summary>
        /// Gets or sets the collection of effects to use in the current pipeline.
        /// </summary>
        winrt::Windows::Foundation::Collections::IVector<IPipelineEffect > Effects() const
        {
            auto effects = GetValue(EffectsProperty()).try_as<winrt::Windows::Foundation::Collections::IVector<IPipelineEffect>>();
            if (!effects)
            {
				effects = winrt::single_threaded_vector<IPipelineEffect>();
                SetValue(EffectsProperty(), effects);
			}

            return effects;
		}

        void Effects(winrt::Windows::Foundation::Collections::IVector<IPipelineEffect> const& value)
        {
            SetValue(EffectsProperty(), value);
		}

		static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> EffectsProperty;

        /// <summary>
        /// Creates a <see cref="Visual"/> to attach to the target element.
        /// </summary>
        winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::Visual> GetAttachedVisualAsync(winrt::Microsoft::UI::Xaml::UIElement const& element) override
        {
            auto visual = co_await PipelineVisualFactoryBase::GetAttachedVisualAsync(element);

            auto spriteVisual = visual.as<winrt::Microsoft::UI::Composition::SpriteVisual>();

            for (const auto& effect : Effects())
            {
                effect.NotifyCompositionBrushInUse(spriteVisual.Brush());
            }

            co_return visual;
        }

        /// <summary>
        /// A method that builds and returns the <see cref="PipelineBuilder"/> pipeline to use in the current instance.
        /// </summary>
        Pipelines::PipelineBuilder OnPipelineRequested() override
        {
            auto builder = Source() ? Source() : Pipelines::PipelineBuilder::FromBackdrop();

            for (const auto& effect : Effects())
            {
                builder = effect.AppendToBuilder(builder);
            }

            return builder;
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct PipelineVisualFactory : PipelineVisualFactoryT<PipelineVisualFactory, implementation::PipelineVisualFactory>
    {
    };
}