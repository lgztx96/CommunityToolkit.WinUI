#include "pch.h"
#include "UIElementExtensions.h"
#if __has_include("UIElementExtensions.g.cpp")
#include "UIElementExtensions.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
    void UIElementExtensions::OnClipToBoundsPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<UIElement>())
        {
            auto clipToBounds = winrt::unbox_value<bool>(e.NewValue());
            auto visual = Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
            visual.Clip(clipToBounds ? visual.Compositor().CreateInsetClip() : nullptr);
        }
    }

    bool UIElementExtensions::GetClipToBounds(UIElement const& element) { return winrt::unbox_value<bool>(element.GetValue(ClipToBoundsProperty)); }

    void UIElementExtensions::SetClipToBounds(UIElement const& element, bool value) { element.SetValue(ClipToBoundsProperty, winrt::box_value(value)); }

    Windows::Foundation::Point UIElementExtensions::CoordinatesFrom(UIElement const& target, UIElement const& parent)
    {
        return target.TransformToVisual(parent).TransformPoint({ 0.0f, 0.0f });
    }

    Windows::Foundation::Point UIElementExtensions::CoordinatesTo(UIElement const& parent, UIElement const& target)
    {
        return target.TransformToVisual(parent).TransformPoint({ 0.0f, 0.0f });
    }
}
