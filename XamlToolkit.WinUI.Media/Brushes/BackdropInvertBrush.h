// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "BackdropInvertBrush.g.h"
#include "Base/XamlCompositionEffectBrushBase.h"

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// The <see cref="BackdropInvertBrush"/> is a <see cref="Brush"/> which inverts whatever is behind it in the application.
    /// </summary>
    struct BackdropInvertBrush : BackdropInvertBrushT<BackdropInvertBrush, XamlCompositionEffectBrushBase>
    {
        BackdropInvertBrush() = default;

        Pipelines::PipelineBuilder OnPipelineRequested() override;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct BackdropInvertBrush : BackdropInvertBrushT<BackdropInvertBrush, implementation::BackdropInvertBrush>
    {
    };
}