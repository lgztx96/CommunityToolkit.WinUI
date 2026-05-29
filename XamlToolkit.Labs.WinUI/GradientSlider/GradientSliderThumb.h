#pragma once

#include "GradientSliderThumb.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.h>
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
    private:
        static constexpr auto ColorPickerPartName = L"PART_ColorPicker";
        static constexpr auto BorderPartName = L"PART_Border";

        static constexpr auto CommonStatesGroupName = L"CommonStates";
        static constexpr auto NormalStateName = L"Normal";
        static constexpr auto PointerOverStateName = L"PointerOver";
        static constexpr auto PressedStateName = L"Pressed";
        static constexpr auto DisabledStateName = L"Disabled";

        Border _border{ nullptr };
        winrt::Microsoft::UI::Xaml::Controls::ColorPicker _colorPicker{ nullptr };

        bool _pointerOver{ false };
        bool _pressed{ false };
        bool _isDragging{ false };
        Point _dragStartPosition;
        Point _lastPosition;

    public:
        GradientSliderThumb();

        void OnApplyTemplate();

        winrt::Microsoft::UI::Xaml::Media::GradientStop GradientStop() const
        {
            return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::GradientStop>(GetValue(GradientStopProperty()));
        }

        void GradientStop(winrt::Microsoft::UI::Xaml::Media::GradientStop const& value)
        {
            SetValue(GradientStopProperty(), winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> GradientStopProperty =
            DependencyProperty::Register(
                L"GradientStop", 
                winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Media::GradientStop>(),
                winrt::xaml_typename<class_type>(), 
                PropertyMetadata(nullptr));

        winrt::event_token DragStarted(DragStartedEventHandler const& handler);
        void DragStarted(winrt::event_token const& token);

        winrt::event_token DragDelta(DragDeltaEventHandler const& handler);
        void DragDelta(winrt::event_token const& token);

        winrt::event_token DragCompleted(DragCompletedEventHandler const& handler);
        void DragCompleted(winrt::event_token const& token);

    private:
        void GradientSliderThumb_PointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerExited(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerMoved(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_PointerCanceled(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void GradientSliderThumb_IsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);
        void ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args);
        void GradientSliderThumb_Tapped(IInspectable const& sender, TappedRoutedEventArgs const& e);

        UIElement::PointerEntered_revoker _pointerEnteredRevoker;
        UIElement::PointerExited_revoker _pointerExitedRevoker;
        UIElement::PointerPressed_revoker _pointerPressedRevoker;
        UIElement::PointerMoved_revoker _pointerMovedRevoker;
        UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        UIElement::PointerCanceled_revoker _pointerCanceledRevoker;
        Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;
        UIElement::Tapped_revoker _tappedRevoker;

        winrt::event<DragStartedEventHandler> _dragStarted;
        winrt::event<DragDeltaEventHandler> _dragDelta;
        winrt::event<DragCompletedEventHandler> _dragCompleted;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct GradientSliderThumb : GradientSliderThumbT<GradientSliderThumb, implementation::GradientSliderThumb>
    {
    };
}
