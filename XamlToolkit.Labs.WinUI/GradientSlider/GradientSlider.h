#pragma once

#include "GradientSlider.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#include <map>
#endif

import winrt.XamlToolkit.Labs.WinUI;

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::System;
    using namespace Microsoft::UI;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Shapes;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct GradientSlider : GradientSliderT<GradientSlider>
    {
    private:
        static constexpr auto ContainerCanvasPartName = L"ContainerCanvas";
        static constexpr auto PlaceholderThumbPartName = L"PlaceholderThumb";
        static constexpr auto BackgroundRectanglePartName = L"BackgroundRectangle";

        static constexpr auto CommonStatesGroupName = L"CommonStates";
        static constexpr auto NormalStateName = L"Normal";
        static constexpr auto PointerOverStateName = L"PointerOver";
        static constexpr auto DisabledStateName = L"Disabled";

        Canvas _containerCanvas{ nullptr };
        Thumb _placeholderThumb{ nullptr };
        Rectangle _backgroundRectangle{ nullptr };

        std::map<GradientStop, winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb> _stopThumbs;
        std::map<GradientStop, int64_t> _stopCallbacks;

        Point _dragPosition;
        winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb _draggingThumb{ nullptr };

    public:
        GradientSlider();

        void OnApplyTemplate();

        GradientStopCollection GradientStops() const;

        void GradientStops(GradientStopCollection const& value);

        bool IsAddStopsEnabled() const;

        void IsAddStopsEnabled(bool value);

        static const wil::single_threaded_property<DependencyProperty> GradientStopsProperty;

        static const wil::single_threaded_property<DependencyProperty> IsAddStopsEnabledProperty;

    private:
        static void GradientStopsChangedCallback(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        void OnThumbDragStarted(DragStartedEventArgs const& e);
        void OnThumbDragCompleted(DragCompletedEventArgs const& e);
        void OnValueChanged();

        void ContainerCanvas_SizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

        winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb AddStopThumb(GradientStop const& stop);
        void RemoveStopThumb(GradientStop const& stop);
        void RefreshThumbs();
        void ClearThumbs();
        void SyncThumbs();
        void SyncBackground();

        void OnGradientStopOffsetChanged(DependencyObject const& d, DependencyProperty const& e);
        void UpdateThumbPosition(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb);

        void Thumb_DragStarted(winrt::IInspectable const& sender, DragStartedEventArgs const& e);
        void Thumb_DragDelta(winrt::IInspectable const& sender, DragDeltaEventArgs const& e);
        void Thumb_DragCompleted(winrt::IInspectable const& sender, DragCompletedEventArgs const& e);
        void Thumb_KeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);
        void Thumb_Loaded(IInspectable const& sender, RoutedEventArgs const& e);
        void Thumb_RightTapped(IInspectable const& sender, RightTappedRoutedEventArgs const& e);

        void ContainerCanvas_PointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerMoved(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerExited(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);

        bool IsPointerOverThumb(double position);
        void HandleThumbDragging(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb, Point position);

    public:
        winrt::event_token ThumbDragStarted(DragStartedEventHandler const& handler);
        void ThumbDragStarted(winrt::event_token const& token);

        winrt::event_token ThumbDragCompleted(DragCompletedEventHandler const& handler);
        void ThumbDragCompleted(winrt::event_token const& token);

        wil::untyped_event<IInspectable> ValueChanged;

    private:
        FrameworkElement::SizeChanged_revoker _containerCanvasSizeChangedRevoker;
        UIElement::PointerEntered_revoker _containerCanvasPointerEnteredRevoker;
        UIElement::PointerMoved_revoker _containerCanvasPointerMovedRevoker;
        UIElement::PointerExited_revoker _containerCanvasPointerExitedRevoker;
        UIElement::PointerPressed_revoker _containerCanvasPointerPressedRevoker;
        UIElement::PointerReleased_revoker _containerCanvasPointerReleasedRevoker;

        winrt::event<DragStartedEventHandler> _thumbDragStarted;
        winrt::event<DragCompletedEventHandler> _thumbDragCompleted;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct GradientSlider : GradientSliderT<GradientSlider, implementation::GradientSlider>
    {
    };
}
