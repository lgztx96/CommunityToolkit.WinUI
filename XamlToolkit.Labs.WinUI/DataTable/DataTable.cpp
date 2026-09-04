#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <ranges>
#include <vector>
#endif
#include "DataTable.h"
#include "DataColumn.h"
#if __has_include("DataTable.g.cpp")
#include "DataTable.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	DataTable::DataTable()
	{
		Loaded({ this, &DataTable::DataTable_Loaded });
	}

	void DataTable::DataTable_Loaded([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		// A tab/content presenter can unload and reload the same table without
		// changing either its final size or its column dependency properties.
		// Force a fresh measure so text measured while the table was hidden
		// cannot keep a stale TextTrimming result.
		InvalidateMeasure();
		InvalidateArrange();

		for (auto& row : Rows())
		{
			row.InvalidateMeasure();
			row.InvalidateArrange();
		}
	}

	bool DataTable::IsAnyColumnAuto()
	{
		auto children = Children();
		return std::any_of(children.begin(), children.end(), [](auto&& e)
			{
				if (auto column = e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>()) {
					auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
					return columnImpl->CurrentWidth().GridUnitType == GridUnitType::Auto;
				}

				return false;
			});
	}

	std::set<winrt::XamlToolkit::Labs::WinUI::DataRow>& DataTable::Rows() { return _rows; }

	double DataTable::ColumnWidth(uint32_t index) const
	{
		return index < _columnWidths.size() ? _columnWidths[index] : 0;
	}

	double DataTable::BeginColumnResize(winrt::XamlToolkit::Labs::WinUI::DataColumn const& resizedColumn)
	{
		const auto width = _layoutWidth > 0 ? _layoutWidth : ActualWidth();
		UpdateColumnWidths(width);
		const auto resolvedWidths = _columnWidths;

		double resizedWidth = resizedColumn.ActualWidth();
		auto children = Children();
		_isFreezingColumnWidths = true;
		try
		{
			for (uint32_t i = 0; i < children.Size(); i++)
			{
				auto column = children.GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
				if (column == nullptr || column.Visibility() != Visibility::Visible)
				{
					continue;
				}

				const auto resolvedWidth = i < resolvedWidths.size() ? resolvedWidths[i] : 0;
				winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column)->SetCurrentWidth(resolvedWidth);
				if (column == resizedColumn)
				{
					resizedWidth = resolvedWidth;
				}
			}
		}
		catch (...)
		{
			_isFreezingColumnWidths = false;
			throw;
		}
		_isFreezingColumnWidths = false;

		// Freezing all currently resolved widths prevents a Star column from
		// compensating in the opposite direction while another column is dragged.
		UpdateColumnWidths(width);
		return resizedWidth;
	}

	void DataTable::ColumnWidthChanged()
	{
		if (_isFreezingColumnWidths)
		{
			return;
		}

		const auto width = _layoutWidth > 0 ? _layoutWidth : ActualWidth();
		UpdateColumnWidths(width);
		InvalidateMeasure();
		InvalidateArrange();

		for (auto& row : Rows())
		{
			row.InvalidateMeasure();
			row.InvalidateArrange();
		}
	}

	void DataTable::UpdateColumnWidths(double availableWidth)
	{
		auto children = Children();
		_columnWidths.assign(children.Size(), 0);

		double fixedWidth = 0;
		double autoWidth = 0;
		double proportionalUnits = 0;
		uint32_t visibleColumns = 0;

		for (uint32_t i = 0; i < children.Size(); i++)
		{
			auto column = children.GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
			if (column == nullptr || column.Visibility() != Visibility::Visible) continue;

			visibleColumns++;
			auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
			const auto currentWidth = columnImpl->CurrentWidth();

			if (GridLengthHelper::GetIsAbsolute(currentWidth))
			{
				_columnWidths[i] = currentWidth.Value;
				fixedWidth += currentWidth.Value;
			}
			else if (GridLengthHelper::GetIsStar(currentWidth))
			{
				proportionalUnits += currentWidth.Value;
			}
			else
			{
				const auto width = std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
				_columnWidths[i] = width;
				autoWidth += width;
			}
		}

		const auto spacingWidth = visibleColumns > 1
			? (visibleColumns - 1) * ColumnSpacing()
			: 0;
		const auto proportionalAmount = proportionalUnits > 0 && std::isfinite(availableWidth)
			? std::max<double>((availableWidth - fixedWidth - autoWidth - spacingWidth) / proportionalUnits, 0)
			: 0;

		for (uint32_t i = 0; i < children.Size(); i++)
		{
			auto column = children.GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
			if (column == nullptr || column.Visibility() != Visibility::Visible) continue;

			auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
			if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
			{
				_columnWidths[i] = proportionalAmount * columnImpl->CurrentWidth().Value;
			}
		}

		for (uint32_t i = 0; i < children.Size(); i++)
		{
			if (auto column = children.GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
				column != nullptr && column.Visibility() == Visibility::Visible)
			{
				winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column)->SetActualColumnWidth(_columnWidths[i]);
			}
		}
	}

	void DataTable::ColumnResized()
	{
		if (_isFreezingColumnWidths)
		{
			return;
		}

		UpdateColumnWidths(_layoutWidth);
		InvalidateArrange();

		for (auto& row : Rows())
		{
			row.InvalidateMeasure();
			row.InvalidateArrange();
		}
	}

	double DataTable::ColumnSpacing() const { return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty)); }
	void DataTable::ColumnSpacing(double value) { SetValue(ColumnSpacingProperty, winrt::box_value(value)); }

	const wil::single_threaded_property<DependencyProperty> DataTable::ColumnSpacingProperty =
		DependencyProperty::Register(L"ColumnSpacing", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0)));

	Size DataTable::MeasureOverride(Size availableSize)
	{
		_layoutWidth = availableSize.Width;

		double fixedWidth = 0;
		double proportionalUnits = 0;
		double autoSized = 0;

		double maxHeight = 0;

		auto elements = Children()
			| std::ranges::views::filter([](auto&& e) { return e.Visibility() == Visibility::Visible && e.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
			| std::ranges::views::transform([](auto&& e) { return e.template as<winrt::XamlToolkit::Labs::WinUI::DataColumn>(); })
			| std::ranges::to<std::vector>();

		// We only need to measure elements that are visible
		for (const auto& column : elements)
		{
			auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
			if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
			{
				proportionalUnits += columnImpl->CurrentWidth().Value;
			}
			else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
			{
				fixedWidth += columnImpl->CurrentWidth().Value;
			}
		}

		// Add in spacing between columns to our fixed size allotment
		if (elements.size() > 1)
		{
			fixedWidth += (elements.size() - 1) * ColumnSpacing();
		}

		const auto proportionalAmount = proportionalUnits > 0 && std::isfinite(availableSize.Width)
			? std::max<double>((availableSize.Width - fixedWidth) / proportionalUnits, 0)
			: 0;

		for (const auto& column : elements)
		{
			auto columnImpl = winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column);
			if (GridLengthHelper::GetIsStar(columnImpl->CurrentWidth()))
			{
				column.Measure(Size(static_cast<float>(proportionalAmount * columnImpl->CurrentWidth().Value), availableSize.Height));
			}
			else if (GridLengthHelper::GetIsAbsolute(columnImpl->CurrentWidth()))
			{
				column.Measure(Size(static_cast<float>(columnImpl->CurrentWidth().Value), availableSize.Height));
			}
			else
			{
				// Avoid negative values when columns don't fit `availableSize`. Otherwise the `Size` constructor will throw.
				column.Measure(Size(std::max<float>(static_cast<float>(availableSize.Width - fixedWidth - autoSized), 0), availableSize.Height));

				// Keep track of already 'allotted' space, use either the maximum child size (if we know it) or the header content
				autoSized += std::max<double>(column.DesiredSize().Width, columnImpl->MaxChildDesiredWidth);
			}

			maxHeight = std::max<double>(maxHeight, column.DesiredSize().Height);
		}

		UpdateColumnWidths(availableSize.Width);

		// Horizontal scrolling requires the panel to report the resolved table width,
		// not merely the viewport width. Keep the table at least as wide as the
		// viewport to avoid whole-row shifts when the columns occupy less space, but
		// allow the desired width to exceed the viewport when columns overflow it.
		double desiredWidth = 0;
		for (const auto& column : elements)
		{
			desiredWidth += winrt::get_self<winrt::XamlToolkit::Labs::WinUI::implementation::DataColumn>(column)->ActualColumnWidth();
		}
		if (elements.size() > 1)
		{
			desiredWidth += (elements.size() - 1) * ColumnSpacing();
		}
		if (std::isfinite(availableSize.Width))
		{
			desiredWidth = std::max<double>(desiredWidth, availableSize.Width);
		}

		return Size(static_cast<float>(desiredWidth), static_cast<float>(maxHeight));
	}

	Size DataTable::ArrangeOverride(Size finalSize)
	{
		_layoutWidth = finalSize.Width;
		UpdateColumnWidths(finalSize.Width);

		double x = 0;
		auto children = Children();
		uint32_t arrangedColumns = 0;
		const auto visibleColumns = std::ranges::count_if(children,
			[](auto&& element)
			{
				return element.Visibility() == Visibility::Visible &&
					element.template try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>() != nullptr;
			});

		for (uint32_t i = 0; i < children.Size(); i++)
		{
			auto column = children.GetAt(i).try_as<winrt::XamlToolkit::Labs::WinUI::DataColumn>();
			if (column == nullptr || column.Visibility() != Visibility::Visible) continue;

			const auto width = ColumnWidth(i);
			column.Arrange(Rect(static_cast<float>(x), 0, static_cast<float>(width), finalSize.Height));
			x += width;

			if (++arrangedColumns < visibleColumns)
			{
				x += ColumnSpacing();
			}
		}

		return finalSize;
	}
}
