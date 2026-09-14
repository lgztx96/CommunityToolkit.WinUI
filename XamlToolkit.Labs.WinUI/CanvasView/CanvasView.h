#pragma once

#include "CanvasView.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Input;
import winrt.Microsoft.UI.Xaml.Controls;
#endif

namespace winrt
{
    using namespace ::winrt::Windows::Foundation;
    using namespace ::winrt::Microsoft::UI::Xaml;
    using namespace ::winrt::Microsoft::UI::Xaml::Input;
    using namespace ::winrt::Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct CanvasView : CanvasViewT<CanvasView>
    {
        /// <summary>
        /// Gets the set of properties that will be automatically bound to the root element within the <see cref="ContentPresenter"/> template to the containing <see cref="ContentPresenter"/> itself within the <see cref="CanvasView"/>. This allows for data binding these properties to a templated object for tracking position based properties.
        /// </summary>
        static inline std::array<std::pair<winrt::DependencyProperty, winrt::hstring>, 4> const lifted_properties
        {
            {
                { winrt::Canvas::LeftProperty(), L"(Canvas.Left)" },
                { winrt::Canvas::TopProperty(), L"(Canvas.Top)" },
                { winrt::Canvas::ZIndexProperty(), L"(Canvas.ZIndex)" },
                { winrt::UIElement::ManipulationModeProperty(), L"ManipulationMode" },
            }
        };

        /// <summary>
        /// Initializes a new instance of the <see cref="CanvasView"/> class.
        /// </summary>
        CanvasView();

        winrt::DependencyObject GetContainerForItemOverride();

        bool IsItemItsOwnContainerOverride(winrt::IInspectable const& item);

        void PrepareContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item);

        void ClearContainerForItemOverride(winrt::DependencyObject const& element, winrt::IInspectable const& item);

    private:
        void ContentPresenter_ManipulationDelta(winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e);

        static void SetupChildBinding(winrt::ContentPresenter const& cp);

        static void SetBindingExpressionValue(
            winrt::FrameworkElement const& fe,
            winrt::DependencyProperty const& property,
            winrt::IInspectable const& value);

        std::map<winrt::ContentPresenter, winrt::event_token> _manipulationDeltaTokens;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct CanvasView : CanvasViewT<CanvasView, implementation::CanvasView>
    {
    };
}
