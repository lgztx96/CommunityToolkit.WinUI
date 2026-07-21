// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "PipelineVisualFactoryBase.g.h"
#include "AttachedVisualFactoryBase.h"
#include "../Pipelines/PipelineBuilder.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Composition.h>

#else
import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Composition;
import winrt.XamlToolkit.WinUI.Media.Pipelines;
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A base class that extends <see cref="AttachedVisualFactoryBase"/> by leveraging the <see cref="PipelineBuilder"/> APIs.
    /// </summary>
    struct PipelineVisualFactoryBase : PipelineVisualFactoryBaseT<PipelineVisualFactoryBase, AttachedVisualFactoryBase>
    {
        PipelineVisualFactoryBase() = default;

        /// <summary>
        /// Creates a <see cref="Visual"/> to attach to the target element.
        /// </summary>
        winrt::IAsyncOperation<winrt::Visual> GetAttachedVisualAsync(winrt::UIElement const& element) override
        {
            auto visual = winrt::ElementCompositionPreview::GetElementVisual(element).Compositor().CreateSpriteVisual();
            auto brush = co_await OnPipelineRequested().BuildAsync();
            visual.Brush(brush);
            co_return visual;
        }

        /// <summary>
        /// A method that builds and returns the <see cref="PipelineBuilder"/> pipeline to use in the current instance.
        /// </summary>
        virtual Pipelines::PipelineBuilder OnPipelineRequested() { return nullptr; };
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct PipelineVisualFactoryBase : PipelineVisualFactoryBaseT<PipelineVisualFactoryBase, implementation::PipelineVisualFactoryBase>
    {
    };
}