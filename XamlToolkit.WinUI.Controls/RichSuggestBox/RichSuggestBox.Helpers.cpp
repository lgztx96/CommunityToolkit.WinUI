#include "pch.h"
#include "winrt_module_imports.h"
#include "RichSuggestBox.h"
#include "../SettingsControls/Helpers/ControlHelper.h"
#include <winrt/Microsoft.UI.Interop.h>
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.Graphics.Display.h>
#include <winrt/Microsoft.UI.Content.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Windowing.h>
#else
import winrt.Microsoft.UI.Content;
import winrt.Microsoft.UI.Dispatching;
import winrt.Microsoft.UI.Windowing;
#endif

namespace winrt
{
	using namespace ::winrt::Microsoft::UI;
	using namespace ::winrt::Microsoft::UI::Content;
	using namespace ::winrt::Microsoft::UI::Dispatching;
	using namespace ::winrt::Microsoft::UI::Windowing;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	bool RichSuggestBox::IsElementOnScreen(winrt::FrameworkElement const& element, double offsetX, double offsetY)
	{
		// DisplayInformation only works in UWP. No alternative to get DisplayInformation.ScreenHeightInRawPixels
		// Or Window position in Window.Current.Bounds
		// Tracking issues:
		// https://github.com/microsoft/WindowsAppSDK/issues/114
		// https://github.com/microsoft/microsoft-ui-xaml/issues/4228
		// TODO: Remove when DisplayInformation.ScreenHeightInRawPixels alternative is available

		if (winrt::DispatcherQueue::GetForCurrentThread() == nullptr)
		{
			return true;
		}

		auto xamlRoot = element.XamlRoot();
		auto appWindowId = xamlRoot.ContentIslandEnvironment().AppWindowId();

		HWND hWnd = GetWindowFromWindowId(appWindowId);

		double scaleFactor = GetDpiForWindow(hWnd) / 96.0;

		// Get bounds of element from root of tree
		auto point = UIElementExtensions::CoordinatesFrom(element, nullptr);
		auto elementBounds = winrt::Rect(point.X, point.Y, element.ActualWidth(), element.ActualHeight());

		// Apply offset
		elementBounds.X += offsetX;
		elementBounds.Y += offsetY;

		// Get Window position
		auto appWindow = winrt::AppWindow::GetFromWindowId(appWindowId);

		auto windowPosition = appWindow.Position();

		// Offset Element within Window on Screen
		elementBounds.X += windowPosition.X / scaleFactor;
		elementBounds.Y += windowPosition.Y / scaleFactor;

		// Get Screen DPI info
		auto displayArea = winrt::DisplayArea::GetFromWindowId(
			appWindowId, winrt::DisplayAreaFallback::Nearest);

		int32_t displayHeight = displayArea.OuterBounds().Height;

		// Check if top/bottom are within confines of screen
		return winrt::RectHelper::GetTop(elementBounds) * scaleFactor >= 0 
			&& winrt::RectHelper::GetBottom(elementBounds) * scaleFactor <= displayHeight;
	}

	bool RichSuggestBox::IsElementInsideWindow(winrt::FrameworkElement const& element, double offsetX, double offsetY)
	{
		// Get bounds of element from root of tree
		auto point = UIElementExtensions::CoordinatesFrom(element, nullptr);
		auto elementBounds = winrt::Rect(point.X, point.Y, element.ActualWidth(), element.ActualHeight());

		// Apply offset
		elementBounds.X += offsetX;
		elementBounds.Y += offsetY;

		// Get size of window itself
		auto windowSize = element.XamlRoot().Size();
		auto windowBounds = winrt::Rect(0, 0, windowSize.Width, windowSize.Height);

		// Calculate if there's an intersection
		elementBounds = winrt::RectHelper::Intersect(elementBounds, windowBounds);

		// See if we are still fully visible within the Window
		return elementBounds.Height >= element.ActualHeight();
	}

	winrt::hstring RichSuggestBox::EnforcePrefixesRequirements(winrt::hstring const& value)
	{
		if (value.empty())
		{
			return winrt::hstring{};
		}

		std::wstring ret;

		for (const auto& ch : value)
		{
			if (iswpunct(ch))
			{
				ret += ch;
			}
		}
		return winrt::hstring{ ret };
	}

	/// <summary>
	/// Pad range with Zero-Width-Spaces.
	/// </summary>
	/// <param name="range">Range to pad.</param>
	/// <param name="format">Character format to apply to the padding.</param>
	void RichSuggestBox::PadRange(winrt::ITextRange const& range, winrt::ITextCharacterFormat const& format)
	{
		auto startPosition = range.StartPosition();
		auto endPosition = range.EndPosition() + 1;
		auto clone = range.GetClone();
		clone.Collapse(true);
		clone.SetText(winrt::TextSetOptions::Unhide, L"\u200B");
		clone.CharacterFormat().SetClone(format);
		clone.SetRange(endPosition, endPosition);
		clone.SetText(winrt::TextSetOptions::Unhide, L"\u200B");
		clone.CharacterFormat().SetClone(format);
		range.SetRange(startPosition, endPosition + 1);
	}

	void RichSuggestBox::ForEachLinkInDocument(winrt::RichEditTextDocument const& document, std::function<void(winrt::ITextRange const&)> const& action)
	{
		auto range = document.GetRange(0, 0);
		range.SetIndex(winrt::TextRangeUnit::Character, -1, false);

		// Handle link at the very end of the document where GetIndex fails to detect
		range.Expand(winrt::TextRangeUnit::Link);
		if (!range.Link().empty())
		{
			if (action) action(range);
		}

		auto nextIndex = range.GetIndex(winrt::TextRangeUnit::Link);
		while (nextIndex != 0 && nextIndex != 1)
		{
			range.Move(winrt::TextRangeUnit::Link, -1);

			auto linkRange = range.GetClone();
			linkRange.Expand(winrt::TextRangeUnit::Link);

			// Adjacent links have the same index. Manually check each link with Collapse and Expand.
			auto previousStart = linkRange.StartPosition();
			auto hasAdjacentToken = true;
			while (hasAdjacentToken)
			{
				if (action) action(linkRange);

				linkRange.Collapse(false);
				linkRange.Expand(winrt::TextRangeUnit::Link);
				hasAdjacentToken = !linkRange.Link().empty() && linkRange.StartPosition() != previousStart;
				previousStart = linkRange.StartPosition();
			}

			nextIndex = range.GetIndex(winrt::TextRangeUnit::Link);
		}
	}
}
