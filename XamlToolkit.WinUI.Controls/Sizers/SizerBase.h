#pragma once

#include "SizerBase.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    using CursorEnum = Microsoft::UI::Input::InputSystemCursorShape;

    struct SizerBase : SizerBaseT<SizerBase>
    {
        static constexpr auto CommonStates = L"CommonStates";
        static constexpr auto NormalState = L"Normal";
        static constexpr auto PointerOverState = L"PointerOver";
        static constexpr auto PressedState = L"Pressed";
        static constexpr auto DisabledState = L"Disabled";
        static constexpr auto OrientationStates = L"OrientationStates";
        static constexpr auto HorizontalState = L"Horizontal";
        static constexpr auto VerticalState = L"Vertical";
        static constexpr auto ThumbVisibilityStates = L"ThumbVisibilityStates";
        static constexpr auto VisibleState = L"Visible";
        static constexpr auto CollapsedState = L"Collapsed";

        SizerBase();

        virtual void OnLoaded([[maybe_unused]] winrt::RoutedEventArgs const& e) {}

        virtual void OnDragStarting() {}

        virtual bool OnDragHorizontal([[maybe_unused]] double horizontalChange) { return false; }

        virtual bool OnDragVertical([[maybe_unused]] double verticalChange) { return false; }

        void OnApplyTemplate();

        void OnKeyDown(winrt::KeyRoutedEventArgs const& e);

        void OnManipulationStarting(winrt::ManipulationStartingRoutedEventArgs const& e);

        void OnManipulationDelta(winrt::ManipulationDeltaRoutedEventArgs const& e);

        void SizerBase_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        void SizerBase_PointerReleased([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void SizerBase_PointerPressed([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void SizerBase_PointerExited([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void SizerBase_PointerEntered([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void SizerBase_ManipulationCompleted([[maybe_unused]] winrt::IInspectable const& sender, winrt::ManipulationCompletedRoutedEventArgs const& e);

        void SizerBase_ManipulationStarted([[maybe_unused]] winrt::IInspectable const& sender, winrt::ManipulationStartedRoutedEventArgs const& e);

        void SizerBase_IsEnabledChanged([[maybe_unused]] winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

        static bool IsValidHeight(winrt::FrameworkElement const& target, double newHeight, double parentActualHeight);

        static bool IsValidWidth(winrt::FrameworkElement const& target, double newWidth, double parentActualWidth);

        static void OnOrientationPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnIsThumbVisiblePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        CursorEnum Cursor() const { return winrt::unbox_value<CursorEnum>(GetValue(CursorProperty)); }
        void Cursor(CursorEnum const& value) { SetValue(CursorProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> CursorProperty;

        double DragIncrement() const { return winrt::unbox_value<double>(GetValue(DragIncrementProperty)); }
        void DragIncrement(double value) { SetValue(DragIncrementProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> DragIncrementProperty;

        double KeyboardIncrement() const { return winrt::unbox_value<double>(GetValue(KeyboardIncrementProperty)); }
        void KeyboardIncrement(double value) { SetValue(KeyboardIncrementProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> KeyboardIncrementProperty;

        winrt::Orientation Orientation() const { return winrt::unbox_value<winrt::Orientation>(GetValue(OrientationProperty)); }
        void Orientation(winrt::Orientation const& value) { SetValue(OrientationProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> OrientationProperty;

        bool IsThumbVisible() const { return winrt::unbox_value<bool>(GetValue(IsThumbVisibleProperty)); }
        void IsThumbVisible(bool value) { SetValue(IsThumbVisibleProperty, winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> IsThumbVisibleProperty;

    private:
        winrt::FrameworkElement::Loaded_revoker _loadedRevoker;
        winrt::UIElement::PointerEntered_revoker _pointerEnteredRevoker;
        winrt::UIElement::PointerExited_revoker _pointerExitedRevoker;
        winrt::UIElement::PointerPressed_revoker _pointerPressedRevoker;
        winrt::UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        winrt::UIElement::ManipulationStarted_revoker _manipulationStartedRevoker;
        winrt::UIElement::ManipulationCompleted_revoker _manipulationCompletedRevoker;
        winrt::Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;

        bool _appliedTemplate = false;

        // private helper bools for Visual States
        bool _pressed = false;
        bool _dragging = false;
        bool _pointerEntered = false;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SizerBase : SizerBaseT<SizerBase, implementation::SizerBase>
    {
    };
}
