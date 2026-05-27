#include "pch.h"
#include "winrt_module_imports.h"
#include "RichSuggestBox.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	bool RichSuggestBox::IsElementOnScreen([[maybe_unused]] FrameworkElement const& element, [[maybe_unused]] double offsetX, [[maybe_unused]] double offsetY)
	{
		// DisplayInformation only works in UWP. No alternative to get DisplayInformation.ScreenHeightInRawPixels
		// Or Window position in Window.Current.Bounds
		// Tracking issues:
		// https://github.com/microsoft/WindowsAppSDK/issues/114
		// https://github.com/microsoft/microsoft-ui-xaml/issues/4228
		// TODO: Remove when DisplayInformation.ScreenHeightInRawPixels alternative is available
//#if !WINAPPSDK
//		if (CoreWindow::GetForCurrentThread() == nullptr)
//		{
//			return true;
//		}
//
//		// Get bounds of element from root of tree
//		auto point = UIElementExtensions::CoordinatesFrom(element, nullptr);
//		auto elementBounds = Rect(point.X, point.Y, element.ActualWidth(), element.ActualHeight());
//
//		// Apply offset
//		elementBounds.X += offsetX;
//		elementBounds.Y += offsetY;
//
//		// Get Window position
//		auto windowBounds = Window::Current().Bounds();
//
//		// Offset Element within Window on Screen
//		elementBounds.X += windowBounds.X;
//		elementBounds.Y += windowBounds.Y;
//
//		// Get Screen DPI info
//		auto displayInfo = DisplayInformation::GetForCurrentView();
//		auto scaleFactor = displayInfo.RawPixelsPerViewPixel;
//		auto displayHeight = displayInfo.ScreenHeightInRawPixels;
//
//		// Check if top/bottom are within confines of screen
//		return RectHelper::GetTop(elementBounds) * scaleFactor >= 0 && RectHelper::GetBottom(elementBounds) * scaleFactor <= displayHeight;
//#else
		return true;
//#endif
	}

	bool RichSuggestBox::IsElementInsideWindow([[maybe_unused]] FrameworkElement const& element, [[maybe_unused]] double offsetX, [[maybe_unused]] double offsetY)
	{
		// THIS IS NOT SUPPORTED IN WINUI3
//#if !WINAPPSDK
//		// Get bounds of element from root of tree
//		auto point = UIElementExtensions::CoordinatesFrom(element, nullptr);
//		auto elementBounds = Rect(point.X, point.Y, element.ActualWidth(), element.ActualHeight());
//
//		// Apply offset
//		elementBounds.X += offsetX;
//		elementBounds.Y += offsetY;
//
//		// Get size of window itself
//		auto windowBounds = IsXamlRootAvailable() && element.XamlRoot() != nullptr
//			? element.XamlRoot().Size().ToRect()
//			: ApplicationView::GetForCurrentView().VisibleBounds.ToSize().ToRect(); // Normalize
//
//		// Calculate if there's an intersection
//		RectHelper::Intersect(elementBounds, windowBounds);
//
//		// See if we are still fully visible within the Window
//		return elementBounds.Height >= element.ActualHeight();
//#else
		return true;
//#endif
	}

	winrt::hstring RichSuggestBox::EnforcePrefixesRequirements(winrt::hstring const& value)
	{
		if (value.empty())
		{
			return winrt::hstring{};
		}

		std::wstring ret;

		for (auto const& ch : value)
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
	void RichSuggestBox::PadRange(ITextRange const& range, ITextCharacterFormat const& format)
	{
		auto startPosition = range.StartPosition();
		auto endPosition = range.EndPosition() + 1;
		auto clone = range.GetClone();
		clone.Collapse(true);
		clone.SetText(TextSetOptions::Unhide, L"\u200B");
		clone.CharacterFormat().SetClone(format);
		clone.SetRange(endPosition, endPosition);
		clone.SetText(TextSetOptions::Unhide, L"\u200B");
		clone.CharacterFormat().SetClone(format);
		range.SetRange(startPosition, endPosition + 1);
	}

	void RichSuggestBox::ForEachLinkInDocument(RichEditTextDocument const& document, std::function<void(ITextRange const&)> const& action)
	{
		auto range = document.GetRange(0, 0);
		range.SetIndex(TextRangeUnit::Character, -1, false);

		// Handle link at the very end of the document where GetIndex fails to detect
		range.Expand(TextRangeUnit::Link);
		if (!range.Link().empty())
		{
			if (action) action(range);
		}

		auto nextIndex = range.GetIndex(TextRangeUnit::Link);
		while (nextIndex != 0 && nextIndex != 1)
		{
			range.Move(TextRangeUnit::Link, -1);

			auto linkRange = range.GetClone();
			linkRange.Expand(TextRangeUnit::Link);

			// Adjacent links have the same index. Manually check each link with Collapse and Expand.
			auto previousStart = linkRange.StartPosition();
			auto hasAdjacentToken = true;
			while (hasAdjacentToken)
			{
				if (action) action(linkRange);

				linkRange.Collapse(false);
				linkRange.Expand(TextRangeUnit::Link);
				hasAdjacentToken = !linkRange.Link().empty() && linkRange.StartPosition() != previousStart;
				previousStart = linkRange.StartPosition();
			}

			nextIndex = range.GetIndex(TextRangeUnit::Link);
		}
	}
}
