#pragma once

#include "ImplicitAnimationSet.g.h"
#include "Interfaces/IImplicitTimeline.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <unordered_map>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ImplicitAnimationSet : ImplicitAnimationSetT<ImplicitAnimationSet>
    {
    public:
        ImplicitAnimationSet();

        winrt::weak_ref<Microsoft::UI::Xaml::UIElement> ParentReference() const;
        void ParentReference(winrt::weak_ref<Microsoft::UI::Xaml::UIElement> const& value);

        Microsoft::UI::Composition::CompositionAnimationGroup GetCompositionAnimationGroup(Microsoft::UI::Xaml::UIElement const& element);
        Microsoft::UI::Composition::ImplicitAnimationCollection GetImplicitAnimationCollection(Microsoft::UI::Xaml::UIElement const& element);

        wil::untyped_event<IInspectable> AnimationsChanged;

    private:
        winrt::weak_ref<Microsoft::UI::Xaml::UIElement> parent;
        Windows::Foundation::Collections::IObservableVector<Microsoft::UI::Xaml::DependencyObject>::VectorChanged_revoker vectorChangedRevoker;
        std::unordered_map<void*, winrt::event_token> animationPropertyChangedEventTokens;

        void OnVectorChanged(Windows::Foundation::Collections::IObservableVector<Microsoft::UI::Xaml::DependencyObject> const& sender, Windows::Foundation::Collections::IVectorChangedEventArgs const& event);
        void RaiseAnimationsChanged(IInspectable const& sender, IInspectable const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ImplicitAnimationSet : ImplicitAnimationSetT<ImplicitAnimationSet, implementation::ImplicitAnimationSet> {};
}
