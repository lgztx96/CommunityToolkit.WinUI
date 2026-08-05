// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once
#include "../../Builders/AnimationBuilder.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// An interface representing a XAML model for a custom animation that requires a specific parent UIElement context.
    /// </summary>
    struct __declspec(uuid("F84C9A72-31E5-4D6B-B2F1-9C7E4A58D3F6")) IAttachedTimeline : ::IUnknown
    {
    public:
        /// <summary>
        /// Appends the current animation to a target AnimationBuilder instance.
        /// </summary>
        virtual AnimationBuilder& AppendToBuilder(
            AnimationBuilder& builder,
            winrt::UIElement const& parent,
            std::optional<winrt::TimeSpan> delayHint = std::nullopt,
            std::optional<winrt::TimeSpan> durationHint = std::nullopt,
            std::optional<EasingType> easingTypeHint = std::nullopt,
            std::optional<winrt::EasingMode> easingModeHint = std::nullopt) = 0;

        virtual ~IAttachedTimeline() = default;
    };
}