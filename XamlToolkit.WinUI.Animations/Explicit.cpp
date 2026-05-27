#include "pch.h"
#include "winrt_module_imports.h"
#include "Explicit.h"
#if __has_include("Explicit.g.cpp")
#include "Explicit.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Explicit::AnimationsProperty = DependencyProperty::RegisterAttached(
        L"Animations",
        winrt::xaml_typename<winrt::AnimationDictionary>(),
        winrt::xaml_typename<class_type>(),
        winrt::PropertyMetadata(nullptr, winrt::PropertyChangedCallback{ &Explicit::OnAnimationsPropertyChanged }));

    winrt::AnimationDictionary Explicit::GetAnimations(winrt::UIElement const& element)
    {
        auto value = element.GetValue(AnimationsProperty());
        if (auto collection = value.try_as<winrt::AnimationDictionary>())
        {
            return collection;
        }

        winrt::AnimationDictionary collection;
        element.SetValue(AnimationsProperty(), collection);
        return collection;
    }

    void Explicit::SetAnimations(winrt::UIElement const& element, winrt::AnimationDictionary const& value)
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

        if (auto oldDictionary = e.OldValue().try_as<winrt::AnimationDictionary>())
        {
            auto impl = winrt::get_self<implementation::AnimationDictionary>(oldDictionary);
            impl->Parent(nullptr);
        }

        if (auto newDictionary = e.NewValue().try_as<winrt::AnimationDictionary>())
        {
            auto impl = winrt::get_self<implementation::AnimationDictionary>(newDictionary);
            impl->Parent(element);
        }
    }
}
