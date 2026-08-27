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
        if (auto it = map.find(key); it != map.end())
        {
            auto impl = winrt::get_self<ImplicitSetImpl>(collection);
            impl->AnimationsChanged(it->second);
            map.erase(it);
        }
    }
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Implicit::ShowAnimationsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ShowAnimations",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Implicit>(),
            winrt::PropertyMetadata(nullptr, &Implicit::OnShowAnimationsPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Implicit::HideAnimationsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"HideAnimations",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Implicit>(),
            winrt::PropertyMetadata(nullptr, &Implicit::OnHideAnimationsPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Implicit::AnimationsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Animations",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Implicit>(),
            winrt::PropertyMetadata(nullptr, &Implicit::OnAnimationsPropertyChanged));

    winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet Implicit::GetShowAnimations(winrt::UIElement const& element)
    {
        auto value = element.GetValue(ShowAnimationsProperty());
        if (auto collection = value.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet collection;
        element.SetValue(ShowAnimationsProperty(), collection);
        return collection;
    }

    void Implicit::SetShowAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(ShowAnimationsProperty(), value);
    }

    winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet Implicit::GetHideAnimations(winrt::UIElement const& element)
    {
        auto value = element.GetValue(HideAnimationsProperty());
        if (auto collection = value.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet collection;
        element.SetValue(HideAnimationsProperty(), collection);
        return collection;
    }

    void Implicit::SetHideAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(HideAnimationsProperty(), value);
    }

    winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet Implicit::GetAnimations(winrt::UIElement const& element)
    {
        auto value = element.GetValue(AnimationsProperty());
        if (auto collection = value.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            return collection;
        }

        winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet collection;
        element.SetValue(AnimationsProperty(), collection);
        return collection;
    }

    void Implicit::SetAnimations(winrt::UIElement const& element, winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet const& value)
    {
        element.SetValue(AnimationsProperty(), value);
    }

    void Implicit::OnShowAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(showChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(element);

            Unsubscribe(showChangedTokens, collection);
            auto token = impl->AnimationsChanged([](winrt::IInspectable const& sender, winrt::IInspectable const&) static
            {
                auto collection = sender.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>();
                if (!collection)
                {
                    return;
                }

                auto collectionImpl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
                if (auto parent = collectionImpl->ParentReference().get())
                {
                    winrt::ElementCompositionPreview::SetImplicitShowAnimation(parent, collectionImpl->GetCompositionAnimationGroup(parent));
                }
            });
            showChangedTokens[winrt::get_abi(collection)] = token;

            winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            winrt::ElementCompositionPreview::SetImplicitShowAnimation(element, impl->GetCompositionAnimationGroup(element));
        }
        else
        {
            winrt::ElementCompositionPreview::SetImplicitShowAnimation(element, nullptr);
        }
    }

    void Implicit::OnHideAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(hideChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(winrt::make_weak(element));

            Unsubscribe(hideChangedTokens, collection);
            auto token = impl->AnimationsChanged([](winrt::IInspectable const& sender, winrt::IInspectable const&) static
            {
                auto collection = sender.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>();
                if (!collection)
                {
                    return;
                }

                auto collectionImpl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
                if (auto parent = collectionImpl->ParentReference().get())
                {
                    winrt::ElementCompositionPreview::SetImplicitHideAnimation(parent, collectionImpl->GetCompositionAnimationGroup(parent));
                }
            });
            hideChangedTokens[winrt::get_abi(collection)] = token;

            winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            winrt::ElementCompositionPreview::SetImplicitHideAnimation(element, impl->GetCompositionAnimationGroup(element));
        }
        else
        {
            winrt::ElementCompositionPreview::SetImplicitHideAnimation(element, nullptr);
        }
    }

    void Implicit::OnAnimationsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::UIElement>();
        if (!element)
        {
            return;
        }

        if (auto oldCollection = e.OldValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            Unsubscribe(implicitChangedTokens, oldCollection);
        }

        if (auto collection = e.NewValue().try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>())
        {
            auto impl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
            impl->ParentReference(winrt::make_weak(element));

            Unsubscribe(implicitChangedTokens, collection);
            auto token = impl->AnimationsChanged([](winrt::IInspectable const& sender, winrt::IInspectable const&) static
            {
                auto collection = sender.try_as<winrt::XamlToolkit::WinUI::Animations::ImplicitAnimationSet>();
                if (!collection)
                {
                    return;
                }

                auto collectionImpl = winrt::get_self<implementation::ImplicitAnimationSet>(collection);
                if (auto parent = collectionImpl->ParentReference().get())
                {
                    auto visual = winrt::ElementCompositionPreview::GetElementVisual(parent);
                    visual.ImplicitAnimations(collectionImpl->GetImplicitAnimationCollection(parent));
                }
            });
            implicitChangedTokens[winrt::get_abi(collection)] = token;

            winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);
            auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
            visual.ImplicitAnimations(impl->GetImplicitAnimationCollection(element));
        }
        else
        {
            auto visual = winrt::ElementCompositionPreview::GetElementVisual(element);
            visual.ImplicitAnimations(nullptr);
        }
    }
}
