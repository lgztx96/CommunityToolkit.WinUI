#include "pch.h"
#include "ColorPreviewer.h"
#if __has_include("Primitives.ColorPreviewer.g.cpp")
#include "Primitives.ColorPreviewer.g.cpp"
#endif
#include "ColorPickerRenderingHelpers.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::Primitives::implementation
{
	ColorPreviewer::ColorPreviewer()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void ColorPreviewer::ConnectEvents(bool connected)
	{
		if (connected && eventsConnected == false)
		{
			// Add all events
			if (CheckeredBackgroundBorder) {
				checkeredLoadedToken = CheckeredBackgroundBorder.Loaded({ this, &ColorPreviewer::CheckeredBackgroundBorder_Loaded });
			}

			if (N1PreviewBorder) {
				n1PointerPressedToken = N1PreviewBorder.PointerPressed({ this, &ColorPreviewer::PreviewBorder_PointerPressed });
			}
			if (N2PreviewBorder) {
				n2PointerPressedToken = N2PreviewBorder.PointerPressed({ this,&ColorPreviewer::PreviewBorder_PointerPressed });
			}
			if (P1PreviewBorder) {
				p1PointerPressedToken = P1PreviewBorder.PointerPressed({ this,&ColorPreviewer::PreviewBorder_PointerPressed });
			}
			if (P2PreviewBorder) {
				p2PointerPressedToken = P2PreviewBorder.PointerPressed({ this, &ColorPreviewer::PreviewBorder_PointerPressed });
			}

			eventsConnected = true;
		}
		else if (connected == false && eventsConnected == true)
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

		CheckeredBackgroundBorder = GetTemplateChild(L"CheckeredBackgroundBorder").try_as<Border>();

		N1PreviewBorder = GetTemplateChild(L"N1PreviewBorder").try_as<Border>();
		N2PreviewBorder = GetTemplateChild(L"N2PreviewBorder").try_as<Border>();
		P1PreviewBorder = GetTemplateChild(L"P1PreviewBorder").try_as<Border>();
		P2PreviewBorder = GetTemplateChild(L"P2PreviewBorder").try_as<Border>();

		// Must connect after controls are resolved
		ConnectEvents(true);

		base_type::OnApplyTemplate();
	}

	void ColorPreviewer::OnDependencyPropertyChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
	{
		VisualStateManager::GoToState(*this, ShowAccentColors() ? L"AccentColorsVisible" : L"AccentColorsCollapsed", true);
	}

	void ColorPreviewer::OnColorChangeRequested(winrt::CommunityToolkit::WinUI::HsvColor color)
	{
		ColorChangeRequested.invoke(*this, color);
	}

	winrt::Windows::Foundation::IAsyncAction ColorPreviewer::CheckeredBackgroundBorder_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
	{
		if (auto border = sender.try_as<Border>())
		{
			int width = static_cast<int>(border.ActualWidth());
			int height = static_cast<int>(border.ActualHeight());

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

	void ColorPreviewer::PreviewBorder_PointerPressed(IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
	{
		if (auto border = sender.try_as<Border>())
		{
			int accentStep = 0;
			auto hsvColor = HsvColor();

			// Get the value component delta
			try
			{
				if (auto tag = border.Tag().try_as<winrt::hstring>())
				{
					accentStep = std::stoi({ tag->data(), tag->size() });
				}
			}
			catch (...) {}

			auto newHsvColor = AccentColorConverter::GetAccent(hsvColor, accentStep);
			OnColorChangeRequested(newHsvColor);
		}
	}
}