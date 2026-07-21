// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "GrayscaleEffect.g.h"
#include "Abstract/PipelineEffect.h"

#ifdef __INTELLISENSE__
#include <winrt/XamlToolkit.WinUI.Media.Pipelines.h>
#else
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A grayscale effect
    /// </summary>
    /// <remarks>This effect maps to the Win2D GrayscaleEffect effect</remarks>
    struct GrayscaleEffect : GrayscaleEffectT<GrayscaleEffect, implementation::PipelineEffect>
    {
        GrayscaleEffect() = default;

        Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
        {
            return builder.Grayscale();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct GrayscaleEffect : GrayscaleEffectT<GrayscaleEffect, implementation::GrayscaleEffect>
    {
    };
}