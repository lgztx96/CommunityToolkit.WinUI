#include "pch.h"
#include "EqualPanel.h"
#if __has_include("EqualPanel.g.cpp")
#include "EqualPanel.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void EqualPanel::OnSpacingChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (auto panel = d.try_as<class_type>())
			panel.InvalidateMeasure();
	}

	Size EqualPanel::MeasureOverride(Size availableSize)
	{
		_maxItemWidth = 0;
		_maxItemHeight = 0;

		auto elements = Children()
			| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible; });
		_visibleItemsCount = 0;

		for (auto child : elements)
		{
			child.Measure(availableSize);
			_maxItemWidth = std::max<double>(_maxItemWidth, child.DesiredSize().Width);
			_maxItemHeight = std::max<double>(_maxItemHeight, child.DesiredSize().Height);
			_visibleItemsCount++;
		}

		if (_visibleItemsCount > 0)
		{
			// Return equal widths based on the widest item
			// In very specific edge cases the AvailableWidth might be infinite resulting in a crash.
			if (HorizontalAlignment() != HorizontalAlignment::Stretch || std::isinf(availableSize.Width))
			{
				auto width = (_maxItemWidth * _visibleItemsCount) + (Spacing() * (_visibleItemsCount - 1));
				return Size(static_cast<float>(width), static_cast<float>(_maxItemHeight));
			}
			else
			{
				// Equal columns based on the available width, adjust for spacing
				double totalWidth = availableSize.Width - (Spacing() * (_visibleItemsCount - 1));
				_maxItemWidth = totalWidth / _visibleItemsCount;
				return Size(availableSize.Width, static_cast<float>(_maxItemHeight));
			}
		}
		else
		{
			return Size(0, 0);
		}
	}

	Size EqualPanel::ArrangeOverride(Size finalSize)
	{
		float x = 0;

		// Check if there's more (little) width available - if so, set max item width to the maximum possible as we have an almost perfect height.
		if (finalSize.Width > _visibleItemsCount * _maxItemWidth + (Spacing() * (_visibleItemsCount - 1)))
		{
			_maxItemWidth = (finalSize.Width - (Spacing() * (_visibleItemsCount - 1))) / _visibleItemsCount;
		}

		auto elements = Children()
			| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible; });
		for (const auto& child : elements)
		{
			child.Arrange(Rect(x, 0, static_cast<float>(_maxItemWidth), static_cast<float>(_maxItemHeight)));
			x += static_cast<float>(_maxItemWidth + Spacing());
		}
		return finalSize;
	}

	void EqualPanel::OnHorizontalAlignmentChanged([[maybe_unused]] DependencyObject const& sender, [[maybe_unused]] DependencyProperty const& dp)
	{
		InvalidateMeasure();
	}
}
