// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "XamlCompositionEffectBrushBase.h"
#if __has_include("XamlCompositionEffectBrushBase.g.cpp")
#include "XamlCompositionEffectBrushBase.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    void XamlCompositionEffectBrushBase::IsEnabled(bool value)
    {
        OnEnabledToggled(value);
    }

    winrt::fire_and_forget XamlCompositionEffectBrushBase::OnConnected()
    {
        auto lock = co_await _connectedMutex.lock_async();

        if (CompositionBrush() == nullptr)
        {

            winrt::Microsoft::UI::Composition::CompositionCapabilities compositionCapabilities;

            // Abort if effects aren't supported.
            if (!compositionCapabilities.AreEffectsSupported())
            {
                co_return;
            }

            if (_isEnabled)
            {
                if (auto builder = OnPipelineRequested())
                {
                    CompositionBrush(co_await builder.BuildAsync());
                }
            }
            else
            {
				auto builder = co_await Pipelines::PipelineBuilder::FromColor(FallbackColor()).BuildAsync();
                CompositionBrush(builder);
            }

            OnCompositionBrushUpdated();
        }

		base_type::OnConnected();
    }

    void XamlCompositionEffectBrushBase::OnDisconnected()
    {
        if (auto brush = CompositionBrush())
        {
            brush.Close();
            CompositionBrush(nullptr);
            OnCompositionBrushUpdated();
        }

		base_type::OnDisconnected();
    }

    winrt::fire_and_forget XamlCompositionEffectBrushBase::OnEnabledToggled(bool value)
    {
        auto lock = co_await _connectedMutex.lock_async();

        if (_isEnabled == value)
        {
            co_return;
        }

        _isEnabled = value;

        if (CompositionBrush())
        {
            winrt::Microsoft::UI::Composition::CompositionCapabilities compositionCapabilities;

            // Abort if effects aren't supported.
            if (!compositionCapabilities.AreEffectsSupported())
            {
                co_return;
            }

            if (_isEnabled)
            {
                if (auto builder = OnPipelineRequested())
                {
                    CompositionBrush(co_await builder.BuildAsync());
                }
            }
            else
            {
                auto builder = co_await Pipelines::PipelineBuilder::FromColor(FallbackColor()).BuildAsync();
                CompositionBrush(builder);
            }

            OnCompositionBrushUpdated();
        }
    }
}