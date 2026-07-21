#pragma once

#include "GradientSliderThumb.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct GradientSliderThumb : GradientSliderThumbT<GradientSliderThumb>
    {
        static constexpr auto ColorPickerPartName = L"PART_ColorPicker";
        static constexpr auto BorderPartName = L"PART_Border";

        static constexpr auto CommonStatesGroupName = L"CommonStates";
        static constexpr auto NormalStateName = L"Normal";
        static constexpr auto PointerOverStateName = L"PointerOver";
        static constexpr auto PressedStateName = L"Pressed";
        static constexpr auto DisabledStateName = L"Disabled";

        GradientSliderThumb();

        void OnApplyTemplate();

        winrt::GradientStop GradientStop() const
        {
            return GetValue(GradientStopProperty()).try_as<winrt::GradientStop>();
        }

        void GradientStop(winrt::GradientStop const& value)
        {
            SetValue(GradientStopProperty(), winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<winrt::DependencyProperty> GradientStopProperty =
            winrt::DependencyProperty::Register(
                L"GradientStop", 
                winrt::xaml_typename<winrt::GradientStop>(),
                winrt::xaml_typename<class_type>(), 
                winrt::PropertyMetadata(nullptr));

        winrt::event_token DragStarted(winrt::DragStartedEventHandler const& handler);
        void DragStarted(winrt::event_token const& token);

        winrt::event_token DragDelta(winrt::DragDeltaEventHandler const& handler);
        void DragDelta(winrt::event_token const& token);

        winrt::event_token DragCompleted(winrt::DragCompletedEventHandler const& handler);
        void DragCompleted(winrt::event_token const& token);

    private:
        void GradientSliderThumb_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerPressed(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerMoved(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerReleased(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerCanceled(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void GradientSliderThumb_IsEnabledChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e);
        void ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);
        void GradientSliderThumb_Tapped(winrt::IInspectable const& sender, winrt::TappedRoutedEventArgs const& e);

        winrt::UIElement::PointerEntered_revoker _pointerEnteredRevoker;
        winrt::UIElement::PointerExited_revoker _pointerExitedRevoker;
        winrt::UIElement::PointerPressed_revoker _pointerPressedRevoker;
        winrt::UIElement::PointerMoved_revoker _pointerMovedRevoker;
        winrt::UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        winrt::UIElement::PointerCanceled_revoker _pointerCanceledRevoker;
        winrt::Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;
        winrt::UIElement::Tapped_revoker _tappedRevoker;

        winrt::event<winrt::DragStartedEventHandler> _dragStarted;
        winrt::event<winrt::DragDeltaEventHandler> _dragDelta;
        winrt::event<winrt::DragCompletedEventHandler> _dragCompleted;

        winrt::Border _border{ nullptr };
        winrt::Microsoft::UI::Xaml::Controls::ColorPicker _colorPicker{ nullptr };

        bool _pointerOver{ false };
        bool _pressed{ false };
        bool _isDragging{ false };
        winrt::Point _dragStartPosition;
        winrt::Point _lastPosition;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct GradientSliderThumb : GradientSliderThumbT<GradientSliderThumb, implementation::GradientSliderThumb>
    {
    };
}
