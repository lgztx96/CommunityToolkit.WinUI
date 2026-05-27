#pragma once

#include "AnimationBuilder.h"
#include "AnimationBuilder.PropertyBuilders.h"

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
	using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    class AnimationBuilderDefault
    {
    private:
        template<typename T>
        static AnimationBuilder& AddValueAnimation(
            PropertyAnimationBuilder<T> propertyBuilder,
            T const& to,
            std::optional<T> const& from,
            std::optional<TimeSpan> delay,
            std::optional<TimeSpan> duration,
            std::optional<RepeatOption> repeat,
            EasingType easingType,
            EasingMode easingMode)
        {
            return propertyBuilder.NormalizedKeyFrames(
                [from, to, easingType, easingMode](INormalizedKeyFrameAnimationBuilder<T>& keyFrames)
                {
                    if (from.has_value())
                    {
                        keyFrames.KeyFrame(0.0, from.value());
                    }

                    keyFrames.KeyFrame(1.0, to, easingType, easingMode);
                },
                delay,
                duration,
                repeat,
                AnimationExtensions::DefaultDelayBehavior());
        }

        static bool TryDecomposeMatrix(float4x4 const& matrix, float3& scale, quaternion& rotation, float3& translation)
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

    public:
        static AnimationBuilder& AnchorPoint(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::AnchorPoint(builder, axis), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& AnchorPoint(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::AnchorPoint(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Opacity(
            AnimationBuilder& builder,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Opacity(builder, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Translation(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Translation(builder, axis, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Translation(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::TranslationXY(builder), to, from, delay, duration, repeat, easingType, easingMode);
            }

            Translation(builder, Axis::X, static_cast<double>(to.x), from ? std::optional<double>(from.value().x) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);
            Translation(builder, Axis::Y, static_cast<double>(to.y), from ? std::optional<double>(from.value().y) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);

            return builder;
        }

        static AnimationBuilder& Translation(
            AnimationBuilder& builder,
            float3 const& to,
            std::optional<float3> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float3>(AnimationBuilderPropertyBuilders::Translation(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Offset(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Offset(builder, axis), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Offset(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::OffsetXY(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Offset(
            AnimationBuilder& builder,
            float3 const& to,
            std::optional<float3> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float3>(AnimationBuilderPropertyBuilders::Offset(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Scale(
            AnimationBuilder& builder,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                float toValue = static_cast<float>(to);
                std::optional<float3> from3 = from ? std::optional<float3>(float3{ static_cast<float>(from.value()) }) : std::nullopt;

                return AddValueAnimation<float3>(
                    AnimationBuilderPropertyBuilders::Scale(builder),
                    float3{ toValue, toValue, toValue },
                    from3,
                    delay,
                    duration,
                    repeat,
                    easingType,
                    easingMode);
            }

            Scale(builder, Axis::X, to, from, delay, duration, repeat, easingType, easingMode, layer);
            Scale(builder, Axis::Y, to, from, delay, duration, repeat, easingType, easingMode, layer);

            return builder;
        }

        static AnimationBuilder& Scale(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Scale(builder, axis, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Scale(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::ScaleXY(builder), to, from, delay, duration, repeat, easingType, easingMode);
            }

            Scale(builder, Axis::X, static_cast<double>(to.x), from ? std::optional<double>(from.value().x) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);
            Scale(builder, Axis::Y, static_cast<double>(to.y), from ? std::optional<double>(from.value().y) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);

            return builder;
        }

        static AnimationBuilder& Scale(
            AnimationBuilder& builder,
            float3 const& to,
            std::optional<float3> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float3>(AnimationBuilderPropertyBuilders::Scale(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& CenterPoint(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::CenterPoint(builder, axis, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& CenterPoint(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::CenterPointXY(builder), to, from, delay, duration, repeat, easingType, easingMode);
            }

            CenterPoint(builder, Axis::X, static_cast<double>(to.x), from ? std::optional<double>(from.value().x) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);
            CenterPoint(builder, Axis::Y, static_cast<double>(to.y), from ? std::optional<double>(from.value().y) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);

            return builder;
        }

        static AnimationBuilder& CenterPoint(
            AnimationBuilder& builder,
            float3 const& to,
            std::optional<float3> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float3>(AnimationBuilderPropertyBuilders::CenterPoint(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Rotation(
            AnimationBuilder& builder,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Rotation(builder), to, from, delay, duration, repeat, easingType, easingMode);
            }

            constexpr double ratio = std::numbers::pi / 180.0;
            std::optional<double> fromDegrees = from ? std::optional<double>(from.value() * ratio) : std::nullopt;
            return AddValueAnimation<double>(
                AnimationBuilderPropertyBuilders::RotationInDegrees(builder, layer),
                to * ratio,
                fromDegrees,
                delay,
                duration,
                repeat,
                easingType,
                easingMode);
        }

        static AnimationBuilder& RotationInDegrees(
            AnimationBuilder& builder,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::RotationInDegrees(builder, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& RotationAxis(
            AnimationBuilder& builder,
            float3 const& to,
            std::optional<float3> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<float3>(AnimationBuilderPropertyBuilders::RotationAxis(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Orientation(
            AnimationBuilder& builder,
            quaternion const& to,
            std::optional<quaternion> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<quaternion>(AnimationBuilderPropertyBuilders::Orientation(builder), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Transform(
            AnimationBuilder& builder,
            float4x4 const& to,
            std::optional<float4x4> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            float3 toScale{};
            quaternion toRotation{};
            float3 toTranslation{};

            if (!TryDecomposeMatrix(to, toScale, toRotation, toTranslation))
            {
                throw winrt::hresult_invalid_argument(L"The destination matrix could not be decomposed");
            }

            std::optional<float3> fromScale;
            std::optional<quaternion> fromRotation;
            std::optional<float3> fromTranslation;

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

            Scale(builder, toScale, fromScale, delay, duration, repeat, easingType, easingMode);
            Orientation(builder, toRotation, fromRotation, delay, duration, repeat, easingType, easingMode);
            Translation(builder, toTranslation, fromTranslation, delay, duration, repeat, easingType, easingMode);

            return builder;
        }

        static AnimationBuilder& Clip(
            AnimationBuilder& builder,
            Side side,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Clip(builder, side), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Clip(
            AnimationBuilder& builder,
            winrt::Microsoft::UI::Xaml::Thickness const& to,
            std::optional<winrt::Microsoft::UI::Xaml::Thickness> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode())
        {
            Clip(builder, Side::Left, to.Left, from ? std::optional<double>(from.value().Left) : std::nullopt, delay, duration, repeat, easingType, easingMode);
            Clip(builder, Side::Top, to.Top, from ? std::optional<double>(from.value().Top) : std::nullopt, delay, duration, repeat, easingType, easingMode);
            Clip(builder, Side::Right, to.Right, from ? std::optional<double>(from.value().Right) : std::nullopt, delay, duration, repeat, easingType, easingMode);
            Clip(builder, Side::Bottom, to.Bottom, from ? std::optional<double>(from.value().Bottom) : std::nullopt, delay, duration, repeat, easingType, easingMode);

            return builder;
        }

        static AnimationBuilder& Size(
            AnimationBuilder& builder,
            Axis axis,
            double to,
            std::optional<double> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            return AddValueAnimation<double>(AnimationBuilderPropertyBuilders::Size(builder, axis, layer), to, from, delay, duration, repeat, easingType, easingMode);
        }

        static AnimationBuilder& Size(
            AnimationBuilder& builder,
            float2 const& to,
            std::optional<float2> from = std::nullopt,
            std::optional<TimeSpan> delay = std::nullopt,
            std::optional<TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeat = std::nullopt,
            EasingType easingType = AnimationExtensions::DefaultEasingType(),
            EasingMode easingMode = AnimationExtensions::DefaultEasingMode(),
            FrameworkLayer layer = FrameworkLayer::Composition)
        {
            if (layer == FrameworkLayer::Composition)
            {
                return AddValueAnimation<float2>(AnimationBuilderPropertyBuilders::Size(builder), to, from, delay, duration, repeat, easingType, easingMode);
            }

            Size(builder, Axis::X, static_cast<double>(to.x), from ? std::optional<double>(from.value().x) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);
            Size(builder, Axis::Y, static_cast<double>(to.y), from ? std::optional<double>(from.value().y) : std::nullopt, delay, duration, repeat, easingType, easingMode, layer);

            return builder;
        }
    };
}
