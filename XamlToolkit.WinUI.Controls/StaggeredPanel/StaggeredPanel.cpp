#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <memory>
#endif
#include "StaggeredPanel.h"
#if __has_include("StaggeredPanel.g.cpp")
#include "StaggeredPanel.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> StaggeredPanel::DesiredColumnWidthProperty =
		winrt::DependencyProperty::Register(
			L"DesiredColumnWidth",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(250.0), &StaggeredPanel::OnDesiredColumnWidthChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> StaggeredPanel::PaddingProperty =
		winrt::DependencyProperty::Register(
			L"Padding",
			winrt::xaml_typename<winrt::Thickness>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }), &StaggeredPanel::OnPaddingChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> StaggeredPanel::ColumnSpacingProperty =
		winrt::DependencyProperty::Register(
			L"ColumnSpacing",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0.0), &StaggeredPanel::OnPaddingChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> StaggeredPanel::RowSpacingProperty =
		winrt::DependencyProperty::Register(
			L"RowSpacing",
			winrt::xaml_typename<double>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0.0), &StaggeredPanel::OnPaddingChanged });

	StaggeredPanel::StaggeredPanel()
	{
		RegisterPropertyChangedCallback(winrt::FrameworkElement::HorizontalAlignmentProperty(), { this, &StaggeredPanel::OnHorizontalAlignmentChanged });
	}

	winrt::Size StaggeredPanel::MeasureOverride(winrt::Size availableSize)
	{
		auto padding = Padding();
		double availableWidth = availableSize.Width - padding.Left - padding.Right;
		double availableHeight = availableSize.Height - padding.Top - padding.Bottom;

		_columnWidth = std::min<double>(DesiredColumnWidth(), availableWidth);
		int numColumns = std::max<int>(1, static_cast<int>(std::floor(availableWidth / _columnWidth)));

		// adjust for column spacing on all columns expect the first
		double totalWidth = _columnWidth + ((numColumns - 1) * (_columnWidth + ColumnSpacing()));
		if (totalWidth > availableWidth)
		{
			numColumns--;
		}
		else if (std::isinf(availableWidth))
		{
			availableWidth = totalWidth;
		}

		if (HorizontalAlignment() == HorizontalAlignment::Stretch)
		{
			availableWidth = availableWidth - ((numColumns - 1) * ColumnSpacing());
			_columnWidth = availableWidth / numColumns;
		}

		auto children = Children();
		if (children.Size() == 0)
		{
			return Size(0, 0);
		}

		auto columnHeights = std::make_unique_for_overwrite<double[]>(numColumns);
		auto itemsPerColumn = std::make_unique_for_overwrite<double[]>(numColumns);
		auto columnHeightsSpan = std::span{ columnHeights.get(), static_cast<size_t>(numColumns) };
		for (int i = 0; i < static_cast<int>(children.Size()); i++)
		{
			auto columnIndex = GetColumnIndex(columnHeightsSpan);

			auto child = children.GetAt(i);
			child.Measure(Size((static_cast<float>(_columnWidth)), static_cast<float>(availableHeight)));
			auto elementSize = child.DesiredSize();
			columnHeights[columnIndex] += elementSize.Height + (itemsPerColumn[columnIndex] > 0 ? RowSpacing() : 0);
			itemsPerColumn[columnIndex]++;
		}

		double desiredHeight = std::ranges::max(columnHeightsSpan);

		return Size(static_cast<float>(availableWidth), static_cast<float>(desiredHeight));
	}

	winrt::Size StaggeredPanel::ArrangeOverride(winrt::Size finalSize)
	{
		double horizontalOffset = Padding().Left;
		double verticalOffset = Padding().Top;
		int numColumns = std::max<int>(1, static_cast<int>(std::floor(finalSize.Width / _columnWidth)));

		// adjust for horizontal spacing on all columns expect the first
		double totalWidth = _columnWidth + ((numColumns - 1) * (_columnWidth + ColumnSpacing()));
		if (totalWidth > finalSize.Width)
		{
			numColumns--;

			// Need to recalculate the totalWidth for a correct horizontal offset
			totalWidth = _columnWidth + ((numColumns - 1) * (_columnWidth + ColumnSpacing()));
		}

		if (HorizontalAlignment() == winrt::HorizontalAlignment::Right)
		{
			horizontalOffset += finalSize.Width - totalWidth;
		}
		else if (HorizontalAlignment() == winrt::HorizontalAlignment::Center)
		{
			horizontalOffset += (finalSize.Width - totalWidth) / 2;
		}

		auto columnHeights = std::make_unique_for_overwrite<double[]>(numColumns);
		auto itemsPerColumn = std::make_unique_for_overwrite<double[]>(numColumns);

		auto columnSpacing = ColumnSpacing();
		auto rowSpacing = RowSpacing();
		auto children = Children();
		auto columnHeightsSpan = std::span{ columnHeights.get(), static_cast<size_t>(numColumns) };
		for (int i = 0; i < static_cast<int>(children.Size()); i++)
		{
			auto columnIndex = GetColumnIndex(columnHeightsSpan);

			auto child = children.GetAt(i);
			auto elementSize = child.DesiredSize();

			double elementHeight = elementSize.Height;

			double itemHorizontalOffset = horizontalOffset + (_columnWidth * columnIndex) + (columnSpacing * columnIndex);
			double itemVerticalOffset = columnHeights[columnIndex] + verticalOffset + (rowSpacing * itemsPerColumn[columnIndex]);

			winrt::Rect bounds(
				static_cast<float>(itemHorizontalOffset), 
				static_cast<float>(itemVerticalOffset), 
				static_cast<float>(_columnWidth), 
				static_cast<float>(elementHeight));

			child.Arrange(bounds);

			columnHeights[columnIndex] += elementSize.Height;
			itemsPerColumn[columnIndex]++;
		}

		return base_type::ArrangeOverride(finalSize);
	}

	void StaggeredPanel::OnDesiredColumnWidthChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto panel = d.try_as<class_type>())
		{
			panel.InvalidateMeasure();
		}
	}

	void StaggeredPanel::OnPaddingChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto panel = d.try_as<class_type>())
		{
			panel.InvalidateMeasure();
		}
	}

	void StaggeredPanel::OnHorizontalAlignmentChanged([[maybe_unused]] winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyProperty const& dp)
	{
		InvalidateMeasure();
	}

	int StaggeredPanel::GetColumnIndex(std::span<const double> columnHeights)
	{
		int columnIndex = 0;
		double height = columnHeights[0];
		for (int j = 1; j < static_cast<int>(columnHeights.size()); j++)
		{
			if (columnHeights[j] < height)
			{
				columnIndex = j;
				height = columnHeights[j];
			}
		}

		return columnIndex;
	}
}
