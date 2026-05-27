#pragma once
#include "../../Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// An interface representing a XAML model for a custom implicit composition animation.
    /// Mirrors the C# IImplicitTimeline shape for internal C++ wiring.
    /// </summary>
    class IImplicitTimeline
    {
    public:
        wil::untyped_event<winrt::Windows::Foundation::IInspectable> AnimationPropertyChanged;

        /// <summary>
        /// Gets a CompositionAnimation from the current node.
        /// </summary>
        virtual CompositionAnimation GetAnimation(UIElement const& element, winrt::hstring& target) = 0;

        virtual ~IImplicitTimeline() = default;
    };
}
