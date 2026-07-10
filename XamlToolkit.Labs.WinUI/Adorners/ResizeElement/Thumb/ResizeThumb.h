#pragma once

#include "ResizeThumb.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#endif

namespace winrt 
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Input;
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    using CursorEnum = winrt::Microsoft::UI::Input::InputSystemCursorShape;

	struct ResizeThumb : ResizeThumbT<ResizeThumb>
	{
		ResizeThumb();

		void OnApplyTemplate();

		void OnManipulationStarting(winrt::ManipulationStartingRoutedEventArgs const& e);

		void OnManipulationDelta(winrt::ManipulationDeltaRoutedEventArgs const& e);

        wil::typed_event<winrt::XamlToolkit::Labs::WinUI::ResizeThumb, ITargetControlResizedEventArgs> TargetControlResized;

#pragma region Properties
        ResizeDirection Direction() const;

        void Direction(ResizeDirection value);

        static void OnDirectionPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const&);

        static const wil::single_threaded_property<winrt::DependencyProperty> DirectionProperty;

        ResizePositionMode PositionMode() const;

        void PositionMode(ResizePositionMode value);

        static const wil::single_threaded_property<winrt::DependencyProperty> PositionModeProperty;

        CursorEnum Cursor() const;

        void Cursor(CursorEnum value);

        static void OnCursorPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const&);

        static const wil::single_threaded_property<winrt::DependencyProperty> CursorProperty;

        winrt::FrameworkElement TargetControl() const;

        void TargetControl(winrt::FrameworkElement const& value);

        static const wil::single_threaded_property<winrt::DependencyProperty> TargetControlProperty;

        double DragIncrement() const;

        void DragIncrement(double value);

        static const wil::single_threaded_property<winrt::DependencyProperty> DragIncrementProperty;
#pragma endregion

#pragma region Helpers
    protected:
        static bool IsValidHeight(winrt::FrameworkElement const& target, double newHeight, double parentActualHeight);

        static bool IsValidWidth(winrt::FrameworkElement const& target, double newWidth, double parentActualWidth);
#pragma endregion

	private:
		void ResizeThumb_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        std::optional<winrt::Thickness> _originalMargin;
        std::optional<winrt::Point> _originalPosition;
        std::optional<winrt::Size> _originalSize;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct ResizeThumb : ResizeThumbT<ResizeThumb, implementation::ResizeThumb>
	{
	};
}
