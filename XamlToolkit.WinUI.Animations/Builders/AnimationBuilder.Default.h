#pragma once

#include "AnimationBuilder.h"
#include "AnimationBuilder.PropertyBuilders.h"
#include "AnimationBuilder.Factories.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/XamlToolkit.WinUI.Animations.h>
#include <numbers>
#else
import winrt.XamlToolkit.WinUI.Animations;
#endif

#include <Windowsnumerics.h>

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
    using namespace Microsoft::UI::Xaml::Media::Animation;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    inline AnimationBuilder& AnimationBuilder::AnchorPoint(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::AnchorPoint(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::AnchorPoint(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"AnchorPoint", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Opacity(
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition ? AddCompositionAnimationFactory(L"Opacity", to, from, delay, duration, repeat, easingType, easingMode) : AddXamlAnimationFactory(L"Opacity", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Translation(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Translation(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::Translation(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Translation(
        winrt::float3 const& to,
        std::optional<winrt::float3> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Translation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Scale(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Scale(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::Scale(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Scale(
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            float toValue = static_cast<float>(to);
            std::optional<winrt::float3> fromValue = from ? std::optional<winrt::float3>(winrt::float3{ static_cast<float>(from.value()) }) : std::nullopt;

            return AddCompositionAnimationFactory(L"Scale", winrt::float3{ toValue, toValue, toValue }, fromValue, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlTransformDoubleAnimationFactory(L"ScaleX", to, from, delay, duration, repeat, easingType, easingMode);
        AddXamlTransformDoubleAnimationFactory(L"ScaleY", to, from, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Offset(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Offset(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Offset(
        winrt::float3 const& to,
        std::optional<winrt::float3> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Offset", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::CenterPoint(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::CenterPoint(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::CenterPoint(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::CenterPoint(
        winrt::float3 const& to,
        std::optional<winrt::float3> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"CenterPoint", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Rotation(
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(L"RotationAngle", to, from, delay, duration, repeat, easingType, easingMode);
        }

        constexpr double radiansToDegrees = 180.0 / std::numbers::pi_v<double>;
        std::optional<double> fromDegrees = from ? std::optional<double>(from.value() * radiansToDegrees) : std::nullopt;
        double toDegrees = to * radiansToDegrees;

        return AddXamlTransformDoubleAnimationFactory(L"Rotation", toDegrees, fromDegrees, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::RotationInDegrees(
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(L"RotationAngleInDegrees", to, from, delay, duration, repeat, easingType, easingMode);
        }

        return AddXamlTransformDoubleAnimationFactory(L"Rotation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Translation(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::TranslationXY(), to, from, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlTransformDoubleAnimationFactory(L"TranslateX", to.x, from ? std::optional<double>(from->x) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        AddXamlTransformDoubleAnimationFactory(L"TranslateY", to.y, from ? std::optional<double>(from->y) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Scale(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::ScaleXY(), to, from, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlTransformDoubleAnimationFactory(L"ScaleX", to.x, from ? std::optional<double>(from->x) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        AddXamlTransformDoubleAnimationFactory(L"ScaleY", to.y, from ? std::optional<double>(from->y) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Scale(
        winrt::float3 const& to,
        std::optional<winrt::float3> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Scale", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Offset(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::OffsetXY(), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::CenterPoint(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::CenterPointXY(), to, from, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlTransformDoubleAnimationFactory(L"CenterX", to.x, from ? std::optional<double>(from->x) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        AddXamlTransformDoubleAnimationFactory(L"CenterY", to.y, from ? std::optional<double>(from->y) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::RotationAxis(
        winrt::float3 const& to,
        std::optional<winrt::float3> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"RotationAxis", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Orientation(
        winrt::quaternion const& to,
        std::optional<winrt::quaternion> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Orientation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Transform(
        float4x4 const& to,
        std::optional<float4x4> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        winrt::float3 toScale{};
        winrt::quaternion toRotation{};
        winrt::float3 toTranslation{};

        if (!decompose(to, &toScale, &toRotation, &toTranslation))
        {
            throw winrt::hresult_invalid_argument(L"The destination matrix could not be decomposed");
        }

        std::optional<winrt::float3> fromScale = std::nullopt;
        std::optional<winrt::quaternion> fromRotation = std::nullopt;
        std::optional<winrt::float3> fromTranslation = std::nullopt;

        if (from.has_value())
        {
            winrt::float3 scale3{};
            winrt::quaternion rotation4{};
            winrt::float3 translation3{};

            if (!decompose(from.value(), &scale3, &rotation4, &translation3))
            {
                throw winrt::hresult_invalid_argument(L"The initial matrix could not be decomposed");
            }

            fromScale = scale3;
            fromRotation = rotation4;
            fromTranslation = translation3;
        }

        Scale(toScale, fromScale, delay, duration, repeat, easingType, easingMode);
        Orientation(toRotation, fromRotation, delay, duration, repeat, easingType, easingMode);
        Translation(toTranslation, fromTranslation, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Clip(
        Side side,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
         auto animation = std::make_unique<CompositionClipScalarAnimation>(
            AnimationExtensions::Properties::Composition::Clip(side),
            static_cast<float>(to),
            from ? std::optional<float>(static_cast<float>(from.value())) : std::nullopt,
            delay.value_or(AnimationExtensions::DefaultDelay()),
            duration.value_or(AnimationExtensions::DefaultDuration()),
            repeat.value_or(RepeatOptionHelper::Once()),
            easingType,
            easingMode);

        compositionAnimationFactories.emplace_back(std::move(animation));

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Clip(
        winrt::Thickness const& to,
        std::optional<winrt::Thickness> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode)
    {
        Clip(Side::Left, to.Left, from ? std::optional<double>(from->Left) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Top, to.Top, from ? std::optional<double>(from->Top) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Right, to.Right, from ? std::optional<double>(from->Right) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Bottom, to.Bottom, from ? std::optional<double>(from->Bottom) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    inline AnimationBuilder& AnimationBuilder::Size(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Size(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlAnimationFactory(AnimationExtensions::Properties::Xaml::Size(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    inline AnimationBuilder& AnimationBuilder::Size(
        winrt::float2 const& to,
        std::optional<winrt::float2> from,
        std::optional<winrt::TimeSpan> delay,
        std::optional<winrt::TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        winrt::EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(L"Size", to, from, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlAnimationFactory(L"Width", static_cast<double>(to.x), from ? std::optional<double>(from->x) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        AddXamlAnimationFactory(L"Height", static_cast<double>(to.y), from ? std::optional<double>(from->y) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }
}
