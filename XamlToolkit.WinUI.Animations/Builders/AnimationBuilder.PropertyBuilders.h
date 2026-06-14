#pragma once

#include "AnimationBuilder.h"
#include "interfaces/IPropertyAnimationBuilder{T}.h"
#include "NormalizedKeyFrameAnimationBuilder{T}.Xaml.h"
#include "TimedKeyFrameAnimationBuilder{T}.Xaml.h"

namespace winrt
{
    using namespace Microsoft::UI::Composition;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Media::Animation;
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::WinUI::Animations
{
    template<typename TFactory>
        requires std::is_base_of_v<ICompositionAnimationFactory, std::remove_cv_t<TFactory>>
    class CompositionClipAnimationFactory final : public ICompositionAnimationFactory
    {
    private:
        TFactory factory;

    public:
        explicit CompositionClipAnimationFactory(TFactory const& factory)
            : factory(factory)
        {
        }

        CompositionAnimation GetAnimation(CompositionObject const& targetHint, CompositionObject& target) override
        {
            auto visual = targetHint.try_as<Visual>();

            if (!visual)
            {
                throw winrt::hresult_invalid_argument(L"Clip property animations require a Visual target.");
            }

            auto clip = visual.Clip().try_as<InsetClip>();

            if (!clip)
            {
                clip = visual.Compositor().CreateInsetClip();
                visual.Clip(clip);
            }

            CompositionObject ignored{ nullptr };
            auto animation = factory.GetAnimation(clip, ignored);

            target = clip;

            return animation;
        }
    };

    template<typename TFactory>
        requires std::is_base_of_v<IXamlAnimationFactory, std::remove_cv_t<TFactory>>
    class XamlTransformAnimationFactory final : public IXamlAnimationFactory
    {
    private:
        TFactory factory;

    public:
        explicit XamlTransformAnimationFactory(TFactory const& factory)
            : factory(factory)
        {
        }

        Timeline GetAnimation(DependencyObject const& targetHint) override
        {
            auto element = targetHint.try_as<UIElement>();

            if (!element)
            {
                throw winrt::hresult_invalid_argument(L"XAML transform property animations require a UIElement target.");
            }

            auto transform = element.RenderTransform().try_as<CompositeTransform>();

            if (!transform)
            {
                transform = CompositeTransform();
                element.RenderTransform(transform);
            }

            return factory.GetAnimation(transform);
        }
    };

    enum class PropertyAnimationBuilderTarget
    {
        Default,
        CompositionClip,
        XamlTransform,
    };

    /// <summary>
    /// A custom <see cref="IPropertyAnimationBuilder{T}"/> for a shared animation.
    /// </summary>
    template<typename T>
    class PropertyAnimationBuilder : public IPropertyAnimationBuilder<T>
    {
    private:
        AnimationBuilder& builder;
        winrt::hstring property;
        FrameworkLayer layer;
        PropertyAnimationBuilderTarget targetKind;

        PropertyAnimationBuilder(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            FrameworkLayer layer,
            PropertyAnimationBuilderTarget targetKind)
            : builder(builder)
            , property(property)
            , layer(layer)
            , targetKind(targetKind)
        {
        }

    public:
        PropertyAnimationBuilder(
            AnimationBuilder& builder,
            winrt::hstring const& property,
            FrameworkLayer layer)
            : PropertyAnimationBuilder(builder, property, layer, PropertyAnimationBuilderTarget::Default)
        {
        }

        static PropertyAnimationBuilder CreateCompositionClip(
            AnimationBuilder& builder,
            winrt::hstring const& property)
        {
            return PropertyAnimationBuilder(builder, property, FrameworkLayer::Composition, PropertyAnimationBuilderTarget::CompositionClip);
        }

        static PropertyAnimationBuilder CreateXamlTransform(
            AnimationBuilder& builder,
            winrt::hstring const& property)
        {
            return PropertyAnimationBuilder(builder, property, FrameworkLayer::Xaml, PropertyAnimationBuilderTarget::XamlTransform);
        }

        AnimationBuilder& NormalizedKeyFrames(
            std::function<void(INormalizedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<winrt::Windows::Foundation::TimeSpan> duration = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) override
        {
            if (targetKind == PropertyAnimationBuilderTarget::Default)
            {
                return builder.NormalizedKeyFrames<T>(property, std::move(build), delay, duration, repeatOption, delayBehavior, layer);
            }

            if (targetKind == PropertyAnimationBuilderTarget::CompositionClip)
            {
                auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    duration.value_or(AnimationExtensions::DefaultDuration()),
                    repeatOption.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);
                builder.AddCompositionAnimationFactory(CompositionClipAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

                return builder;
            }

            auto keyFrameBuilder = NormalizedKeyFrameAnimationBuilderXaml<T>(
                property,
                delay,
                duration.value_or(AnimationExtensions::DefaultDuration()),
                repeatOption.value_or(RepeatOptionHelper::Once()));

            build(keyFrameBuilder);
            builder.AddXamlAnimationFactory(XamlTransformAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

            return builder;
        }

        AnimationBuilder& TimedKeyFrames(
            std::function<void(ITimedKeyFrameAnimationBuilder<T>&)> build,
            std::optional<winrt::Windows::Foundation::TimeSpan> delay = std::nullopt,
            std::optional<RepeatOption> repeatOption = std::nullopt,
            std::optional<AnimationDelayBehavior> delayBehavior = std::nullopt) override
        {
            if (targetKind == PropertyAnimationBuilderTarget::Default)
            {
                return builder.TimedKeyFrames<T>(property, std::move(build), delay, repeatOption, delayBehavior, layer);
            }

            if (targetKind == PropertyAnimationBuilderTarget::CompositionClip)
            {
                auto keyFrameBuilder = TimedKeyFrameAnimationBuilderComposition<T>(
                    property,
                    delay,
                    repeatOption.value_or(RepeatOptionHelper::Once()),
                    delayBehavior.value_or(AnimationExtensions::DefaultDelayBehavior()));

                build(keyFrameBuilder);

                builder.AddCompositionAnimationFactory(CompositionClipAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

                return builder;
            }

            auto keyFrameBuilder = TimedKeyFrameAnimationBuilderXaml<T>(
                property,
                delay,
                repeatOption.value_or(RepeatOptionHelper::Once()));

            build(keyFrameBuilder);
            builder.AddXamlAnimationFactory(XamlTransformAnimationFactory<decltype(keyFrameBuilder)>(keyFrameBuilder));

            return builder;
        }
    };
}