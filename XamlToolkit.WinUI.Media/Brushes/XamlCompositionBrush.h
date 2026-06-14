// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "XamlCompositionBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"
#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

import winrt.XamlToolkit.WinUI.Media;
import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A simple class that can be used to quickly create XAML brushes from arbitrary <see cref="PipelineBuilder"/> pipelines
    /// </summary>
    struct XamlCompositionBrush : XamlCompositionBrushT<XamlCompositionBrush, XamlCompositionEffectBrushBase>
    {
        XamlCompositionBrush() = default;
        XamlCompositionBrush(Pipelines::PipelineBuilder const& pipeline);

        wil::single_threaded_property<Pipelines::PipelineBuilder> Pipeline;

        Pipelines::PipelineBuilder OnPipelineRequested() override;

        winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush Clone();
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct XamlCompositionBrush : XamlCompositionBrushT<XamlCompositionBrush, implementation::XamlCompositionBrush>
    {
    };
}