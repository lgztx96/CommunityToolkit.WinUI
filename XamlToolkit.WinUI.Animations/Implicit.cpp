#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <unordered_map>
#endif
#include "Implicit.h"
#if __has_include("Implicit.g.cpp")
#include "Implicit.g.cpp"
#endif

namespace
{
    using ImplicitSet = winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet;
    using ImplicitSetImpl = winrt::XamlToolkit::WinUI::Animations::implementation::ImplicitAnimationSet;

    std::unordered_map<void*, winrt::event_token> showChangedTokens;
    std::unordered_map<void*, winrt::event_token> hideChangedTokens;
    std::unordered_map<void*, winrt::event_token> implicitChangedTokens;

    void Unsubscribe(std::unordered_map<void*, winrt::event_token>& map, ImplicitSet const& collection)
    {
        if (!collection)
        {
            return;
        }

        void* key = winrt::get_abi(collection);
        auto it = map.find(key);

        if (it != map.end())
        {
            auto impl = winrt::get_self<ImplicitSetImpl>(collection);
            impl->AnimationsChanged(it->second);
            map.erase(it);
        }
    }
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;

    const wil::single_threaded_property<DependencyProperty> Implicit::ShowAnimationsProperty = DependencyProperty::RegisterAttached(
        L"ShowAnimations",
        winrt::xaml_typename<Animations::ImplicitAnimationSet>(),
        winrt::xaml_typename<class_type>(),
        PropertyMetadata(nullptr, PropertyChangedCallback{ &Implicit::OnShowAnimationsPropertyChanged }));

    const wil::single_threaded_property<DependencyProperty> Implicit::HideAnimationsProperty = DependencyProperty::RegisterAttached(
        L"HideAnimations",
        winrt::xaml_typename<Animations::ImplicitAnimationSet>(),
        winrt::xaml_typename<class_type>(),
        PropertyMetadata(nullptr, PropertyChangedCallback{ &Implicit::OnHideAnimationsPropertyChanged }));

    const wil::single_threaded_property<DependencyProperty> Implicit::AnimationsProperty = DependencyProperty::RegisterAttached(
        L"Animations",
        winrt::xaml_typename<Animations::ImplicitAnimationSet>(),
        winrt::xaml_typename<class_type>(),
        PropertyMetadata(nullptr, PropertyChangedCallback{ &Implicit::OnAnimationsPropertyChanged }));

    Animations::ImplicitAnimationSet Implicit::GetShowAnimations(UIElement const& element)
    {
        auto value = element.GetValue(ShowAnimationsProperty());
        if (auto collection = value.try_as<Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        Animations::ImplicitAnimationSet collection;
        element.SetValue(ShowAnimationsProperty(), collection);
        return collection;
    }

    void Implicit::SetShowAnimations(UIElement const& element, Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(ShowAnimationsProperty(), value);
    }

    Animations::ImplicitAnimationSet Implicit::GetHideAnimations(UIElement const& element)
    {
        auto value = element.GetValue(HideAnimationsProperty);
        if (auto collection = value.try_as<Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        Animations::ImplicitAnimationSet collection;
        element.SetValue(HideAnimationsProperty, collection);
        return collection;
    }

    void Implicit::SetHideAnimations(UIElement const& element, Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(HideAnimationsProperty, value);
    }

    Animations::ImplicitAnimationSet Implicit::GetAnimations(UIElement const& element)
    {
        auto value = element.GetValue(AnimationsProperty);
        if (auto collection = value.try_as<Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        Animations::ImplicitAnimationSet collection;
        element.SetValue(AnimationsProperty, collection);
        return collection;
    }

    void Implicit::SetAnimations(UIElement const& element, Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(AnimationsProperty, value);
    }

    void Implicit::OnShowAnimationsPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(showChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(element);

            Unsubscribe(showChangedTokens, collection);
            auto token = impl->AnimationsChanged([=](IInspectable const& sender, IInspectable const&)
            {
                auto changed = sender.try_as<Animations::ImplicitAnimationSet>();
                if (!changed)
                {
                    return;
                }

                auto changedImpl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(changed);
                if (auto parent = changedImpl->ParentReference().get())
                {
                    ElementCompositionPreview::SetImplicitShowAnimation(parent, changedImpl->GetCompositionAnimationGroup(parent));
                }
            });
            showChangedTokens[winrt::get_abi(collection)] = token;

            ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            ElementCompositionPreview::SetImplicitShowAnimation(element, impl->GetCompositionAnimationGroup(element));
        }
        else
        {
            ElementCompositionPreview::SetImplicitShowAnimation(element, nullptr);
        }
    }

    void Implicit::OnHideAnimationsPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(hideChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(winrt::make_weak(element));

            Unsubscribe(hideChangedTokens, collection);
            auto token = impl->AnimationsChanged([=](IInspectable const& sender, IInspectable const&)
            {
                auto changed = sender.try_as<Animations::ImplicitAnimationSet>();
                if (!changed)
                {
                    return;
                }

                auto changedImpl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(changed);
                if (auto parent = changedImpl->ParentReference().get())
                {
                    ElementCompositionPreview::SetImplicitHideAnimation(parent, changedImpl->GetCompositionAnimationGroup(parent));
                }
            });
            hideChangedTokens[winrt::get_abi(collection)] = token;

            ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            ElementCompositionPreview::SetImplicitHideAnimation(element, impl->GetCompositionAnimationGroup(element));
        }
        else
        {
            ElementCompositionPreview::SetImplicitHideAnimation(element, nullptr);
        }
    }

    void Implicit::OnAnimationsPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(implicitChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(winrt::make_weak(element));

            Unsubscribe(implicitChangedTokens, collection);
            auto token = impl->AnimationsChanged([](IInspectable const& sender, IInspectable const&)
            {
                auto changed = sender.try_as<Animations::ImplicitAnimationSet>();
                if (!changed)
                {
                    return;
                }

                auto changedImpl = winrt::get_self<Animations::implementation::ImplicitAnimationSet>(changed);
                if (auto parent = changedImpl->ParentReference().get())
                {
                    auto visual = ElementCompositionPreview::GetElementVisual(parent);
                    visual.ImplicitAnimations(changedImpl->GetImplicitAnimationCollection(parent));
                }
            });
            implicitChangedTokens[winrt::get_abi(collection)] = token;

            ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            auto visual = ElementCompositionPreview::GetElementVisual(element);
            visual.ImplicitAnimations(impl->GetImplicitAnimationCollection(element));
        }
        else
        {
            auto visual = ElementCompositionPreview::GetElementVisual(element);
            visual.ImplicitAnimations(nullptr);
        }
    }
}
