// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once
#include "AnimationBuilder.h"
#include "AnimationBuilder.Factories.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    template<typename T>
    inline AnimationBuilder& AnimationBuilder::AddCompositionAnimationFactory(
        winrt::hstring const& property,
        T const& to,
        std::optional<T> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        auto animation = std::make_unique<AnimationFactory<T>>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode);

        compositionAnimationFactories.emplace_back(std::move(animation));

        return *this;
    }

    template<typename T>
    inline AnimationBuilder& AnimationBuilder::AddXamlAnimationFactory(
        winrt::hstring const& property,
        T const& to,
        std::optional<T> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        auto animation = std::make_unique<AnimationFactory<T>>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode);

        xamlAnimationFactories.emplace_back(std::move(animation));

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::AddXamlTransformDoubleAnimationFactory(
        winrt::hstring const& property,
        double to,
        std::optional<double> from,
        std::optional<winrt::Windows::Foundation::TimeSpan> delay,
        std::optional<winrt::Windows::Foundation::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        auto animation = std::make_unique<XamlTransformDoubleAnimationFactory>(
            property,
            to,
            from,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode);

        xamlAnimationFactories.emplace_back(std::move(animation));

        return *this;
    }
}