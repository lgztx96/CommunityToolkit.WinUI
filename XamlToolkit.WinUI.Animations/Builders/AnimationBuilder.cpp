// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationBuilder.h"
#include "AnimationBuilder.PropertyBuilders.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Hosting;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    namespace
    {
        inline void StartCompositionAnimations(
            std::vector<std::unique_ptr<ICompositionAnimationFactory>> const& factories,
            UIElement const& element,
            std::optional<CompositionScopedBatch> const& batch = std::nullopt)
        {
            ElementCompositionPreview::SetIsTranslationEnabled(element, true);

            Visual visual = ElementCompositionPreview::GetElementVisual(element);

            for (auto const& factory : factories)
            {
                CompositionObject target{ nullptr };
                auto animation = factory->GetAnimation(visual, target);

                (target ? target : visual).StartAnimation(animation.Target(), animation);
            }

            if (batch.has_value())
            {
                batch->End();
            }
        }

        inline bool TryDecomposeMatrix(float4x4 const& matrix, float3& scale, quaternion& rotation, float3& translation)
        {
            DirectX::XMFLOAT4X4 m{
                matrix.m11, matrix.m12, matrix.m13, matrix.m14,
                matrix.m21, matrix.m22, matrix.m23, matrix.m24,
                matrix.m31, matrix.m32, matrix.m33, matrix.m34,
                matrix.m41, matrix.m42, matrix.m43, matrix.m44
            };

            DirectX::XMVECTOR scaleVec{};
            DirectX::XMVECTOR rotVec{};
            DirectX::XMVECTOR transVec{};

            if (!DirectX::XMMatrixDecompose(&scaleVec, &rotVec, &transVec, DirectX::XMLoadFloat4x4(&m)))
            {
                return false;
            }

            DirectX::XMFLOAT3 s{};
            DirectX::XMFLOAT4 r{};
            DirectX::XMFLOAT3 t{};

            DirectX::XMStoreFloat3(&s, scaleVec);
            DirectX::XMStoreFloat4(&r, rotVec);
            DirectX::XMStoreFloat3(&t, transVec);

            scale = float3{ s.x, s.y, s.z };
            rotation = quaternion{ r.x, r.y, r.z, r.w };
            translation = float3{ t.x, t.y, t.z };

            return true;
        }
    }

    AnimationBuilder& AnimationBuilder::AnchorPoint(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::AnchorPoint(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::AnchorPoint(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"AnchorPoint", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Opacity(
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition ? AddCompositionAnimationFactory(L"Opacity", to, from, delay, duration, repeat, easingType, easingMode) : AddXamlAnimationFactory(L"Opacity", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Translation(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition 
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Translation(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::Translation(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Translation(
        float3 const& to,
        std::optional<float3> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Translation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Scale(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Scale(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::Scale(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Scale(
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            float toValue = static_cast<float>(to);
            std::optional<float3> fromValue = from ? std::optional<float3>(float3{ static_cast<float>(from.value()) }) : std::nullopt;

            return AddCompositionAnimationFactory(L"Scale", float3{ toValue, toValue, toValue }, fromValue, delay, duration, repeat, easingType, easingMode);
        }

        AddXamlTransformDoubleAnimationFactory(L"ScaleX", to, from, delay, duration, repeat, easingType, easingMode);
        AddXamlTransformDoubleAnimationFactory(L"ScaleY", to, from, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    AnimationBuilder& AnimationBuilder::Offset(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Offset(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Offset(
        float3 const& to,
        std::optional<float3> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Offset", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::CenterPoint(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition 
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::CenterPoint(axis), to, from, delay, duration, repeat, easingType, easingMode)
            : AddXamlTransformDoubleAnimationFactory(AnimationExtensions::Properties::Xaml::CenterPoint(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::CenterPoint(
        float3 const& to,
        std::optional<float3> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"CenterPoint", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Rotation(
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
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

    AnimationBuilder& AnimationBuilder::RotationInDegrees(
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        if (layer == FrameworkLayer::Composition)
        {
            return AddCompositionAnimationFactory(L"RotationAngleInDegrees", to, from, delay, duration, repeat, easingType, easingMode);
        }

        return AddXamlTransformDoubleAnimationFactory(L"Rotation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Translation(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
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

    AnimationBuilder& AnimationBuilder::Scale(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
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

    AnimationBuilder& AnimationBuilder::Scale(
        float3 const& to,
        std::optional<float3> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Scale", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Offset(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::OffsetXY(), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::CenterPoint(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
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

    AnimationBuilder& AnimationBuilder::RotationAxis(
        float3 const& to,
        std::optional<float3> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"RotationAxis", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Orientation(
        quaternion const& to,
        std::optional<quaternion> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionAnimationFactory(L"Orientation", to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Transform(
        float4x4 const& to,
        std::optional<float4x4> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        float3 toScale{};
        quaternion toRotation{};
        float3 toTranslation{};

        if (!TryDecomposeMatrix(to, toScale, toRotation, toTranslation))
        {
            throw winrt::hresult_invalid_argument(L"The destination matrix could not be decomposed");
        }

        std::optional<float3> fromScale = std::nullopt;
        std::optional<quaternion> fromRotation = std::nullopt;
        std::optional<float3> fromTranslation = std::nullopt;

        if (from.has_value())
        {
            float3 scale3{};
            quaternion rotation4{};
            float3 translation3{};

            if (!TryDecomposeMatrix(from.value(), scale3, rotation4, translation3))
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

    AnimationBuilder& AnimationBuilder::Clip(
        Side side,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        return AddCompositionClipAnimationFactory(AnimationExtensions::Properties::Composition::Clip(side), static_cast<float>(to), from ? std::optional<float>(static_cast<float>(from.value())) : std::nullopt, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Clip(
        winrt::Microsoft::UI::Xaml::Thickness const& to,
        std::optional<winrt::Microsoft::UI::Xaml::Thickness> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode)
    {
        Clip(Side::Left, to.Left, from ? std::optional<double>(from->Left) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Top, to.Top, from ? std::optional<double>(from->Top) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Right, to.Right, from ? std::optional<double>(from->Right) : std::nullopt, delay, duration, repeat, easingType, easingMode);
        Clip(Side::Bottom, to.Bottom, from ? std::optional<double>(from->Bottom) : std::nullopt, delay, duration, repeat, easingType, easingMode);

        return *this;
    }

    AnimationBuilder& AnimationBuilder::Size(
        Axis axis,
        double to,
        std::optional<double> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
        FrameworkLayer layer)
    {
        return layer == FrameworkLayer::Composition 
            ? AddCompositionAnimationFactory(AnimationExtensions::Properties::Composition::Size(axis), to, from, delay, duration, repeat, easingType, easingMode) 
            : AddXamlAnimationFactory(AnimationExtensions::Properties::Xaml::Size(axis), to, from, delay, duration, repeat, easingType, easingMode);
    }

    AnimationBuilder& AnimationBuilder::Size(
        float2 const& to,
        std::optional<float2> from,
        std::optional<TimeSpan> delay,
        std::optional<TimeSpan> duration,
        std::optional<RepeatOption> repeat,
        EasingType easingType,
        EasingMode easingMode,
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


    PropertyAnimationBuilder<double> AnimationBuilder::AnchorPoint(Axis axis)
    {
        return AnimationBuilderPropertyBuilders::AnchorPoint(*this, axis);
    }

    PropertyAnimationBuilder<float2> AnimationBuilder::AnchorPoint()
    {
        return AnimationBuilderPropertyBuilders::AnchorPoint(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Opacity(FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::Opacity(*this, layer);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Translation(Axis axis, FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::Translation(*this, axis, layer);
    }

    PropertyAnimationBuilder<float3> AnimationBuilder::Translation()
    {
        return AnimationBuilderPropertyBuilders::Translation(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Offset(Axis axis)
    {
        return AnimationBuilderPropertyBuilders::Offset(*this, axis);
    }

    PropertyAnimationBuilder<float3> AnimationBuilder::Offset()
    {
        return AnimationBuilderPropertyBuilders::Offset(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Scale(Axis axis, FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::Scale(*this, axis, layer);
    }

    PropertyAnimationBuilder<float3> AnimationBuilder::Scale()
    {
        return AnimationBuilderPropertyBuilders::Scale(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::CenterPoint(Axis axis, FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::CenterPoint(*this, axis, layer);
    }

    PropertyAnimationBuilder<float3> AnimationBuilder::CenterPoint()
    {
        return AnimationBuilderPropertyBuilders::CenterPoint(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Rotation()
    {
        return AnimationBuilderPropertyBuilders::Rotation(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::RotationInDegrees(FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::RotationInDegrees(*this, layer);
    }

    PropertyAnimationBuilder<float3> AnimationBuilder::RotationAxis()
    {
        return AnimationBuilderPropertyBuilders::RotationAxis(*this);
    }

    PropertyAnimationBuilder<quaternion> AnimationBuilder::Orientation()
    {
        return AnimationBuilderPropertyBuilders::Orientation(*this);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Clip(Side side)
    {
        return AnimationBuilderPropertyBuilders::Clip(*this, side);
    }

    PropertyAnimationBuilder<double> AnimationBuilder::Size(Axis axis, FrameworkLayer layer)
    {
        return AnimationBuilderPropertyBuilders::Size(*this, axis, layer);
    }

    PropertyAnimationBuilder<float2> AnimationBuilder::Size()
    {
        return AnimationBuilderPropertyBuilders::Size(*this);
    }

    void AnimationBuilder::Start(UIElement const& element)
    {
        if (!compositionAnimationFactories.empty())
        {
            ElementCompositionPreview::SetIsTranslationEnabled(element, true);

            Visual visual = ElementCompositionPreview::GetElementVisual(element);

            for (const auto& factory : compositionAnimationFactories)
            {
                CompositionObject target{ nullptr };
                auto animation = factory->GetAnimation(visual, target);

                if (target == nullptr)
                {
                    visual.StartAnimation(animation.Target(), animation);
                }
                else
                {
                    target.StartAnimation(animation.Target(), animation);
                }
            }
        }

        if (!xamlAnimationFactories.empty())
        {
            Storyboard storyboard;
            auto children = storyboard.Children();
            for (auto const& factory : xamlAnimationFactories)
            {
                children.Append(factory->GetAnimation(element));
            }

            storyboard.Begin();
        }
    }

    void AnimationBuilder::Start(UIElement const& element, std::function<void()> callback)
    {
        // The point of this overload is to allow consumers to invoke a callback when an animation
        // completes, without having to create an async state machine. There are three different possible
        // scenarios to handle, and each can have a specialized code path to ensure the implementation
        // is as lean and efficient as possible. Specifically, for a given AnimationBuilder instance:
        //   1) There are only Composition animations
        //   2) There are only XAML animations
        //   3) There are both Composition and XAML animations
        // The implementation details of each of these paths is described below.
        if (!compositionAnimationFactories.empty())
        {
            if (xamlAnimationFactories.empty())
            {
                // Only Composition animations
                ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                Visual visual = ElementCompositionPreview::GetElementVisual(element);
                CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(CompositionBatchTypes::Animation);

                batch.Completed([callback](auto&&, auto&&) { callback(); });
                StartCompositionAnimations(compositionAnimationFactories, element, batch);
            }
            else
            {
                // In this case we need to wait for both the Composition and XAML animation groups to complete. These two
                // groups use different APIs and can have a different duration, so we need to synchronize between them
                // without creating an async state machine (as that'd defeat the point of this separate overload).
                //
                // The code below relies on a mutable boxed counter that's shared across the two closures for the Completed
                // events for both the Composition scoped batch and the XAML Storyboard. The counter is initialized to 2, and
                // when each group completes, the counter is decremented (we don't need an interlocked decrement as the delegates
                // will already be invoked on the current DispatcherQueue instance, which acts as the synchronization context here.
                // The handlers for the Composition batch and the Storyboard will never execute concurrently). If the counter has
                // reached zero, it means that both groups have completed, so the user-provided callback is triggered, otherwise
                // the handler just does nothing. This ensures that the callback is executed exactly once when all the animation
                // complete, but without the need to create TaskCompletionSource-s and an async state machine to await for that.
                //
                // Note: we're using StrongBox<T> here because that exposes a mutable field of the type we need (int).
                // We can't just mutate a boxed int in-place with Unsafe.Unbox<T> as that's against the ECMA spec, since
                // that API uses the unbox IL opcode (§III.4.32) which returns a "controlled-mutability managed pointer"
                // (§III.1.8.1.2.2), which is not "verifier-assignable-to" (ie. directly assigning to it is not legal).
                auto counter = std::make_shared<int>(2);

                ElementCompositionPreview::SetIsTranslationEnabled(element, true);

                Visual visual = ElementCompositionPreview::GetElementVisual(element);
                CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(CompositionBatchTypes::Animation);

                batch.Completed([counter, callback](auto&&, auto&&) {
                    if (--(*counter) == 0)
                    {
                        callback();
                    }
                });
                StartCompositionAnimations(compositionAnimationFactories, element, batch);

                Storyboard storyboard;
                auto children = storyboard.Children();
                for (auto const& factory : xamlAnimationFactories)
                {
                    children.Append(factory->GetAnimation(element));
                }

                storyboard.Completed([counter, callback](auto&&, auto&&) {
  
                    if (--(*counter) == 0)
                    {
                        callback();
                    }
                });
                storyboard.Begin();
            }
        }
        else
        {
            // There are only XAML animations. This case is extremely similar to that where we only have Composition
            // animations, with the main difference being that the Completed event is directly exposed from the
            // Storyboard type, so we don't need a separate type to track the animation completion. The same
            // considerations regarding the closure to capture the provided callback apply here as well.
            Storyboard storyboard;
            auto children = storyboard.Children();
            for (auto const& factory : xamlAnimationFactories)
            {
                children.Append(factory->GetAnimation(element));
            }

            storyboard.Completed([callback](auto&&, auto&&) { callback(); });
            storyboard.Begin();
        }
    }

    winrt::Windows::Foundation::IAsyncAction AnimationBuilder::StartAsync(UIElement const& element)
    {
        winrt::Windows::Foundation::IAsyncAction compositionTask{ nullptr };
        winrt::Windows::Foundation::IAsyncAction xamlTask{ nullptr };
        auto cancelation_token{ co_await winrt::get_cancellation_token() };

		std::vector<std::tuple<CompositionObject, winrt::hstring>> compositionAnimations;

        if (!compositionAnimationFactories.empty())
        {
            auto completionEvent = std::make_shared<winrt::handle>(winrt::check_pointer(CreateEventW(nullptr, true, false, nullptr)));

            ElementCompositionPreview::SetIsTranslationEnabled(element, true);

            Visual visual = ElementCompositionPreview::GetElementVisual(element);
            CompositionScopedBatch batch = visual.Compositor().CreateScopedBatch(CompositionBatchTypes::Animation);

            batch.Completed([completionEvent](auto&&, auto&&)
            {
                 SetEvent(completionEvent->get());
            });

            for (const auto& factory : compositionAnimationFactories)
            {
                CompositionObject target{ nullptr };
                auto animation = factory->GetAnimation(visual, target);

                if (target == nullptr)
                {
                    visual.StartAnimation(animation.Target(), animation);
                }
                else
                {
                    target.StartAnimation(animation.Target(), animation);
                }

                compositionAnimations.emplace_back(target ? target : visual, animation.Target());
            }

            batch.End();

            compositionTask = [completionEvent]() -> winrt::Windows::Foundation::IAsyncAction
            {
                co_await winrt::resume_on_signal(completionEvent->get());
            }();
        }

        Storyboard storyboard{ nullptr };

        if (!xamlAnimationFactories.empty())
        {
			storyboard = Storyboard();
            auto completionEvent = std::make_shared<winrt::handle>(winrt::check_pointer(CreateEventW(nullptr, true, false, nullptr)));
            
            auto children = storyboard.Children();
            for (auto const& factory : xamlAnimationFactories)
            {
                children.Append(factory->GetAnimation(element));
            }

            storyboard.Completed([completionEvent](auto&&, auto&&)
            {
                 SetEvent(completionEvent->get());
            });

            storyboard.Begin();

            xamlTask = [completionEvent]() -> winrt::Windows::Foundation::IAsyncAction
            {
                co_await winrt::resume_on_signal(completionEvent->get());
            }();
        }

        cancelation_token.callback([=] 
        {
            for (const auto& [target, path] : compositionAnimations)
            {
                target.StopAnimation(path);
            }

            if (storyboard) storyboard.Stop();
        });

        if (compositionTask) co_await compositionTask;
        if (xamlTask) co_await xamlTask;
    }
}
