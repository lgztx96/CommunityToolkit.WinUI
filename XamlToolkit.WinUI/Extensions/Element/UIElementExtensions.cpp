#include "pch.h"
#include "UIElementExtensions.h"
#if __has_include("UIElementExtensions.g.cpp")
#include "UIElementExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UIElementExtensions::ClipToBoundsProperty =
        winrt::DependencyProperty::RegisterAttached(L"ClipToBounds",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<XamlToolkit::WinUI::UIElementExtensions>(),
            winrt::PropertyMetadata(winrt::box_value(false), &UIElementExtensions::OnClipToBoundsPropertyChanged));

    void UIElementExtensions::OnClipToBoundsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<winrt::UIElement>())
        {
            auto clipToBounds = winrt::unbox_value<bool>(e.NewValue());
            auto visual = winrt::Microsoft::UI::Xaml::Hosting::ElementCompositionPreview::GetElementVisual(element);
            visual.Clip(clipToBounds ? visual.Compositor().CreateInsetClip() : nullptr);
        }
    }

    bool UIElementExtensions::GetClipToBounds(winrt::UIElement const& element) 
    { 
        return winrt::unbox_value<bool>(element.GetValue(ClipToBoundsProperty()));
    }

    void UIElementExtensions::SetClipToBounds(winrt::UIElement const& element, bool value) 
    { 
        element.SetValue(ClipToBoundsProperty(), winrt::box_value(value));
    }

    winrt::Point UIElementExtensions::CoordinatesFrom(winrt::UIElement const& target, winrt::UIElement const& parent)
    {
        return target.TransformToVisual(parent).TransformPoint({ 0.0f, 0.0f });
    }

    winrt::Point UIElementExtensions::CoordinatesTo(winrt::UIElement const& parent, winrt::UIElement const& target)
    {
        return target.TransformToVisual(parent).TransformPoint({ 0.0f, 0.0f });
    }
}
