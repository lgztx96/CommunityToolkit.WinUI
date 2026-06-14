// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "PipelineBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.Windows.Foundation.Collections;
import winrt.Microsoft.UI.Xaml;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A <see cref="Brush"/> that renders a customizable Composition/Win2D effects pipeline
    /// </summary>
    struct PipelineBrush : PipelineBrushT<PipelineBrush, XamlCompositionEffectBrushBase>
    {
        PipelineBrush() = default;

        wil::single_threaded_rw_property<Pipelines::PipelineBuilder> Source{ nullptr };

        winrt::Windows::Foundation::Collections::IVector<IPipelineEffect> Effects() const;
        void Effects(winrt::Windows::Foundation::Collections::IVector<IPipelineEffect> const& value);

        static const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> EffectsProperty;

        Pipelines::PipelineBuilder OnPipelineRequested() override;
        void OnCompositionBrushUpdated() override;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct PipelineBrush : PipelineBrushT<PipelineBrush, implementation::PipelineBrush>
    {
    };
}