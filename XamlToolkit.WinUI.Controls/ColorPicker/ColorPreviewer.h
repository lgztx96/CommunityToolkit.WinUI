#pragma once

#include "Primitives.ColorPreviewer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace XamlToolkit::WinUI;
}

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::implementation
{
	struct ColorPreviewer : ColorPreviewerT<ColorPreviewer>
	{
		ColorPreviewer();

		void OnApplyTemplate();

		wil::untyped_event<winrt::XamlToolkit::WinUI::HsvColor> ColorChangeRequested;

		static const wil::single_threaded_property<winrt::DependencyProperty> HsvColorProperty;

		winrt::XamlToolkit::WinUI::HsvColor HsvColor() const
		{
			return winrt::unbox_value<winrt::XamlToolkit::WinUI::HsvColor>(GetValue(HsvColorProperty()));
		}

		void HsvColor(winrt::XamlToolkit::WinUI::HsvColor const& value)
		{
			SetValue(HsvColorProperty(), winrt::box_value(value));
		}

		static const wil::single_threaded_property<winrt::DependencyProperty> ShowAccentColorsProperty;

		bool ShowAccentColors() const
		{
			return winrt::unbox_value<bool>(GetValue(ShowAccentColorsProperty()));
		}

		void ShowAccentColors(bool value)
		{
			if (ShowAccentColors() != value)
			{
				SetValue(ShowAccentColorsProperty(), winrt::box_value(value));
			}
		}

	protected:
		virtual void OnDependencyPropertyChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		virtual void OnColorChangeRequested(winrt::XamlToolkit::WinUI::HsvColor color);

	private:
		void ConnectEvents(bool connected);

		winrt::fire_and_forget CheckeredBackgroundBorder_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void PreviewBorder_PointerPressed(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		bool eventsConnected = false;

		winrt::Border CheckeredBackgroundBorder;

		winrt::Border N1PreviewBorder;
		winrt::Border N2PreviewBorder;
		winrt::Border P1PreviewBorder;
		winrt::Border P2PreviewBorder;

		winrt::event_token checkeredLoadedToken{};
		winrt::event_token n1PointerPressedToken{};
		winrt::event_token n2PointerPressedToken{};
		winrt::event_token p1PointerPressedToken{};
		winrt::event_token p2PointerPressedToken{};
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::factory_implementation
{
	struct ColorPreviewer : ColorPreviewerT<ColorPreviewer, implementation::ColorPreviewer>
	{
	};
}
