#include "pch.h"
#include "winrt_module_imports.h"
#include "ImplicitOffsetAnimationHelper.h"
#if __has_include("ImplicitOffsetAnimationHelper.g.cpp")
#include "ImplicitOffsetAnimationHelper.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    void ImplicitOffsetAnimationHelper::OnEnableOffsetAnimationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<FrameworkElement>())
        {
            if (auto newValue = e.NewValue().try_as<bool>(); newValue && *newValue)
            {

                element.Loaded([element = element](auto&&, auto&&)
                {
                    auto visual = Hosting::ElementCompositionPreview::GetElementVisual(element);
                    auto compositor = visual.Compositor();

                    auto animation = compositor.CreateVector3KeyFrameAnimation();
                    animation.Target(L"Offset");
                    animation.Duration(std::chrono::milliseconds(400));
                    animation.InsertExpressionKeyFrame(1.0f, L"this.FinalValue");

                    auto implicitAnimations = compositor.CreateImplicitAnimationCollection();
                    implicitAnimations.Insert(L"Offset", animation);

                    visual.ImplicitAnimations(implicitAnimations);
                });
            }
        }
    }

    bool ImplicitOffsetAnimationHelper::GetEnableOffsetAnimation(winrt::UIElement const& element) noexcept
    {
        winrt::unbox_value<bool>(element.GetValue(EnableOffsetAnimationProperty));
    }

    void ImplicitOffsetAnimationHelper::SetEnableOffsetAnimation(winrt::UIElement const& element, bool value)
    {
        element.SetValue(EnableOffsetAnimationProperty, winrt::box_value(value));
    }
}
