#include "pch.h"
#include "winrt_module_imports.h"
#include "Explicit.h"
#if __has_include("Explicit.g.cpp")
#include "Explicit.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Explicit::AnimationsProperty = 
        winrt::DependencyProperty::RegisterAttached(
            L"Animations",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::AnimationDictionary>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Explicit>(),
            winrt::PropertyMetadata(nullptr, &Explicit::OnAnimationsPropertyChanged));

    winrt::XamlToolkit::WinUI::Animations::AnimationDictionary Explicit::GetAnimations(winrt::UIElement const& element)
    {
        auto value = element.GetValue(AnimationsProperty());
        if (auto collection = value.try_as<winrt::XamlToolkit::WinUI::Animations::AnimationDictionary>())
        {
            return collection;
        }

        winrt::XamlToolkit::WinUI::Animations::AnimationDictionary collection;
        element.SetValue(AnimationsProperty(), collection);
        return collection;
    }

    void Explicit::SetAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::AnimationDictionary const& value)
    {
        element.SetValue(AnimationsProperty(), value);
    }

    void Explicit::OnAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldDictionary = e.OldValue().try_as<winrt::XamlToolkit::WinUI::Animations::AnimationDictionary>())
        {
            auto impl = winrt::get_self<implementation::AnimationDictionary>(oldDictionary);
            impl->Parent(nullptr);
        }

        if (auto newDictionary = e.NewValue().try_as<winrt::XamlToolkit::WinUI::Animations::AnimationDictionary>())
        {
            auto impl = winrt::get_self<implementation::AnimationDictionary>(newDictionary);
            impl->Parent(element);
        }
    }
}
