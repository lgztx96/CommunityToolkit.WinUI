#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "GridSplitter.h"
#if __has_include("GridSplitter.g.cpp")
#include "GridSplitter.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> GridSplitter::ResizeDirectionProperty =
		winrt::DependencyProperty::Register(
			L"GridResizeDirection",
			winrt::xaml_typename<GridResizeDirection>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(GridResizeDirection::Auto), &GridSplitter::OnResizeDirectionPropertyChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> GridSplitter::ResizeBehaviorProperty =
		winrt::DependencyProperty::Register(
			L"GridResizeBehavior",
			winrt::xaml_typename<GridResizeBehavior>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(GridResizeBehavior::BasedOnAlignment)));

	const wil::single_threaded_property<winrt::DependencyProperty> GridSplitter::ParentLevelProperty =
		winrt::DependencyProperty::Register(
			L"ParentLevel",
			winrt::xaml_typename<int>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(0)));

	GridSplitter::GridSplitter()
		: _currentSize(0)
		, _siblingSize(0)
		, _resizeDirection(GridResizeDirection::Auto)
		, _resizeBehavior(GridResizeBehavior::BasedOnAlignment)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	void GridSplitter::OnResizeDirectionPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto splitter = d.try_as<Controls::GridSplitter>()) 
		{
			if (const auto direction = e.NewValue().try_as<GridResizeDirection>(); 
				direction && *direction != GridResizeDirection::Auto) {
				// Update base classes property based on specific polyfill for GridSplitter
				splitter.Orientation(
					direction == GridResizeDirection::Rows ?
					winrt::Orientation::Horizontal :
					winrt::Orientation::Vertical);
			}
		}
	}

	bool GridSplitter::IsStarColumn(winrt::ColumnDefinition const& definition)
	{
		return definition.Width().GridUnitType == winrt::GridUnitType::Star;
	}

	bool GridSplitter::IsStarRow(winrt::RowDefinition const& definition)
	{
		return definition.Height().GridUnitType == winrt::GridUnitType::Star;
	}

	bool GridSplitter::SetColumnWidth(winrt::ColumnDefinition const& columnDefinition, double newWidth, winrt::GridUnitType unitType)
	{
		const auto minWidth = columnDefinition.MinWidth();
		if (!std::isnan(minWidth) && newWidth < minWidth)
		{
			newWidth = minWidth;
		}

		const auto maxWidth = columnDefinition.MaxWidth();
		if (!std::isnan(maxWidth) && newWidth > maxWidth)
		{
			newWidth = maxWidth;
		}

		if (newWidth > ActualWidth())
		{
			columnDefinition.Width(winrt::GridLength(newWidth, unitType));
			return true;
		}

		return false;
	}

	bool GridSplitter::IsValidColumnWidth(winrt::ColumnDefinition const& columnDefinition, double newWidth)
	{
		const auto minWidth = columnDefinition.MinWidth();
		if (!std::isnan(minWidth) && newWidth < minWidth)
		{
			return false;
		}

		const auto maxWidth = columnDefinition.MaxWidth();
		if (!std::isnan(maxWidth) && newWidth > maxWidth)
		{
			return false;
		}

		if (newWidth <= ActualWidth())
		{
			return false;
		}

		return true;
	}

	bool GridSplitter::SetRowHeight(winrt::RowDefinition const& rowDefinition, double newHeight, winrt::GridUnitType unitType)
	{
		const auto minHeight = rowDefinition.MinHeight();
		if (!std::isnan(minHeight) && newHeight < minHeight)
		{
			newHeight = minHeight;
		}

		const auto maxHeight = rowDefinition.MaxHeight();
		if (!std::isnan(maxHeight) && newHeight > maxHeight)
		{
			newHeight = maxHeight;
		}

		if (newHeight > ActualHeight())
		{
			rowDefinition.Height(winrt::GridLength(newHeight, unitType));
			return true;
		}

		return false;
	}

	bool GridSplitter::IsValidRowHeight(winrt::RowDefinition const& rowDefinition, double newHeight)
	{
		const auto minHeight = rowDefinition.MinHeight();
		if (!std::isnan(minHeight) && newHeight < minHeight)
		{
			return false;
		}

		const auto maxHeight = rowDefinition.MaxHeight();
		if (!std::isnan(maxHeight) && newHeight > maxHeight)
		{
			return false;
		}

		if (newHeight <= ActualHeight())
		{
			return false;
		}

		return true;
	}

	// Return the targeted Column based on the resize behavior
	int GridSplitter::GetTargetedColumn() const
	{
		const auto currentIndex = winrt::Grid::GetColumn(TargetControl());
		return GetTargetIndex(currentIndex);
	}

	// Return the sibling Row based on the resize behavior
	int GridSplitter::GetTargetedRow() const
	{
		const auto currentIndex = winrt::Grid::GetRow(TargetControl());
		return GetTargetIndex(currentIndex);
	}

	// Return the sibling Column based on the resize behavior
	int GridSplitter::GetSiblingColumn() const
	{
		const auto currentIndex = winrt::Grid::GetColumn(TargetControl());
		return GetSiblingIndex(currentIndex);
	}

	// Return the sibling Row based on the resize behavior
	int GridSplitter::GetSiblingRow() const
	{
		const auto currentIndex = winrt::Grid::GetRow(TargetControl());
		return GetSiblingIndex(currentIndex);
	}

	// Gets index based on resize behavior for first targeted row/column
	int GridSplitter::GetTargetIndex(int currentIndex) const
	{
		switch (_resizeBehavior)
		{
		case GridResizeBehavior::CurrentAndNext:
			return currentIndex;
		case GridResizeBehavior::PreviousAndNext:
			return currentIndex - 1;
		case GridResizeBehavior::PreviousAndCurrent:
			return currentIndex - 1;
		default:
			return -1;
		}
	}

	// Gets index based on resize behavior for second targeted row/column
	int GridSplitter::GetSiblingIndex(int currentIndex) const
	{
		switch (_resizeBehavior)
		{
		case GridResizeBehavior::CurrentAndNext:
			return currentIndex + 1;
		case GridResizeBehavior::PreviousAndNext:
			return currentIndex + 1;
		case GridResizeBehavior::PreviousAndCurrent:
			return currentIndex;
		default:
			return -1;
		}
	}

	// Checks the control alignment and Width/Height to detect the control resize direction columns/rows
	GridResizeDirection GridSplitter::GetResizeDirection() const
	{
		GridResizeDirection direction = ResizeDirection();

		if (direction == GridResizeDirection::Auto)
		{
			// When HorizontalAlignment is Left, Right or Center, resize Columns
			if (HorizontalAlignment() != winrt::HorizontalAlignment::Stretch)
			{
				direction = GridResizeDirection::Columns;
			}

			// When VerticalAlignment is Top, Bottom or Center, resize Rows
			else if (VerticalAlignment() != winrt::VerticalAlignment::Stretch)
			{
				direction = GridResizeDirection::Rows;
			}

			// Check Width vs Height
			else if (ActualWidth() <= ActualHeight())
			{
				direction = GridResizeDirection::Columns;
			}
			else
			{
				direction = GridResizeDirection::Rows;
			}
		}

		return direction;
	}

	// Get the resize behavior (Which columns/rows should be resized) based on alignment and Direction
	GridResizeBehavior GridSplitter::GetResizeBehavior() const
	{
		GridResizeBehavior resizeBehavior = ResizeBehavior();

		if (resizeBehavior == GridResizeBehavior::BasedOnAlignment)
		{
			if (_resizeDirection == GridResizeDirection::Columns)
			{
				switch (HorizontalAlignment())
				{
				case winrt::HorizontalAlignment::Left:
					resizeBehavior = GridResizeBehavior::PreviousAndCurrent;
					break;
				case winrt::HorizontalAlignment::Right:
					resizeBehavior = GridResizeBehavior::CurrentAndNext;
					break;
				default:
					resizeBehavior = GridResizeBehavior::PreviousAndNext;
					break;
				}
			}

			// resize direction is vertical
			else
			{
				switch (VerticalAlignment())
				{
				case winrt::VerticalAlignment::Top:
					resizeBehavior = GridResizeBehavior::PreviousAndCurrent;
					break;
				case winrt::VerticalAlignment::Bottom:
					resizeBehavior = GridResizeBehavior::CurrentAndNext;
					break;
				default:
					resizeBehavior = GridResizeBehavior::PreviousAndNext;
					break;
				}
			}
		}

		return resizeBehavior;
	}

	void GridSplitter::OnLoaded([[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		_resizeDirection = GetResizeDirection();
		Orientation(_resizeDirection == GridResizeDirection::Rows ?
			winrt::Orientation::Horizontal : winrt::Orientation::Vertical);
		_resizeBehavior = GetResizeBehavior();
	}

	void GridSplitter::OnDragStarting()
	{
		_resizeDirection = GetResizeDirection();
		Orientation(_resizeDirection == GridResizeDirection::Rows ?
			winrt::Orientation::Horizontal : winrt::Orientation::Vertical);
		_resizeBehavior = GetResizeBehavior();

		// Record starting points
		if (Orientation() == winrt::Orientation::Horizontal)
		{
			const auto currentRow = CurrentRow();
			const auto siblingRow = SiblingRow();

			_currentSize = currentRow ? currentRow.ActualHeight() : -1;
			_siblingSize = siblingRow ? siblingRow.ActualHeight() : -1;
		}
		else
		{
			const auto currentColumn = CurrentColumn();
			const auto siblingColumn = SiblingColumn();

			_currentSize = currentColumn ? currentColumn.ActualWidth() : -1;
			_siblingSize = siblingColumn ? siblingColumn.ActualWidth() : -1;
		}
	}

	bool GridSplitter::OnDragVertical(double verticalChange)
	{
		const auto currentRow = CurrentRow();
		const auto siblingRow = SiblingRow();
		const auto resizable = Resizable();

		if (currentRow == nullptr || siblingRow == nullptr || resizable == nullptr)
		{
			return false;
		}

		const auto currentChange = _currentSize + verticalChange;
		const auto siblingChange = _siblingSize + (verticalChange * -1); // sibling moves opposite

		// Would changing the columnn sizes violate the constraints?
		if (!IsValidRowHeight(currentRow, currentChange) || !IsValidRowHeight(siblingRow, siblingChange))
		{
			return false;
		}

		// NOTE: If the column contains another row with Star sizing, it's not enough to just change current.
		// The change will flow to the Star sized item and not to the sibling if the sibling is fixed-size.
		// So, we need to explicitly apply the change to the sibling.

		// if current row has fixed height then resize it
		if (!IsStarRow(currentRow))
		{
			// No need to check for the row Min height because it is automatically respected
			auto changed = SetRowHeight(currentRow, currentChange, winrt::GridUnitType::Pixel);

			if (!IsStarRow(siblingRow))
			{
				changed = SetRowHeight(siblingRow, siblingChange, winrt::GridUnitType::Pixel);
			}

			return changed;
		}

		// if sibling row has fixed width then resize it
		else if (!IsStarRow(siblingRow))
		{
			return SetRowHeight(siblingRow, siblingChange, winrt::GridUnitType::Pixel);
		}

		// if both row haven't fixed height (auto *)
		else
		{
			// change current row height to the new height with respecting the auto
			// change sibling row height to the new height relative to current row
			// respect the other star row height by setting it's height to it's actual height with stars

			// We need to validate current and sibling height to not cause any unexpected behavior
			if (!IsValidRowHeight(currentRow, currentChange) ||
				!IsValidRowHeight(siblingRow, siblingChange))
			{
				return false;
			}

			for (const auto& rowDefinition : resizable.RowDefinitions())
			{
				if (rowDefinition == currentRow)
				{
					SetRowHeight(currentRow, currentChange, winrt::GridUnitType::Star);
				}
				else if (rowDefinition == siblingRow)
				{
					SetRowHeight(siblingRow, siblingChange, winrt::GridUnitType::Star);
				}
				else if (IsStarRow(rowDefinition))
				{
					rowDefinition.Height(winrt::GridLength(rowDefinition.ActualHeight(), winrt::GridUnitType::Star));
				}
			}

			return true;
		}
	}

	bool GridSplitter::OnDragHorizontal(double horizontalChange)
	{
		const auto currentColumn = CurrentColumn();
		const auto siblingColumn = SiblingColumn();
		const auto resizable = Resizable();

		if (currentColumn == nullptr || siblingColumn == nullptr || resizable == nullptr)
		{
			return false;
		}

		const auto currentChange = _currentSize + horizontalChange;
		const auto siblingChange = _siblingSize + (horizontalChange * -1); // sibling moves opposite

		// Would changing the columnn sizes violate the constraints?
		if (!IsValidColumnWidth(currentColumn, currentChange) || !IsValidColumnWidth(siblingColumn, siblingChange))
		{
			return false;
		}

		// NOTE: If the row contains another column with Star sizing, it's not enough to just change current.
		// The change will flow to the Star sized item and not to the sibling if the sibling is fixed-size.
		// So, we need to explicitly apply the change to the sibling.

		// if current column has fixed width then resize it
		if (!IsStarColumn(currentColumn))
		{
			// No need to check for the Column Min width because it is automatically respected
			auto changed = SetColumnWidth(currentColumn, currentChange, winrt::GridUnitType::Pixel);

			if (!IsStarColumn(siblingColumn))
			{
				changed = SetColumnWidth(siblingColumn, siblingChange, winrt::GridUnitType::Pixel);
			}

			return changed;
		}

		// if sibling column has fixed width then resize it
		else if (!IsStarColumn(siblingColumn))
		{
			return SetColumnWidth(siblingColumn, siblingChange, winrt::GridUnitType::Pixel);
		}

		// if both column haven't fixed width (auto *)
		else
		{
			// change current column width to the new width with respecting the auto
			// change sibling column width to the new width relative to current column
			// respect the other star column width by setting it's width to it's actual width with stars

			// We need to validate current and sibling width to not cause any unexpected behavior
			if (!IsValidColumnWidth(currentColumn, currentChange) ||
				!IsValidColumnWidth(siblingColumn, siblingChange))
			{
				return false;
			}

			for (const auto& columnDefinition : resizable.ColumnDefinitions())
			{
				if (columnDefinition == currentColumn)
				{
					SetColumnWidth(currentColumn, currentChange, winrt::GridUnitType::Star);
				}
				else if (columnDefinition == siblingColumn)
				{
					SetColumnWidth(siblingColumn, siblingChange, winrt::GridUnitType::Star);
				}
				else if (IsStarColumn(columnDefinition))
				{
					columnDefinition.Width(winrt::GridLength(columnDefinition.ActualWidth(), winrt::GridUnitType::Star));
				}
			}

			return true;
		}
	}

	winrt::FrameworkElement GridSplitter::TargetControl() const
	{
		const auto parentLevel = ParentLevel();
		if (parentLevel == 0)
		{
			return *this;
		}

		// TODO: Can we just use our Visual/Logical Tree extensions for this?
		auto parent = Parent();
		for (int i = 2; i < parentLevel; i++) // TODO: Why is this 2? We need better documentation on ParentLevel
		{
			if (const auto frameworkElement = parent.try_as<winrt::FrameworkElement>())
			{
				parent = frameworkElement.Parent();
			}
			else
			{
				break;
			}
		}

		return parent.try_as<winrt::FrameworkElement>();
	}

	winrt::Grid GridSplitter::Resizable() const
	{
		if (const auto target = TargetControl())
		{
			if (const auto parent = target.Parent())
			{
				return parent.try_as<winrt::Grid>();
			}
		};

		return nullptr;
	}

	winrt::ColumnDefinition GridSplitter::CurrentColumn() const
	{
		if (const auto resizable = Resizable())
		{
			if (const auto gridSplitterTargetedColumnIndex = GetTargetedColumn(); gridSplitterTargetedColumnIndex >= 0)
			{
				const auto columnDefinitions = resizable.ColumnDefinitions();
				const auto index = static_cast<uint32_t>(gridSplitterTargetedColumnIndex);

				if (index < columnDefinitions.Size())
				{
					return columnDefinitions.GetAt(index);
				}
			}
		}

		return nullptr;
	}

	winrt::ColumnDefinition GridSplitter::SiblingColumn() const
	{
		if (const auto resizable = Resizable())
		{
			if (const auto gridSplitterSiblingColumnIndex = GetSiblingColumn(); gridSplitterSiblingColumnIndex >= 0)
			{
				const auto columnDefinitions = resizable.ColumnDefinitions();
				const auto index = static_cast<uint32_t>(gridSplitterSiblingColumnIndex);

				if (index < columnDefinitions.Size())
				{
					return columnDefinitions.GetAt(index);
				}
			}
		}

		return nullptr;
	}

	winrt::RowDefinition GridSplitter::CurrentRow() const
	{
		if (const auto resizable = Resizable())
		{
			if (const auto gridSplitterTargetedRowIndex = GetTargetedRow(); gridSplitterTargetedRowIndex >= 0)
			{
				const auto rowDefinitions = resizable.RowDefinitions();
				const auto index = static_cast<uint32_t>(gridSplitterTargetedRowIndex);

				if (index < rowDefinitions.Size())
				{
					return rowDefinitions.GetAt(index);
				}
			}
		}

		return nullptr;
	}

	winrt::RowDefinition GridSplitter::SiblingRow() const
	{
		if (const auto resizable = Resizable())
		{
			if (const auto gridSplitterSiblingRowIndex = GetSiblingRow(); gridSplitterSiblingRowIndex >= 0)
			{
				const auto rowDefinitions = resizable.RowDefinitions();
				const auto index = static_cast<uint32_t>(gridSplitterSiblingRowIndex);

				if (index < rowDefinitions.Size())
				{
					return rowDefinitions.GetAt(index);
				}
			}
		}

		return nullptr;
	}
}
