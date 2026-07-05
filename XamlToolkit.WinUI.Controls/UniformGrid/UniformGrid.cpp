#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ranges>
#endif
#include "UniformGrid.h"
#if __has_include("UniformGrid.g.cpp")
#include "UniformGrid.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> UniformGrid::AutoLayoutProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"AutoLayout",
			winrt::xaml_typename<winrt::IReference<bool>>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	const wil::single_threaded_property<winrt::DependencyProperty> UniformGrid::ColumnsProperty =
		winrt::DependencyProperty::Register(
			L"Columns",
			winrt::xaml_typename<int32_t>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> UniformGrid::FirstColumnProperty =
		winrt::DependencyProperty::Register(
			L"FirstColumn",
			winrt::xaml_typename<int32_t>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> UniformGrid::OrientationProperty =
		winrt::DependencyProperty::Register(
			L"Orientation",
			winrt::xaml_typename<winrt::Orientation>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(winrt::Orientation::Horizontal), &UniformGrid::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> UniformGrid::RowsProperty =
		winrt::DependencyProperty::Register(
			L"Rows",
			winrt::xaml_typename<int32_t>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(0), &UniformGrid::OnPropertyChanged });

	void UniformGrid::OnPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::IInspectable const& newValue)
	{
		if (const auto self = d.try_as<class_type>())
		{
			self.InvalidateMeasure();
		}
	}

	winrt::Size UniformGrid::MeasureOverride(winrt::Size availableSize)
	{
		// Get all Visible FrameworkElement Children
		auto visible = Children() | 
		std::views::filter([](const auto& item)
		{
			return item.Visibility() != winrt::Visibility::Collapsed
				&& static_cast<bool>(item.template try_as<winrt::FrameworkElement>());
		}) | 
		std::views::transform([](const auto& item)
		{
			return item.template as<winrt::FrameworkElement>();
		}) | std::ranges::to<std::vector>();

		int rows, columns;
		std::tie(rows, columns) = GetDimensions(visible, Rows(), Columns(), FirstColumn());

		// Now that we know size, setup automatic rows/columns
		// to utilize Grid for UniformGrid behavior.
		// We also interleave any specified rows/columns with fixed sizes.
		SetupRowDefinitions(rows);
		SetupColumnDefinitions(columns);

		TakenSpotsReferenceHolder* spotref{ nullptr };

		// If the last spot holder matches the size currently in use, just reset
		// that instance and reuse it to avoid allocating a new bit array.
		if (_spotref && _spotref->Height() == rows && _spotref->Width() == columns)
		{
			spotref = _spotref.get();

			spotref->Reset();
		}
		else
		{
			_spotref = std::make_unique<TakenSpotsReferenceHolder>(rows, columns);
			spotref = _spotref.get();
		}

		// Figure out which children we should automatically layout and where available openings are.
		for (const auto& child : visible)
		{
			if (child)
			{
				auto row = winrt::Grid::GetRow(child);
				auto col = winrt::Grid::GetColumn(child);
				auto rowspan = winrt::Grid::GetRowSpan(child);
				auto colspan = winrt::Grid::GetColumnSpan(child);

				// If an element needs to be forced in the 0, 0 position,
				// they should manually set UniformGrid.AutoLayout to False for that element.
				if ((row == 0 && col == 0 && GetAutoLayout(child) == nullptr))
				{
					SetAutoLayout(child, true);
				}
				else if (auto layout = GetAutoLayout(child); layout && layout.GetBoolean())
				{
					SetAutoLayout(child, true);
				}
				else
				{
					SetAutoLayout(child, false);

					spotref->Fill(true, row, col, colspan, rowspan);
				}
			}
		}

		// Setup available size with our known dimensions now.
		// UniformGrid expands size based on largest singular item.
		double columnSpacingSize = 0;
		double rowSpacingSize = 0;

		columnSpacingSize = ColumnSpacing() * (columns - 1);
		rowSpacingSize = RowSpacing() * (rows - 1);

		auto childWidth = static_cast<float>((availableSize.Width - columnSpacingSize) / columns);
		auto childHeight = static_cast<float>((availableSize.Height - rowSpacingSize) / rows);
		winrt::Size childSize{ childWidth, childHeight };

		double maxWidth = 0.0;
		double maxHeight = 0.0;

		auto freespots = GetFreeSpot(spotref, FirstColumn(), Orientation() == winrt::Orientation::Vertical);

		auto spotIter = freespots.begin();
		auto spotEnd = freespots.end();

		for (auto& child : visible)
		{
			if (!child) continue;

			if (auto layout = GetAutoLayout(child); layout && layout.GetBoolean())
			{
				if (spotIter != spotEnd)
				{
					int row, column;
					std::tie(row, column) = *spotIter;
					++spotIter;

					winrt::Grid::SetRow(child, row);
					winrt::Grid::SetColumn(child, column);

					auto rowspan = winrt::Grid::GetRowSpan(child);
					auto colspan = winrt::Grid::GetColumnSpan(child);

					if (rowspan > 1 || colspan > 1)
					{
						spotref->Fill(true, row, column, colspan, rowspan);
					}
				}
				else
				{
					child.Measure(winrt::SizeHelper::Empty());
					_overflow.push_back(child);
					continue;
				}
			}
			else if (winrt::Grid::GetRow(child) < 0 || winrt::Grid::GetRow(child) >= rows ||
				winrt::Grid::GetColumn(child) < 0 || winrt::Grid::GetColumn(child) >= columns)
			{
				child.Measure(winrt::SizeHelper::Empty());
				_overflow.push_back(child);
				continue;
			}

			child.Measure(childSize);
			maxWidth = std::max<double>(maxWidth, child.DesiredSize().Width);
			maxHeight = std::max<double>(maxHeight, child.DesiredSize().Height);
		}

		// Return our desired size based on the largest child we found, our dimensions, and spacing.
		float width = static_cast<float>((maxWidth * columns) + columnSpacingSize);
		float height = static_cast<float>((maxHeight * rows) + rowSpacingSize);
		winrt::Size desiredSize{ width, height };

		// Required to perform regular grid measurement, but ignore result.
		base_type::MeasureOverride(desiredSize);

		return desiredSize;
	}

	winrt::Size UniformGrid::ArrangeOverride(winrt::Size finalSize)
	{
		// Have grid to the bulk of our heavy lifting.
		auto size = base_type::ArrangeOverride(finalSize);

		// Make sure all overflown elements have no size.
		for (const auto& child : _overflow)
		{
			child.Arrange(winrt::Rect{ 0,0,0,0 });
		}

		_overflow.clear(); // Reset for next time.

		return size;
	}

	// Provides the next spot in the boolean array with a 'false' value.
	std::generator<std::tuple<int, int>> UniformGrid::GetFreeSpot(TakenSpotsReferenceHolder* arrayref, int firstcolumn, bool topdown)
	{
		if (topdown)
		{
			int rows = arrayref->Height();

			// Layout spots from Top-Bottom, Left-Right (right-left handled automatically by Grid with Flow-Direction).
			// Effectively transpose the Grid Layout.
			for (int c = 0; c < arrayref->Width(); c++)
			{
				int start = (c == 0 && firstcolumn > 0 && firstcolumn < rows) ? firstcolumn : 0;
				for (int r = start; r < rows; r++)
				{
					if (!arrayref->Get(r, c))
					{
						co_yield{ r, c };
					}
				}
			}
		}
		else
		{
			int columns = arrayref->Width();

			// Layout spots as normal from Left-Right.
			// (right-left handled automatically by Grid with Flow-Direction
			// during its layout, internal model is always left-right).
			for (int r = 0; r < arrayref->Height(); r++)
			{
				int start = (r == 0 && firstcolumn > 0 && firstcolumn < columns) ? firstcolumn : 0;
				for (int c = start; c < columns; c++)
				{
					if (!arrayref->Get(r, c))
					{
						co_yield{ r, c };
					}
				}
			}
		}
	}

	// Based on the number of visible children,
	// returns the dimensions of the
	// grid we need to hold all elements.
	std::tuple<int, int> UniformGrid::GetDimensions(std::span<winrt::FrameworkElement> visible, int rows, int cols, int firstColumn)
	{
		// If a dimension isn't specified, we need to figure out the other one (or both).
		if (rows == 0 || cols == 0)
		{
			// Calculate the size & area of all objects in the grid to know how much space we need.
			auto count = std::max(1, std::accumulate(visible.begin(), visible.end(), 0, [](int sum, auto const& item)
			{
				return sum + winrt::Grid::GetRowSpan(item) * winrt::Grid::GetColumnSpan(item);
			}));

			if (rows == 0)
			{
				if (cols > 0)
				{
					// Bound check
					auto first = (firstColumn >= cols || firstColumn < 0) ? 0 : firstColumn;

					// If we have columns but no rows, calculate rows based on column offset and number of children.
					rows = (count + first + (cols - 1)) / cols;
					return { rows, cols };
				}
				else
				{
					// Otherwise, determine square layout if both are zero.
					int size = static_cast<int>(std::ceil(std::sqrt(count)));

					// Figure out if firstColumn is in bounds
					auto first = (firstColumn >= size || firstColumn < 0) ? 0 : firstColumn;

					rows = static_cast<int>(std::ceil(std::sqrt(count + first)));
					return { rows, rows };
				}
			}
			else if (cols == 0)
			{
				// If we have rows and no columns, then calculate columns needed based on rows
				cols = (count + (rows - 1)) / rows;

				// Now that we know a rough size of our shape, see if the FirstColumn effects that:
				auto first = (firstColumn >= cols || firstColumn < 0) ? 0 : firstColumn;

				cols = (count + first + (rows - 1)) / rows;
			}
		}

		return { rows, cols };
	}

	// Used to interleave specified row dimensions with automatic rows added to use
	// underlying Grid layout for main arrange of UniformGrid.
	void UniformGrid::SetupRowDefinitions(uint32_t rows)
	{
		auto rowDefinitions = RowDefinitions();
		auto rowCount = rowDefinitions.Size();
		// Mark initial definitions so we don't erase them.
		for (const auto& rd : rowDefinitions)
		{
			if (GetAutoLayout(rd) == nullptr)
			{
				SetAutoLayout(rd, false);
			}
		}

		// Remove non-autolayout rows we've added and then add them in the right spots.
		if (rows != rowCount)
		{
			for (int r = static_cast<int>(rowCount) - 1; r >= 0; r--)
			{
				if (auto layout = GetAutoLayout(rowDefinitions.GetAt(r)); layout && layout.GetBoolean())
				{
					rowDefinitions.RemoveAt(r);
				}
			}

			for (auto r = rowCount; r < rows; r++)
			{
				winrt::RowDefinition rd;
				SetAutoLayout(rd, true);
				rowDefinitions.InsertAt(r, rd);
			}
		}
	}

	// Used to interleave specified column dimensions with automatic columns added to use
	// underlying Grid layout for main arrange of UniformGrid.
	void UniformGrid::SetupColumnDefinitions(uint32_t columns)
	{
		auto columnDefinitions = ColumnDefinitions();
		auto columnCount = columnDefinitions.Size();
		// Mark initial definitions so we don't erase them.
		for (const auto& cd : columnDefinitions)
		{
			if (GetAutoLayout(cd) == nullptr)
			{
				SetAutoLayout(cd, false);
			}
		}

		// Remove non-autolayout columns we've added and then add them in the right spots.
		if (columns != columnCount)
		{
			for (int c = static_cast<int>(columnCount) - 1; c >= 0; c--)
			{
				if (auto layout = GetAutoLayout(columnDefinitions.GetAt(c)); layout && layout.GetBoolean())
				{
					columnDefinitions.RemoveAt(c);
				}
			}

			for (auto c = columnCount; c < columns; c++)
			{
				auto cd = ColumnDefinition();
				SetAutoLayout(cd, true);
				columnDefinitions.InsertAt(c, cd);
			}
		}
	}
}
