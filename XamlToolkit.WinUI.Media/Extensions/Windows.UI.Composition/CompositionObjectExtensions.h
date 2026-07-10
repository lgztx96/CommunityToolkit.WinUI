// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <functional>
#include <wil/resource.h>
#else
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Numerics;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
#endif

namespace winrt 
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Media::Extensions
{
    /// <summary>
    /// Extension methods for CompositionObject
    /// </summary>
    struct CompositionObjectExtensions
    {
        /// <summary>
        /// Starts an ExpressionAnimation to keep the size of the source Visual in sync with the target UIElement
        /// </summary>
        /// <param name="source">The Visual to start the animation on</param>
        /// <param name="target">The target UIElement to read the size updates from</param>
        static void BindSize(winrt::Visual const& source, winrt::UIElement const& target)
        {
            auto visual = winrt::ElementCompositionPreview::GetElementVisual(target);
            auto bindSizeAnimation = source.Compositor().CreateExpressionAnimation(L"visual.Size");

            bindSizeAnimation.SetReferenceParameter(L"visual", visual);

            // Start the animation
            source.StartAnimation(L"Size", bindSizeAnimation);
        }

        /// <summary>
        /// Starts an animation on the given property of a CompositionObject (scalar)
        /// </summary>
        static winrt::IAsyncAction StartAnimationAsync(winrt::CompositionObject const& target, winrt::hstring const& property, float value, winrt::TimeSpan const& duration)
        {
            // Stop previous animations
            target.StopAnimation(property);

            auto compositor = target.Compositor();
            // Setup the animation to run
            auto animation = compositor.CreateScalarKeyFrameAnimation();
            animation.InsertKeyFrame(1.0f, value);
            animation.Duration(duration);

            // Get the batch and start the animations
            auto batch = compositor.CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

            wil::shared_event completionEvent(wil::EventOptions::ManualReset);

            batch.Completed([completionEvent](auto&&, auto&&)
            {
				completionEvent.SetEvent();
            });

            target.StartAnimation(property, animation);

            batch.End();

            co_await winrt::resume_on_signal(completionEvent.get());
        }

        /// <summary>
        /// Starts an animation on the given property of a CompositionObject (Color)
        /// </summary>
        static winrt::IAsyncAction StartAnimationAsync(winrt::CompositionObject const& target, winrt::hstring const& property, winrt::Windows::UI::Color const& value, TimeSpan const& duration)
        {
            // Stop previous animations
            target.StopAnimation(property);

            auto compositor = target.Compositor();
            // Setup the animation to run
            auto animation = compositor.CreateColorKeyFrameAnimation();
            animation.InsertKeyFrame(1.0f, value);
            animation.Duration(duration);

            // Get the batch and start the animations
            auto batch = compositor.CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

            wil::shared_event completionEvent(wil::EventOptions::ManualReset);

            batch.Completed([completionEvent](auto&&, auto&&)
            {
				completionEvent.SetEvent();
            });

            target.StartAnimation(property, animation);

            batch.End();

            co_await winrt::resume_on_signal(completionEvent.get());
        }

        /// <summary>
        /// Starts an animation on the given property of a CompositionObject (Vector4)
        /// </summary>
        static winrt::IAsyncAction StartAnimationAsync(winrt::CompositionObject const& target, winrt::hstring const& property, winrt::float4 const& value, winrt::TimeSpan const& duration)
        {
            // Stop previous animations
            target.StopAnimation(property);

            // Setup the animation to run
            auto animation = target.Compositor().CreateVector4KeyFrameAnimation();
            animation.InsertKeyFrame(1.0f, value);
            animation.Duration(duration);

            // Get the batch and start the animations
            auto batch = target.Compositor().CreateScopedBatch(winrt::CompositionBatchTypes::Animation);

            wil::shared_event completionEvent(wil::EventOptions::ManualReset);

            batch.Completed([completionEvent](auto&&, auto&&)
            {
				completionEvent.SetEvent();
            });

            target.StartAnimation(property, animation);

            batch.End();

            co_await winrt::resume_on_signal(completionEvent.get());
        }
    };
}