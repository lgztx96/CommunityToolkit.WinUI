#include "pch.h"
#include "winrt_module_imports.h"
#include "TransitionHelper.h"
#include "TransitionConfig.h"
#include <algorithm>
#if __has_include("TransitionHelper.g.cpp")
#include "TransitionHelper.g.cpp"
#endif

using winrt::hstring;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Media;
using namespace winrt::Microsoft::UI::Composition;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Numerics;

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    const wil::single_threaded_property<DependencyProperty> TransitionHelper::IdProperty = DependencyProperty::RegisterAttached(L"Id", xaml_typename<hstring>(), xaml_typename<class_type>(), nullptr);
    const wil::single_threaded_property<DependencyProperty> TransitionHelper::IsIndependentProperty = DependencyProperty::RegisterAttached(L"IsIndependent", xaml_typename<bool>(), xaml_typename<class_type>(), PropertyMetadata{ box_value(false) });
    const wil::single_threaded_property<DependencyProperty> TransitionHelper::IndependentTranslationProperty = DependencyProperty::RegisterAttached(L"IndependentTranslation", xaml_typename<IReference<Point>>(), xaml_typename<class_type>(), nullptr);
    const wil::single_threaded_property<DependencyProperty> TransitionHelper::CoordinatedTargetProperty = DependencyProperty::RegisterAttached(L"CoordinatedTarget", xaml_typename<hstring>(), xaml_typename<class_type>(), nullptr);

    TransitionHelper::TransitionHelper() = default;

    hstring TransitionHelper::GetId(DependencyObject const& obj)
    {
        auto value = obj.GetValue(IdProperty()); return value ? unbox_value<hstring>(value) : hstring{};
    }
    void TransitionHelper::SetId(DependencyObject const& obj, hstring const& value)
    {
        obj.SetValue(IdProperty(), box_value(value));
    }
    bool TransitionHelper::GetIsIndependent(DependencyObject const& obj)
    {
        auto value = obj.GetValue(IsIndependentProperty()); return value ? unbox_value<bool>(value) : false;
    }
    void TransitionHelper::SetIsIndependent(DependencyObject const& obj, bool value)
    {
        obj.SetValue(IsIndependentProperty(), box_value(value));
    }
    IReference<Point> TransitionHelper::GetIndependentTranslation(DependencyObject const& obj)
    {
        return obj.GetValue(IndependentTranslationProperty()).try_as<IReference<Point>>();
    }
    void TransitionHelper::SetIndependentTranslation(DependencyObject const& obj, IReference<Point> const& value)
    {
        obj.SetValue(IndependentTranslationProperty(), value);
    }
    hstring TransitionHelper::GetCoordinatedTarget(DependencyObject const& obj)
    {
        auto value = obj.GetValue(CoordinatedTargetProperty()); return value ? unbox_value<hstring>(value) : hstring{};
    }
    void TransitionHelper::SetCoordinatedTarget(DependencyObject const& obj, hstring const& value)
    {
        obj.SetValue(CoordinatedTargetProperty(), box_value(value));
    }

    void TransitionHelper::Source(FrameworkElement const& value)
    {
        if (m_source != value)
        {
            Stop();
            m_source = value;
            m_sourceZIndex = value ? Controls::Canvas::GetZIndex(value) : 0;
            m_sourceElementsValid = false;
            m_isTargetState = false;
        }
    }
    void TransitionHelper::Target(FrameworkElement const& value)
    {
        if (m_target != value)
        {
            Stop();
            m_target = value;
            m_targetZIndex = value ? Controls::Canvas::GetZIndex(value) : 0;
            m_targetElementsValid = false;
            m_isTargetState = false;
        }
    }

    void TransitionHelper::FindElementsRecursive(DependencyObject const& root, ElementSet& result) const
    {
        auto element = root.try_as<UIElement>();
        if (element)
        {
            auto id = GetId(root);
            if (!id.empty())
            {
                result.connected.insert_or_assign(id, element); return;
            }
            auto coordinatedTarget = GetCoordinatedTarget(root);
            if (!coordinatedTarget.empty())
            {
                result.coordinated[coordinatedTarget].push_back(element); return;
            }
            if (GetIsIndependent(root))
            {
                result.independent.push_back(element); return;
            }
        }
        auto count = VisualTreeHelper::GetChildrenCount(root);
        for (int i = 0; i < count; ++i) FindElementsRecursive(VisualTreeHelper::GetChild(root, i), result);
    }

    TransitionHelper::ElementSet TransitionHelper::FindElements(DependencyObject const& root) const
    {
        ElementSet result;
        if (root) FindElementsRecursive(root, result);
        return result;
    }

    void TransitionHelper::RestoreElementSet(ElementSet const& elements) const
    {
        for (auto const& [id, element] : elements.connected)
        {
            StopVisual(element);
        }
        for (auto const& element : elements.independent)
        {
            StopVisual(element);
        }
        for (auto const& [id, coordinatedElements] : elements.coordinated)
        {
            for (auto const& element : coordinatedElements)
            {
                StopVisual(element);
            }
        }
    }

    void TransitionHelper::InitializeControl(FrameworkElement const& element) const
    {
        if (!element)
        {
            return;
        }

        element.IsHitTestVisible(m_hitTestVisible);
        if (element.Visibility() == Visibility::Collapsed)
        {
            element.Visibility(Visibility::Visible);
            element.UpdateLayout();
        }
        else if (element.Opacity() < .01)
        {
            element.Opacity(1);
        }

        auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
        if (!visual.IsVisible())
        {
            visual.IsVisible(true);
        }
    }

    CompositionEasingFunction TransitionHelper::Easing(Compositor const& compositor, winrt::XamlToolkit::WinUI::Animations::EasingType type, winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode mode)
    {
        if (type == winrt::XamlToolkit::WinUI::Animations::EasingType::Linear) return compositor.CreateLinearEasingFunction();
        auto ease = compositor.CreateCubicBezierEasingFunction({ .1f, .9f }, { .2f, 1.f });
        if (mode == winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseIn) ease = compositor.CreateCubicBezierEasingFunction({ .7f, 0.f }, { 1.f, .5f });
        else if (mode == winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseInOut) ease = compositor.CreateCubicBezierEasingFunction({ .645f, .045f }, { .355f, 1.f });
        return ease;
    }

    void TransitionHelper::StopVisual(UIElement const& element)
    {
        winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
        auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
        visual.StopAnimation(L"Translation.XY"); visual.StopAnimation(L"Scale.XY"); visual.StopAnimation(L"Opacity");
        if (auto clip = visual.Clip().try_as<InsetClip>())
        {
            clip.StopAnimation(L"LeftInset"); clip.StopAnimation(L"TopInset");
            clip.StopAnimation(L"RightInset"); clip.StopAnimation(L"BottomInset");
        }
        visual.Opacity(1); visual.Scale({ 1, 1, 1 }); visual.Clip(nullptr); visual.Properties().InsertVector3(L"Translation", {});
    }

    void TransitionHelper::SetVisible(UIElement const& element, VisualStateToggleMethod method, bool visible) const
    {
        if (!element) return;
        if (method == VisualStateToggleMethod::ByVisibility) element.Visibility(visible ? Visibility::Visible : Visibility::Collapsed);
        else winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element).IsVisible(visible);
        element.IsHitTestVisible(visible);
    }

    void TransitionHelper::AnimatePair(
        UIElement const& source,
        UIElement const& target,
        std::vector<UIElement> const& sourceCoordinated,
        std::vector<UIElement> const& targetCoordinated,
        winrt::XamlToolkit::Labs::WinUI::TransitionConfig const& config,
        bool reverse,
        TimeSpan duration)
    {
        // A reverse transition uses the currently visible target as the animation
        // source and the old source as the animation target. This keeps all
        // coordinate and scale calculations identical in both directions.
        auto animationSource = reverse ? target : source;
        auto animationTarget = reverse ? source : target;
        auto sourceFe = animationSource.try_as<FrameworkElement>(); auto targetFe = animationTarget.try_as<FrameworkElement>();
        auto sourceSize = sourceFe ? float2{ (float)sourceFe.ActualWidth(), (float)sourceFe.ActualHeight() } : animationSource.ActualSize();
        auto targetSize = targetFe ? float2{ (float)targetFe.ActualWidth(), (float)targetFe.ActualHeight() } : animationTarget.ActualSize();
        winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(animationSource, true);
        winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(animationTarget, true);
        auto sourceVisual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(animationSource); auto targetVisual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(animationTarget);
        auto center = float2{ sourceSize.x * config.NormalizedCenterPoint().X, sourceSize.y * config.NormalizedCenterPoint().Y };
        auto targetCenter = float2{ targetSize.x * config.NormalizedCenterPoint().X, targetSize.y * config.NormalizedCenterPoint().Y };
        auto point = animationTarget.TransformToVisual(animationSource).TransformPoint({});
        auto translation = float2{ point.X, point.Y } - center + targetCenter;
        auto sizeScale = float2{ targetSize.x / (sourceSize.x == 0 ? 1 : sourceSize.x), targetSize.y / (sourceSize.y == 0 ? 1 : sourceSize.y) };
        auto scale = float2{ 1, 1 };
        if (config.ScaleMode() == ScaleMode::Scale) scale = sizeScale;
        else if (config.ScaleMode() == ScaleMode::ScaleX) scale = { sizeScale.x, sizeScale.x };
        else if (config.ScaleMode() == ScaleMode::ScaleY) scale = { sizeScale.y, sizeScale.y };
        else if (config.ScaleMode() == ScaleMode::Custom && config.CustomScalingCalculator()) scale = config.CustomScalingCalculator().GetScaling(animationSource, animationTarget);
        auto easingType = config.EasingType() ? config.EasingType().Value() : m_defaultEasingType;
        auto easingMode = config.EasingMode() ? config.EasingMode().Value() : m_defaultEasingMode;
        if (reverse && m_inverseEasing)
        {
            if (easingMode == winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseIn)
                easingMode = winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseOut;
            else if (easingMode == winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseOut)
                easingMode = winrt::Microsoft::UI::Xaml::Media::Animation::EasingMode::EaseIn;
        }
        auto compositor = sourceVisual.Compositor();
        sourceVisual.CenterPoint({ center.x, center.y, 0 });
        targetVisual.CenterPoint({ targetCenter.x, targetCenter.y, 0 });
        auto translationAnimation = compositor.CreateVector2KeyFrameAnimation(); translationAnimation.Duration(duration); translationAnimation.InsertKeyFrame(0, float2{}, Easing(compositor, easingType, easingMode)); translationAnimation.InsertKeyFrame(1, translation, Easing(compositor, easingType, easingMode));
        auto scaleAnimation = compositor.CreateVector2KeyFrameAnimation(); scaleAnimation.Duration(duration); scaleAnimation.InsertKeyFrame(0, float2{ 1, 1 }, Easing(compositor, easingType, easingMode)); scaleAnimation.InsertKeyFrame(1, scale, Easing(compositor, easingType, easingMode));
        sourceVisual.StartAnimation(L"Translation.XY", translationAnimation); sourceVisual.StartAnimation(L"Scale.XY", scaleAnimation);
        auto targetTranslation = compositor.CreateVector2KeyFrameAnimation(); targetTranslation.Duration(duration); targetTranslation.InsertKeyFrame(0, -translation, Easing(compositor, easingType, easingMode)); targetTranslation.InsertKeyFrame(1, float2{}, Easing(compositor, easingType, easingMode));
        targetVisual.StartAnimation(L"Translation.XY", targetTranslation);
        auto targetScale = compositor.CreateVector2KeyFrameAnimation(); targetScale.Duration(duration);
        targetScale.InsertKeyFrame(0, float2{ 1.f / std::max(scale.x, .0001f), 1.f / std::max(scale.y, .0001f) }, Easing(compositor, easingType, easingMode));
        targetScale.InsertKeyFrame(1, float2{ 1, 1 }, Easing(compositor, easingType, easingMode));
        targetVisual.StartAnimation(L"Scale.XY", targetScale);

        auto opacityKeyReference = config.OpacityTransitionProgressKey();
        auto opacityKey = opacityKeyReference ? opacityKeyReference.Value() : m_defaultOpacityKey;
        auto key = std::clamp(reverse ? opacityKey.Y : opacityKey.X, 0.f, 1.f);
        auto sourceStart = std::clamp(key - .1f, 0.f, 1.f);
        auto sourceEnd = std::clamp(key + .1f, 0.f, 1.f);
        auto targetKey = std::clamp(key - .1f, 0.f, 1.f);
        auto targetStart = std::clamp(targetKey - .1f, 0.f, 1.f);
        auto targetEnd = std::clamp(targetKey + .1f, 0.f, 1.f);
        auto sourceOpacity = compositor.CreateScalarKeyFrameAnimation(); sourceOpacity.Duration(duration);
        sourceOpacity.InsertKeyFrame(0, 1.f); sourceOpacity.InsertKeyFrame(sourceStart, 1.f);
        sourceOpacity.InsertKeyFrame(sourceEnd, 0.f); sourceOpacity.InsertKeyFrame(1, 0.f);
        sourceVisual.StartAnimation(L"Opacity", sourceOpacity);
        auto targetOpacity = compositor.CreateScalarKeyFrameAnimation(); targetOpacity.Duration(duration);
        targetOpacity.InsertKeyFrame(0, 0.f); targetOpacity.InsertKeyFrame(targetStart, 0.f);
        targetOpacity.InsertKeyFrame(targetEnd, 1.f); targetOpacity.InsertKeyFrame(1, 1.f);
        targetVisual.StartAnimation(L"Opacity", targetOpacity);

        auto const& animationSourceCoordinated = reverse ? targetCoordinated : sourceCoordinated;
        auto const& animationTargetCoordinated = reverse ? sourceCoordinated : targetCoordinated;
        for (auto const& element : animationSourceCoordinated)
        {
            winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
            auto size = element.try_as<FrameworkElement>();
            visual.CenterPoint({ size ? static_cast<float>(size.ActualWidth() * config.NormalizedCenterPoint().X) : 0.f,
                                 size ? static_cast<float>(size.ActualHeight() * config.NormalizedCenterPoint().Y) : 0.f, 0.f });
            visual.StartAnimation(L"Translation.XY", translationAnimation);
            visual.StartAnimation(L"Scale.XY", scaleAnimation);
            visual.StartAnimation(L"Opacity", sourceOpacity);
        }
        for (auto const& element : animationTargetCoordinated)
        {
            winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
            auto size = element.try_as<FrameworkElement>();
            visual.CenterPoint({ size ? static_cast<float>(size.ActualWidth() * config.NormalizedCenterPoint().X) : 0.f,
                                 size ? static_cast<float>(size.ActualHeight() * config.NormalizedCenterPoint().Y) : 0.f, 0.f });
            visual.StartAnimation(L"Translation.XY", targetTranslation);
            visual.StartAnimation(L"Scale.XY", targetScale);
            visual.StartAnimation(L"Opacity", targetOpacity);
        }

        if (config.EnableClipAnimation())
        {
            auto animateInset = [&](Visual const& visual, float4 from, float4 to)
                {
                    auto clip = compositor.CreateInsetClip();
                    visual.Clip(clip);
                    auto start = std::array{ from.x, from.y, from.z, from.w };
                    auto end = std::array{ to.x, to.y, to.z, to.w };
                    constexpr std::array paths{ L"LeftInset", L"TopInset", L"RightInset", L"BottomInset" };
                    for (size_t i = 0; i < paths.size(); ++i)
                    {
                        auto animation = compositor.CreateScalarKeyFrameAnimation();
                        animation.Duration(duration);
                        animation.InsertKeyFrame(0, start[i], Easing(compositor, easingType, easingMode));
                        animation.InsertKeyFrame(1, end[i], Easing(compositor, easingType, easingMode));
                        clip.StartAnimation(paths[i], animation);
                    }
                };
            auto positive = [](float value)
                {
                    return std::max(value, 0.f);
                };
            auto sourceClip = float4{
                positive((sourceSize.x - targetSize.x / std::max(scale.x, .0001f)) * .5f),
                positive((sourceSize.y - targetSize.y / std::max(scale.y, .0001f)) * .5f),
                positive((sourceSize.x - targetSize.x / std::max(scale.x, .0001f)) * .5f),
                positive((sourceSize.y - targetSize.y / std::max(scale.y, .0001f)) * .5f) };
            auto targetClip = float4{
                positive((targetSize.x - sourceSize.x * scale.x) * .5f),
                positive((targetSize.y - sourceSize.y * scale.y) * .5f),
                positive((targetSize.x - sourceSize.x * scale.x) * .5f),
                positive((targetSize.y - sourceSize.y * scale.y) * .5f) };
            animateInset(sourceVisual, {}, sourceClip);
            animateInset(targetVisual, targetClip, {});
        }
    }

    void TransitionHelper::AnimateIndependent(UIElement const& element, bool show, TimeSpan delay) const
    {
        if (!element) return;

        winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
        auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
        auto compositor = visual.Compositor();
        auto translationReference = GetIndependentTranslation(element);
        auto translationPoint = translationReference ? translationReference.Value() : m_defaultTranslation;
        auto translation = float2{ translationPoint.X, translationPoint.Y };
        auto duration = show ? m_showDuration : m_hideDuration;
        auto easing = Easing(compositor, m_independentEasingType, m_independentEasingMode);

        auto translationAnimation = compositor.CreateVector2KeyFrameAnimation();
        translationAnimation.Duration(duration); translationAnimation.DelayTime(delay);
        translationAnimation.DelayBehavior(AnimationDelayBehavior::SetInitialValueBeforeDelay);
        translationAnimation.InsertKeyFrame(0, show ? translation : float2{}, easing);
        translationAnimation.InsertKeyFrame(1, show ? float2{} : translation, easing);
        visual.StartAnimation(L"Translation.XY", translationAnimation);

        auto opacityAnimation = compositor.CreateScalarKeyFrameAnimation();
        opacityAnimation.Duration(duration); opacityAnimation.DelayTime(delay);
        opacityAnimation.DelayBehavior(AnimationDelayBehavior::SetInitialValueBeforeDelay);
        opacityAnimation.InsertKeyFrame(0, show ? 0.f : 1.f, easing);
        opacityAnimation.InsertKeyFrame(1, show ? 1.f : 0.f, easing);
        visual.StartAnimation(L"Opacity", opacityAnimation);
    }

    IAsyncAction TransitionHelper::Animate(bool reverse, bool forceUpdateAnimatedElements)
    {
        winrt::apartment_context uiContext;
        if (!m_source || !m_target) throw hresult_invalid_argument(L"Source and Target must be set.");
        if (m_isAnimating)
        {
            Stop();
        }
        else if (m_isTargetState == !reverse)
        {
            co_return;
        }

        InitializeControl(m_source);
        InitializeControl(m_target);
        Controls::Canvas::SetZIndex(m_isTargetState ? m_source : m_target, std::max(m_sourceZIndex, m_targetZIndex) + 1);
        if (forceUpdateAnimatedElements || !m_sourceElementsValid)
        {
            m_sourceElements = FindElements(m_source);
            m_sourceElementsValid = true;
        }
        if (forceUpdateAnimatedElements || !m_targetElementsValid)
        {
            m_targetElements = FindElements(m_target);
            m_targetElementsValid = true;
        }

        auto generation = ++m_animationGeneration;
        m_isAnimating = true;
        auto duration = reverse ? m_reverseDuration : m_duration;
        auto compositor = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(m_source).Compositor();
        wil::shared_event completionEvent(wil::EventOptions::ManualReset);
        auto batch = compositor.CreateScopedBatch(CompositionBatchTypes::Animation);
        batch.Completed([completionEvent](auto&&, auto&&)
                        {
                            completionEvent.SetEvent();
                        });

        for (auto const& [id, source] : m_sourceElements.connected)
        {
            auto target = m_targetElements.connected.find(id);
            if (target != m_targetElements.connected.end())
            {
                winrt::XamlToolkit::Labs::WinUI::TransitionConfig config{ nullptr };
                for (auto const& item : m_configs) if (item.Id() == id)
                {
                    config = item; break;
                }
                if (!config) config = winrt::make<implementation::TransitionConfig>();
                static const std::vector<UIElement> empty;
                auto sourceCoordinated = m_sourceElements.coordinated.find(id);
                auto targetCoordinated = m_targetElements.coordinated.find(id);
                AnimatePair(
                    source,
                    target->second,
                    sourceCoordinated == m_sourceElements.coordinated.end() ? empty : sourceCoordinated->second,
                    targetCoordinated == m_targetElements.coordinated.end() ? empty : targetCoordinated->second,
                    config,
                    reverse,
                    duration);
            }
        }

        std::vector<UIElement> sourceIndependent = m_sourceElements.independent;
        std::vector<UIElement> targetIndependent = m_targetElements.independent;
        for (auto const& [id, element] : m_sourceElements.connected)
        {
            if (!m_targetElements.connected.contains(id))
            {
                sourceIndependent.push_back(element);
                if (auto coordinated = m_sourceElements.coordinated.find(id); coordinated != m_sourceElements.coordinated.end())
                {
                    sourceIndependent.insert(sourceIndependent.end(), coordinated->second.begin(), coordinated->second.end());
                }
            }
        }
        for (auto const& [id, element] : m_targetElements.connected)
        {
            if (!m_sourceElements.connected.contains(id))
            {
                targetIndependent.push_back(element);
                if (auto coordinated = m_targetElements.coordinated.find(id); coordinated != m_targetElements.coordinated.end())
                {
                    targetIndependent.insert(targetIndependent.end(), coordinated->second.begin(), coordinated->second.end());
                }
            }
        }

        auto animateIndependentSet = [this](std::vector<UIElement> const& elements, bool show)
            {
                auto delay = show ? m_showDelay : TimeSpan{};
                for (auto const& element : elements)
                {
                    AnimateIndependent(element, show, delay);
                    if (show) delay += m_showInterval;
                }
            };
        animateIndependentSet(sourceIndependent, reverse);
        animateIndependentSet(targetIndependent, !reverse);

        batch.End();
        co_await winrt::resume_on_signal(completionEvent.get());
        co_await uiContext;
        if (generation != m_animationGeneration)
        {
            co_return;
        }

        m_isAnimating = false;
        m_isTargetState = !reverse;
        Controls::Canvas::SetZIndex(m_source, m_sourceZIndex);
        Controls::Canvas::SetZIndex(m_target, m_targetZIndex);
        RestoreElementSet(m_sourceElements);
        RestoreElementSet(m_targetElements);
        SetVisible(reverse ? m_source : m_target, reverse ? m_sourceToggle : m_targetToggle, true);
        SetVisible(reverse ? m_target : m_source, reverse ? m_targetToggle : m_sourceToggle, false);
    }

    IAsyncAction TransitionHelper::StartAsync()
    {
        co_await Animate(false, false);
    }
    IAsyncAction TransitionHelper::StartAsync(bool forceUpdateAnimatedElements)
    {
        co_await Animate(false, forceUpdateAnimatedElements);
    }
    IAsyncAction TransitionHelper::ReverseAsync()
    {
        co_await Animate(true, false);
    }
    IAsyncAction TransitionHelper::ReverseAsync(bool forceUpdateAnimatedElements)
    {
        co_await Animate(true, forceUpdateAnimatedElements);
    }
    void TransitionHelper::Stop()
    {
        if (!m_isAnimating)
        {
            return;
        }

        ++m_animationGeneration;
        RestoreElementSet(m_sourceElements);
        RestoreElementSet(m_targetElements);
        m_isAnimating = false;
    }
    void TransitionHelper::Reset(bool toInitialState)
    {
        Stop();
        RestoreElementSet(m_sourceElements);
        RestoreElementSet(m_targetElements);
        m_isTargetState = !toInitialState;
        if (m_source) Controls::Canvas::SetZIndex(m_source, m_sourceZIndex);
        if (m_target) Controls::Canvas::SetZIndex(m_target, m_targetZIndex);
        if (m_source) SetVisible(m_source, m_sourceToggle, toInitialState);
        if (m_target) SetVisible(m_target, m_targetToggle, !toInitialState);
    }
}
