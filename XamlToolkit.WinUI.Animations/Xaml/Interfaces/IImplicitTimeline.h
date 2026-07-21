#pragma once
#include "../../Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt 
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// An interface representing a XAML model for a custom implicit composition animation.
    /// Mirrors the C# IImplicitTimeline shape for internal C++ wiring.
    /// </summary>
    class IImplicitTimeline
    {
    public:
        wil::untyped_event<winrt::IInspectable> AnimationPropertyChanged;

        /// <summary>
        /// Gets a CompositionAnimation from the current node.
        /// </summary>
        virtual winrt::CompositionAnimation GetAnimation(winrt::UIElement const& element, winrt::hstring& target) = 0;

        virtual ~IImplicitTimeline() = default;
    };
}
