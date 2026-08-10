// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once
#include "../../Builders/AnimationBuilder.h"
#ifdef __INTELLISENSE__
#include <rpcndr.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A C++ interface representing a XAML model for a custom animation.
    /// This is a C++-only interface because it uses AnimationBuilder.
    /// </summary>
    MIDL_INTERFACE("2B9F4D8C-76A1-43E7-91D5-E8C24F6AB173") ITimeline : ::IUnknown
    {
    public:
        /// <summary>
        /// Appends the current animation to a target AnimationBuilder instance.
        /// This method is used when the current timeline instance is explicitly triggered.
        /// </summary>
        /// <param name="builder">The target AnimationBuilder instance to schedule the animation on.</param>
        /// <param name="delayHint">A hint for the animation delay, if present.</param>
        /// <param name="durationHint">A hint for the animation duration, if present.</param>
        /// <param name="easingTypeHint">A hint for the easing type, if present.</param>
        /// <param name="easingModeHint">A hint for the easing mode, if present.</param>
        /// <returns>The same AnimationBuilder instance as builder.</returns>
        virtual AnimationBuilder& AppendToBuilder(
            AnimationBuilder& builder,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt) = 0;
    };
}