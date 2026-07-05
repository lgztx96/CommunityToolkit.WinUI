#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <string>
#endif
#include "ColorPreviewer.h"
#if __has_include("Primitives.ColorPreviewer.g.cpp")
#include "Primitives.ColorPreviewer.g.cpp"
#endif
#include "ColorPickerRenderingHelpers.h"
#include "AccentColorConverter.h"

namespace winrt::XamlToolkit::WinUI::Controls::Primitives::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> ColorPreviewer::HsvColorProperty =
		winrt::DependencyProperty::Register(
			L"HsvColor",
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::HsvColor>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::Helpers::ColorHelper::ToHsv(winrt::Microsoft::UI::Colors::Transparent())),
			[](auto& s, auto& e)
			{
				auto self = winrt::get_self<ColorPreviewer>(s.template as<class_type>())->get_strong();
				self->OnDependencyPropertyChanged(s, e);
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> ColorPreviewer::ShowAccentColorsProperty =
		winrt::DependencyProperty::Register(
			L"ShowAccentColors",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true),
			[](auto& s, auto& e)
			{
				auto self = winrt::get_self<ColorPreviewer>(s.template as<class_type>())->get_strong();
				self->OnDependencyPropertyChanged(s, e);
			}));

	ColorPreviewer::ColorPreviewer()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void ColorPreviewer::ConnectEvents(bool connected)
	{
		if (connected && !eventsConnected)
		{
			// Add all events
			if (CheckeredBackgroundBorder) 
			{
				checkeredLoadedToken = CheckeredBackgroundBorder.Loaded({ this, &ColorPreviewer::CheckeredBackgroundBorder_Loaded });
			}

			if (N1PreviewBorder) 
			{
				n1PointerPressedToken = N1PreviewBorder.PointerPressed({ this, &ColorPreviewer::PreviewBorder_PointerPressed });
			}

			if (N2PreviewBorder) 
			{
				n2PointerPressedToken = N2PreviewBorder.PointerPressed({ this,&ColorPreviewer::PreviewBorder_PointerPressed });
			}

			if (P1PreviewBorder) 
			{
				p1PointerPressedToken = P1PreviewBorder.PointerPressed({ this,&ColorPreviewer::PreviewBorder_PointerPressed });
			}

			if (P2PreviewBorder) 
			{
				p2PointerPressedToken = P2PreviewBorder.PointerPressed({ this, &ColorPreviewer::PreviewBorder_PointerPressed });
			}

			eventsConnected = true;
		}
		else if (!connected && eventsConnected)
		{
			// Remove all events
			if (CheckeredBackgroundBorder) { CheckeredBackgroundBorder.Loaded(checkeredLoadedToken); }

			if (N1PreviewBorder) { N1PreviewBorder.PointerPressed(n1PointerPressedToken); }
			if (N2PreviewBorder) { N2PreviewBorder.PointerPressed(n2PointerPressedToken); }
			if (P1PreviewBorder) { P1PreviewBorder.PointerPressed(p1PointerPressedToken); }
			if (P2PreviewBorder) { P2PreviewBorder.PointerPressed(p2PointerPressedToken); }

			eventsConnected = false;
		}
	}

	void ColorPreviewer::OnApplyTemplate()
	{
		// Remove any existing events present if the control was previously loaded then unloaded
		ConnectEvents(false);

		CheckeredBackgroundBorder = GetTemplateChild(L"CheckeredBackgroundBorder").try_as<winrt::Border>();

		N1PreviewBorder = GetTemplateChild(L"N1PreviewBorder").try_as<winrt::Border>();
		N2PreviewBorder = GetTemplateChild(L"N2PreviewBorder").try_as<winrt::Border>();
		P1PreviewBorder = GetTemplateChild(L"P1PreviewBorder").try_as<winrt::Border>();
		P2PreviewBorder = GetTemplateChild(L"P2PreviewBorder").try_as<winrt::Border>();

		// Must connect after controls are resolved
		ConnectEvents(true);

		base_type::OnApplyTemplate();
	}

	void ColorPreviewer::OnDependencyPropertyChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		winrt::VisualStateManager::GoToState(*this, ShowAccentColors() ? L"AccentColorsVisible" : L"AccentColorsCollapsed", true);
	}

	void ColorPreviewer::OnColorChangeRequested(winrt::XamlToolkit::WinUI::HsvColor color)
	{
		ColorChangeRequested.invoke(*this, color);
	}

	winrt::fire_and_forget ColorPreviewer::CheckeredBackgroundBorder_Loaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
	{
		if (const auto border = sender.try_as<winrt::Border>())
		{
			const int width = static_cast<int>(border.ActualWidth());
			const int height = static_cast<int>(border.ActualHeight());

			const auto& bitmap = co_await ColorPickerRenderingHelpers::CreateCheckeredBitmapAsync(
				width,
				height,
				ColorPickerRenderingHelpers::CheckerBackgroundColor());

			if (bitmap)
			{
				border.Background(ColorPickerRenderingHelpers::BitmapToBrush(bitmap, width, height));
			}
		}
	}

	void ColorPreviewer::PreviewBorder_PointerPressed(winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
	{
		if (const auto border = sender.try_as<winrt::Border>())
		{
			int accentStep = 0;
			const auto hsvColor = HsvColor();

			// Get the value component delta
			try
			{
				if (const auto tag = border.Tag().try_as<winrt::hstring>())
				{
					accentStep = std::stoi({ tag->data(), tag->size() });
				}
			}
			catch (...) {}

			auto newHsvColor = winrt::XamlToolkit::WinUI::Controls::AccentColorConverter::GetAccent(hsvColor, accentStep);
			OnColorChangeRequested(newHsvColor);
		}
	}
}
