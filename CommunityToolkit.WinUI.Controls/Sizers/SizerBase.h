#pragma once

#include "SizerBase.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;

    using CursorEnum = Microsoft::UI::Input::InputSystemCursorShape;

    template <typename D, typename... I>
    using SizerBaseT = SizerBase_base<D, I...>;

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

        FrameworkElement::Loaded_revoker _loadedRevoker;
        UIElement::PointerEntered_revoker _pointerEnteredRevoker;
        UIElement::PointerExited_revoker _pointerExitedRevoker;
        UIElement::PointerPressed_revoker _pointerPressedRevoker;
        UIElement::PointerReleased_revoker _pointerReleasedRevoker;
        UIElement::ManipulationStarted_revoker _manipulationStartedRevoker;
        UIElement::ManipulationCompleted_revoker _manipulationCompletedRevoker;
        Control::IsEnabledChanged_revoker _isEnabledChangedRevoker;

        bool _appliedTemplate = false;

        // private helper bools for Visual States
        bool _pressed = false;
        bool _dragging = false;
        bool _pointerEntered = false;

        virtual void OnLoaded([[maybe_unused]] RoutedEventArgs const& e) {}
        virtual void OnDragStarting() {}
        virtual bool OnDragHorizontal([[maybe_unused]] double horizontalChange) { return false; }
        virtual bool OnDragVertical([[maybe_unused]] double verticalChange) { return false; }

        SizerBase();

        void OnApplyTemplate();

        void OnKeyDown(KeyRoutedEventArgs const& e);

        void OnManipulationStarting(ManipulationStartingRoutedEventArgs const& e);

        void OnManipulationDelta(ManipulationDeltaRoutedEventArgs const& e);

        void SizerBase_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

        void SizerBase_PointerReleased([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e);

        void SizerBase_PointerPressed([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e);

        void SizerBase_PointerExited([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e);

        void SizerBase_PointerEntered([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e);

        void SizerBase_ManipulationCompleted([[maybe_unused]] IInspectable const& sender, ManipulationCompletedRoutedEventArgs const& e);

        void SizerBase_ManipulationStarted([[maybe_unused]] IInspectable const& sender, ManipulationStartedRoutedEventArgs const& e);

        void SizerBase_IsEnabledChanged([[maybe_unused]] IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);

        static bool IsValidHeight(FrameworkElement const& target, double newHeight, double parentActualHeight);

        static bool IsValidWidth(FrameworkElement const& target, double newWidth, double parentActualWidth);

        static void OnOrientationPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnIsThumbVisiblePropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        CursorEnum Cursor() { return winrt::unbox_value<CursorEnum>(GetValue(CursorProperty)); }
        void Cursor(CursorEnum value) { SetValue(CursorProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> CursorProperty =
            DependencyProperty::Register(L"Cursor", winrt::xaml_typename <CursorEnum>(),
                winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnOrientationPropertyChanged));

        double DragIncrement() { return winrt::unbox_value<double>(GetValue(DragIncrementProperty)); }
        void DragIncrement(double value) { SetValue(DragIncrementProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> DragIncrementProperty =
            DependencyProperty::Register(L"DragIncrement", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(1.0)));

        double KeyboardIncrement() { return winrt::unbox_value<double>(GetValue(KeyboardIncrementProperty)); }
        void KeyboardIncrement(double value) { SetValue(KeyboardIncrementProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> KeyboardIncrementProperty =
            DependencyProperty::Register(L"KeyboardIncrement", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(8.0)));

        Orientation Orientation() { return winrt::unbox_value<Microsoft::UI::Xaml::Controls::Orientation>(GetValue(OrientationProperty)); }
        void Orientation(Microsoft::UI::Xaml::Controls::Orientation value) { SetValue(OrientationProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> OrientationProperty =
            DependencyProperty::Register(L"Orientation", winrt::xaml_typename<Microsoft::UI::Xaml::Controls::Orientation>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(Orientation::Vertical), OnOrientationPropertyChanged));

        bool IsThumbVisible() { return winrt::unbox_value<bool>(GetValue(IsThumbVisibleProperty)); }
        void IsThumbVisible(bool value) { SetValue(IsThumbVisibleProperty, winrt::box_value(value)); }

        static inline const wil::single_threaded_property<DependencyProperty> IsThumbVisibleProperty =
            DependencyProperty::Register(L"IsThumbVisible", winrt::xaml_typename<bool>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(true), OnIsThumbVisiblePropertyChanged));
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct SizerBase : SizerBaseT<SizerBase, implementation::SizerBase>
    {
    };
}
