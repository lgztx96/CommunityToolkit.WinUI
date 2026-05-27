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
	public:
		wil::untyped_event<winrt::XamlToolkit::WinUI::HsvColor> ColorChangeRequested;

	private:
		bool eventsConnected = false;

		Border CheckeredBackgroundBorder;

		Border N1PreviewBorder;
		Border N2PreviewBorder;
		Border P1PreviewBorder;
		Border P2PreviewBorder;

		winrt::event_token checkeredLoadedToken{};
		winrt::event_token n1PointerPressedToken{};
		winrt::event_token n2PointerPressedToken{};
		winrt::event_token p1PointerPressedToken{};
		winrt::event_token p2PointerPressedToken{};

	public:

		static inline const wil::single_threaded_property<DependencyProperty> HsvColorProperty =
			DependencyProperty::Register(
				L"HsvColor",
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::HsvColor>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHsv(Colors::Transparent())),
					[](auto& s, auto& e)
					{
						auto self = winrt::get_self<ColorPreviewer>(s.template as<class_type>())->get_strong();
						self->OnDependencyPropertyChanged(s, e);
					}));

		winrt::XamlToolkit::WinUI::HsvColor HsvColor()
		{
			return winrt::unbox_value<winrt::XamlToolkit::WinUI::HsvColor>(GetValue(HsvColorProperty));
		}

		void HsvColor(winrt::XamlToolkit::WinUI::HsvColor const& value)
		{
			SetValue(HsvColorProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<DependencyProperty> ShowAccentColorsProperty =
			DependencyProperty::Register(
				L"ShowAccentColors",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(true),
					[](auto& s, auto& e)
					{
						auto self = winrt::get_self<ColorPreviewer>(s.template as<class_type>())->get_strong();
						self->OnDependencyPropertyChanged(s, e);
					}));

		bool ShowAccentColors()
		{
			return winrt::unbox_value<bool>(GetValue(ShowAccentColorsProperty));
		}

		void ShowAccentColors(bool value)
		{
			auto current = winrt::unbox_value<bool>(GetValue(ShowAccentColorsProperty));
			if (current != value)
			{
				SetValue(ShowAccentColorsProperty, winrt::box_value(value));
			}
		}

		ColorPreviewer();

		void OnApplyTemplate();

	private:
		void ConnectEvents(bool connected);

	protected:
		virtual void OnDependencyPropertyChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& args);

		virtual void OnColorChangeRequested(winrt::XamlToolkit::WinUI::HsvColor color);

	private:
		winrt::Windows::Foundation::IAsyncAction CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void PreviewBorder_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::factory_implementation
{
	struct ColorPreviewer : ColorPreviewerT<ColorPreviewer, implementation::ColorPreviewer>
	{
	};
}
