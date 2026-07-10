// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "XamlCompositionEffectBrushBase.g.h"
#include "../../Pipelines/PipelineBuilder.h"
#include "../XamlToolkit.WinUI/common.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.h>
#else
import std;
import winrt.Microsoft.UI.Composition;
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A custom <see cref="XamlCompositionBrushBase"/> that's ready to be used with a custom <see cref="PipelineBuilder"/> pipeline.
    /// </summary>
    struct XamlCompositionEffectBrushBase : XamlCompositionEffectBrushBaseT<XamlCompositionEffectBrushBase>
    {
        XamlCompositionEffectBrushBase() = default;

        bool IsEnabled() const noexcept { return _isEnabled; }
        void IsEnabled(bool value);

        /// <summary>
        /// A method that builds and returns the <see cref="PipelineBuilder"/> pipeline to use in the current instance.
        /// </summary>
        virtual Pipelines::PipelineBuilder OnPipelineRequested() { return nullptr; }

        /// <summary>
        /// Updates the <see cref="XamlCompositionBrushBase.CompositionBrush"/> property depending on the input value.
        /// </summary>
        /// <param name="value">The new value being set to the <see cref="IsEnabled"/> property.</param>
        winrt::fire_and_forget OnEnabledToggled(bool value);

        /// <summary>
        /// Invoked whenever the <see cref="CompositionBrush"/> property is updated.
        /// </summary>
        virtual void OnCompositionBrushUpdated() {}

        virtual winrt::fire_and_forget OnConnected();
        virtual void OnDisconnected();

    private:
        bool _isEnabled{ true };
		winrt::async_mutex _connectedMutex;
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct XamlCompositionEffectBrushBase : XamlCompositionEffectBrushBaseT<XamlCompositionEffectBrushBase, implementation::XamlCompositionEffectBrushBase>
    {
    };
}