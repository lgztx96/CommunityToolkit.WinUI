#pragma once

#include "Primitives.ColorPreviewer.g.h"
#include <winrt/CommunityToolkit.WinUI.Helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::Primitives::implementation
{
	using namespace winrt::Windows::UI;
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Input;

	struct ColorPreviewer : ColorPreviewerT<ColorPreviewer>
	{
	public:
		wil::untyped_event<winrt::CommunityToolkit::WinUI::HsvColor> ColorChangeRequested;

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
				winrt::xaml_typename<winrt::CommunityToolkit::WinUI::HsvColor>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(winrt::box_value(winrt::CommunityToolkit::WinUI::Helpers::ColorHelper::ToHsv(Colors::Transparent())),
					[](auto& s, auto& e)
					{
						auto self = winrt::get_self<ColorPreviewer>(s.template as<class_type>())->get_strong();
						self->OnDependencyPropertyChanged(s, e);
					}));

		winrt::CommunityToolkit::WinUI::HsvColor HsvColor()
		{
			return winrt::unbox_value<winrt::CommunityToolkit::WinUI::HsvColor>(GetValue(HsvColorProperty));
		}

		void HsvColor(winrt::CommunityToolkit::WinUI::HsvColor const& value)
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

		virtual void OnColorChangeRequested(winrt::CommunityToolkit::WinUI::HsvColor color);

	private:
		winrt::Windows::Foundation::IAsyncAction CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

		void PreviewBorder_PointerPressed(IInspectable const& sender, PointerRoutedEventArgs const& e);
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::Primitives::factory_implementation
{
	struct ColorPreviewer : ColorPreviewerT<ColorPreviewer, implementation::ColorPreviewer>
	{
	};
}
