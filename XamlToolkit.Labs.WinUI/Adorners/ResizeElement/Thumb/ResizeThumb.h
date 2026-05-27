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
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    using CursorEnum = winrt::Microsoft::UI::Input::InputSystemCursorShape;

	struct ResizeThumb : ResizeThumbT<ResizeThumb>
	{
		ResizeThumb();

		void OnApplyTemplate();

		void OnManipulationStarting(ManipulationStartingRoutedEventArgs const& e);

		void OnManipulationDelta(ManipulationDeltaRoutedEventArgs const& e);

        wil::typed_event<winrt::XamlToolkit::Labs::WinUI::ResizeThumb, ITargetControlResizedEventArgs> TargetControlResized;

#pragma region Properties
        ResizeDirection Direction() const;

        void Direction(ResizeDirection value);

        static void OnDirectionPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const&);

        static const wil::single_threaded_property<DependencyProperty> DirectionProperty;

        ResizePositionMode PositionMode() const;

        void PositionMode(ResizePositionMode value);

        static const wil::single_threaded_property<DependencyProperty> PositionModeProperty;

        CursorEnum Cursor() const;

        void Cursor(CursorEnum value);

        static void OnCursorPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const&);

        static const wil::single_threaded_property<DependencyProperty> CursorProperty;

        FrameworkElement TargetControl() const;

        void TargetControl(FrameworkElement const& value);

        static const wil::single_threaded_property<DependencyProperty> TargetControlProperty;

        double DragIncrement() const;

        void DragIncrement(double value);

        static const wil::single_threaded_property<DependencyProperty> DragIncrementProperty;
#pragma endregion

#pragma region Helpers
    protected:
        static bool IsValidHeight(FrameworkElement const& target, double newHeight, double parentActualHeight);

        static bool IsValidWidth(FrameworkElement const& target, double newWidth, double parentActualWidth);
#pragma endregion

	private:
		void ResizeThumb_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

        std::optional<Thickness> _originalMargin;
        std::optional<Point> _originalPosition;
        std::optional<Size> _originalSize;
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct ResizeThumb : ResizeThumbT<ResizeThumb, implementation::ResizeThumb>
	{
	};
}
