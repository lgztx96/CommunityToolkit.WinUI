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
else
import winrt.XamlToolkit.Labs.WinUI;
#endif

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

        winrt::Canvas _containerCanvas{ nullptr };
        winrt::Thumb _placeholderThumb{ nullptr };
        winrt::Rectangle _backgroundRectangle{ nullptr };

        std::map<winrt::GradientStop, winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb> _stopThumbs;
        std::map<winrt::GradientStop, int64_t> _stopCallbacks;

        winrt::Point _dragPosition;
        winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb _draggingThumb{ nullptr };

    public:
        GradientSlider();

        void OnApplyTemplate();

        winrt::GradientStopCollection GradientStops() const;

        void GradientStops(winrt::GradientStopCollection const& value);

        bool IsAddStopsEnabled() const;

        void IsAddStopsEnabled(bool value);

        static const wil::single_threaded_property<winrt::DependencyProperty> GradientStopsProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> IsAddStopsEnabledProperty;

    private:
        static void GradientStopsChangedCallback(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        void OnThumbDragStarted(winrt::DragStartedEventArgs const& e);
        void OnThumbDragCompleted(winrt::DragCompletedEventArgs const& e);
        void OnValueChanged();

        void ContainerCanvas_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb AddStopThumb(GradientStop const& stop);
        void RemoveStopThumb(GradientStop const& stop);
        void RefreshThumbs();
        void ClearThumbs();
        void SyncThumbs();
        void SyncBackground();

        void OnGradientStopOffsetChanged(winrt::DependencyObject const& d, winrt::DependencyProperty const& e);
        void UpdateThumbPosition(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb);

        void Thumb_DragStarted(winrt::IInspectable const& sender, winrt::DragStartedEventArgs const& e);
        void Thumb_DragDelta(winrt::IInspectable const& sender, winrt::DragDeltaEventArgs const& e);
        void Thumb_DragCompleted(winrt::IInspectable const& sender, winrt::DragCompletedEventArgs const& e);
        void Thumb_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);
        void Thumb_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);
        void Thumb_RightTapped(winrt::IInspectable const& sender, winrt::RightTappedRoutedEventArgs const& e);

        void ContainerCanvas_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerMoved(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerPressed(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
        void ContainerCanvas_PointerReleased(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        bool IsPointerOverThumb(double position);
        void HandleThumbDragging(winrt::XamlToolkit::Labs::WinUI::GradientSliderThumb const& thumb, winrt::Point position);

    public:
        winrt::event_token ThumbDragStarted(winrt::DragStartedEventHandler const& handler);
        void ThumbDragStarted(winrt::event_token const& token);

        winrt::event_token ThumbDragCompleted(winrt::DragCompletedEventHandler const& handler);
        void ThumbDragCompleted(winrt::event_token const& token);

        wil::untyped_event<winrt::IInspectable> ValueChanged;

    private:
        winrt::FrameworkElement::SizeChanged_revoker _containerCanvasSizeChangedRevoker;
        winrt::UIElement::PointerEntered_revoker _containerCanvasPointerEnteredRevoker;
        winrt::UIElement::PointerMoved_revoker _containerCanvasPointerMovedRevoker;
        winrt::UIElement::PointerExited_revoker _containerCanvasPointerExitedRevoker;
        winrt::UIElement::PointerPressed_revoker _containerCanvasPointerPressedRevoker;
        winrt::UIElement::PointerReleased_revoker _containerCanvasPointerReleasedRevoker;

        winrt::event<winrt::DragStartedEventHandler> _thumbDragStarted;
        winrt::event<winrt::DragCompletedEventHandler> _thumbDragCompleted;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct GradientSlider : GradientSliderT<GradientSlider, implementation::GradientSlider>
    {
    };
}
