// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "InvertEffect.g.h"
#include "Abstract/PipelineEffect.h"

import winrt.XamlToolkit.WinUI.Media.Pipelines;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// An color inversion effect
    /// </summary>
    /// <remarks>This effect maps to the Win2D InvertEffect effect</remarks>
    struct InvertEffect : InvertEffectT<InvertEffect, implementation::PipelineEffect>
    {
        InvertEffect() = default;

        Pipelines::PipelineBuilder AppendToBuilder(Pipelines::PipelineBuilder const& builder) override
        {
            return builder.Invert();
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct InvertEffect : InvertEffectT<InvertEffect, implementation::InvertEffect>
    {
    };
}