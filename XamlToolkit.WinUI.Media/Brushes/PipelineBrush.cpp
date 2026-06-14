// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineBrush.h"
#if __has_include("PipelineBrush.g.cpp")
#include "PipelineBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> PipelineBrush::EffectsProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Effects",
            winrt::xaml_typename<winrt::Windows::Foundation::Collections::IVector<IPipelineEffect>>(),
            winrt::xaml_typename<class_type>(),
            nullptr);

    winrt::Windows::Foundation::Collections::IVector<IPipelineEffect> PipelineBrush::Effects() const
    {
        auto effects = GetValue(EffectsProperty())
            .try_as<winrt::Windows::Foundation::Collections::IVector<IPipelineEffect>>();

        if (!effects)
        {
            effects = winrt::single_threaded_vector<IPipelineEffect>();
            SetValue(EffectsProperty(), effects);
        }
        return effects;
    }

    void PipelineBrush::Effects(winrt::Windows::Foundation::Collections::IVector<IPipelineEffect> const& value)
    {
        SetValue(EffectsProperty(), value);
    }

    Pipelines::PipelineBuilder PipelineBrush::OnPipelineRequested()
    {
        auto builder = Source() ? Source() : Pipelines::PipelineBuilder::FromBackdrop();

        for (const auto& effect : Effects())
        {
            builder = effect.AppendToBuilder(builder);
        }

        return builder;
    }

    void PipelineBrush::OnCompositionBrushUpdated()
    {
        for (const auto& effect : Effects())
        {
            effect.NotifyCompositionBrushInUse(CompositionBrush());
        }
    }
}