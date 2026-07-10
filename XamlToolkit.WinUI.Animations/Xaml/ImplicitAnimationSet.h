#pragma once

#include "ImplicitAnimationSet.g.h"
#include "Interfaces/IImplicitTimeline.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <unordered_map>
#endif

namespace winrt 
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Microsoft::UI::Composition;
    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    struct ImplicitAnimationSet : ImplicitAnimationSetT<ImplicitAnimationSet>
    {
    public:
        ImplicitAnimationSet();

        winrt::weak_ref<winrt::UIElement> ParentReference() const;
        void ParentReference(winrt::weak_ref<winrt::UIElement> const& value);

        winrt::CompositionAnimationGroup GetCompositionAnimationGroup(winrt::UIElement const& element);
        winrt::ImplicitAnimationCollection GetImplicitAnimationCollection(winrt::UIElement const& element);

        wil::untyped_event<winrt::IInspectable> AnimationsChanged;

    private:
        winrt::weak_ref<winrt::UIElement> parent;
        winrt::IObservableVector<winrt::DependencyObject>::VectorChanged_revoker vectorChangedRevoker;
        std::unordered_map<void*, winrt::event_token> animationPropertyChangedEventTokens;

        void OnVectorChanged(winrt::IObservableVector<winrt::DependencyObject> const& sender, winrt::IVectorChangedEventArgs const& event);
        void RaiseAnimationsChanged(winrt::IInspectable const& sender, winrt::IInspectable const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct ImplicitAnimationSet : ImplicitAnimationSetT<ImplicitAnimationSet, implementation::ImplicitAnimationSet> {};
}
