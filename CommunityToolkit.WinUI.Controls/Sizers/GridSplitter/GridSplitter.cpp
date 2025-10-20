#include "pch.h"
#include "GridSplitter.h"
#if __has_include("GridSplitter.g.cpp")
#include "GridSplitter.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void GridSplitter::OnResizeDirectionPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto splitter = d.try_as<Controls::GridSplitter>()) {

			if (auto direction = e.NewValue().try_as<GridResizeDirection>(); direction != GridResizeDirection::Auto) {
				// Update base classes property based on specific polyfill for GridSplitter
				splitter.Orientation(
					direction == GridResizeDirection::Rows ?
					Orientation::Horizontal :
					Orientation::Vertical);
			}
		}
	}

	bool GridSplitter::IsStarColumn(ColumnDefinition const& definition)
	{
		return (definition.GetValue(ColumnDefinition::WidthProperty()).as<GridLength>()).GridUnitType == GridUnitType::Star;
	}

	bool GridSplitter::IsStarRow(RowDefinition const& definition)
	{
		return (definition.GetValue(RowDefinition::HeightProperty()).as<GridLength>()).GridUnitType == GridUnitType::Star;
	}

	bool GridSplitter::SetColumnWidth(ColumnDefinition const& columnDefinition, double newWidth, GridUnitType unitType)
	{
		auto minWidth = columnDefinition.MinWidth();
		if (!std::isnan(minWidth) && newWidth < minWidth)
		{
			newWidth = minWidth;
		}

		auto maxWidth = columnDefinition.MaxWidth();
		if (!std::isnan(maxWidth) && newWidth > maxWidth)
		{
			newWidth = maxWidth;
		}

		if (newWidth > ActualWidth())
		{
			columnDefinition.Width(GridLength(newWidth, unitType));
			return true;
		}

		return false;
	}

	bool GridSplitter::IsValidColumnWidth(ColumnDefinition const& columnDefinition, double newWidth)
	{
		auto minWidth = columnDefinition.MinWidth();
		if (!std::isnan(minWidth) && newWidth < minWidth)
		{
			return false;
		}

		auto maxWidth = columnDefinition.MaxWidth();
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

	bool GridSplitter::SetRowHeight(RowDefinition const& rowDefinition, double newHeight, GridUnitType unitType)
	{
		auto minHeight = rowDefinition.MinHeight();
		if (!std::isnan(minHeight) && newHeight < minHeight)
		{
			newHeight = minHeight;
		}

		auto maxWidth = rowDefinition.MaxHeight();
		if (!std::isnan(maxWidth) && newHeight > maxWidth)
		{
			newHeight = maxWidth;
		}

		if (newHeight > ActualHeight())
		{
			rowDefinition.Height(GridLength(newHeight, unitType));
			return true;
		}

		return false;
	}

	bool GridSplitter::IsValidRowHeight(RowDefinition const& rowDefinition, double newHeight)
	{
		auto minHeight = rowDefinition.MinHeight();
		if (!std::isnan(minHeight) && newHeight < minHeight)
		{
			return false;
		}

		auto maxHeight = rowDefinition.MaxHeight();
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
	int GridSplitter::GetTargetedColumn()
	{
		auto currentIndex = Grid::GetColumn(TargetControl());
		return GetTargetIndex(currentIndex);
	}

	// Return the sibling Row based on the resize behavior
	int GridSplitter::GetTargetedRow()
	{
		auto currentIndex = Grid::GetRow(TargetControl());
		return GetTargetIndex(currentIndex);
	}

	// Return the sibling Column based on the resize behavior
	int GridSplitter::GetSiblingColumn()
	{
		auto currentIndex = Grid::GetColumn(TargetControl());
		return GetSiblingIndex(currentIndex);
	}

	// Return the sibling Row based on the resize behavior
	int GridSplitter::GetSiblingRow()
	{
		auto currentIndex = Grid::GetRow(TargetControl());
		return GetSiblingIndex(currentIndex);
	}

	// Gets index based on resize behavior for first targeted row/column
	int GridSplitter::GetTargetIndex(int currentIndex)
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
	int GridSplitter::GetSiblingIndex(int currentIndex)
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
	GridResizeDirection GridSplitter::GetResizeDirection()
	{
		GridResizeDirection direction = ResizeDirection();

		if (direction == GridResizeDirection::Auto)
		{
			// When HorizontalAlignment is Left, Right or Center, resize Columns
			if (HorizontalAlignment() != HorizontalAlignment::Stretch)
			{
				direction = GridResizeDirection::Columns;
			}

			// When VerticalAlignment is Top, Bottom or Center, resize Rows
			else if (VerticalAlignment() != VerticalAlignment::Stretch)
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
	GridResizeBehavior GridSplitter::GetResizeBehavior()
	{
		GridResizeBehavior resizeBehavior = ResizeBehavior();

		if (resizeBehavior == GridResizeBehavior::BasedOnAlignment)
		{
			if (_resizeDirection == GridResizeDirection::Columns)
			{
				switch (HorizontalAlignment())
				{
				case HorizontalAlignment::Left:
					resizeBehavior = GridResizeBehavior::PreviousAndCurrent;
					break;
				case HorizontalAlignment::Right:
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
				case VerticalAlignment::Top:
					resizeBehavior = GridResizeBehavior::PreviousAndCurrent;
					break;
				case VerticalAlignment::Bottom:
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

	void GridSplitter::OnLoaded([[maybe_unused]] RoutedEventArgs const& e)
	{
		_resizeDirection = GetResizeDirection();
		Orientation(_resizeDirection == GridResizeDirection::Rows ?
			Orientation::Horizontal : Orientation::Vertical);
		_resizeBehavior = GetResizeBehavior();
	}

	void GridSplitter::OnDragStarting()
	{
		_resizeDirection = GetResizeDirection();
		Orientation(_resizeDirection == GridResizeDirection::Rows ?
			Orientation::Horizontal : Orientation::Vertical);
		_resizeBehavior = GetResizeBehavior();

		// Record starting points
		if (Orientation() == Orientation::Horizontal)
		{
			_currentSize = CurrentRow() ? CurrentRow().ActualHeight() : -1;
			_siblingSize = SiblingRow() ? SiblingRow().ActualHeight() : -1;
		}
		else
		{
			_currentSize = CurrentColumn() ? CurrentColumn().ActualWidth() : -1;
			_siblingSize = SiblingColumn() ? SiblingColumn().ActualWidth() : -1;
		}
	}

	bool GridSplitter::OnDragVertical(double verticalChange)
	{
		if (CurrentRow() == nullptr || SiblingRow() == nullptr || Resizable() == nullptr)
		{
			return false;
		}

		auto currentChange = _currentSize + verticalChange;
		auto siblingChange = _siblingSize + (verticalChange * -1); // sibling moves opposite

		// Would changing the columnn sizes violate the constraints?
		if (!IsValidRowHeight(CurrentRow(), currentChange) || !IsValidRowHeight(SiblingRow(), siblingChange))
		{
			return false;
		}

		// NOTE: If the column contains another row with Star sizing, it's not enough to just change current.
		// The change will flow to the Star sized item and not to the sibling if the sibling is fixed-size.
		// So, we need to explicitly apply the change to the sibling.

		// if current row has fixed height then resize it
		if (!IsStarRow(CurrentRow()))
		{
			// No need to check for the row Min height because it is automatically respected
			auto changed = SetRowHeight(CurrentRow(), currentChange, GridUnitType::Pixel);

			if (!IsStarRow(SiblingRow()))
			{
				changed = SetRowHeight(SiblingRow(), siblingChange, GridUnitType::Pixel);
			}

			return changed;
		}

		// if sibling row has fixed width then resize it
		else if (!IsStarRow(SiblingRow()))
		{
			return SetRowHeight(SiblingRow(), siblingChange, GridUnitType::Pixel);
		}

		// if both row haven't fixed height (auto *)
		else
		{
			// change current row height to the new height with respecting the auto
			// change sibling row height to the new height relative to current row
			// respect the other star row height by setting it's height to it's actual height with stars

			// We need to validate current and sibling height to not cause any unexpected behavior
			if (!IsValidRowHeight(CurrentRow(), currentChange) ||
				!IsValidRowHeight(SiblingRow(), siblingChange))
			{
				return false;
			}

			for (auto rowDefinition : Resizable().RowDefinitions())
			{
				if (rowDefinition == CurrentRow())
				{
					SetRowHeight(CurrentRow(), currentChange, GridUnitType::Star);
				}
				else if (rowDefinition == SiblingRow())
				{
					SetRowHeight(SiblingRow(), siblingChange, GridUnitType::Star);
				}
				else if (IsStarRow(rowDefinition))
				{
					rowDefinition.Height(GridLength(rowDefinition.ActualHeight(), GridUnitType::Star));
				}
			}

			return true;
		}
	}

	bool GridSplitter::OnDragHorizontal(double horizontalChange)
	{
		if (CurrentColumn() == nullptr || SiblingColumn() == nullptr || Resizable() == nullptr)
		{
			return false;
		}

		auto currentChange = _currentSize + horizontalChange;
		auto siblingChange = _siblingSize + (horizontalChange * -1); // sibling moves opposite

		// Would changing the columnn sizes violate the constraints?
		if (!IsValidColumnWidth(CurrentColumn(), currentChange) || !IsValidColumnWidth(SiblingColumn(), siblingChange))
		{
			return false;
		}

		// NOTE: If the row contains another column with Star sizing, it's not enough to just change current.
		// The change will flow to the Star sized item and not to the sibling if the sibling is fixed-size.
		// So, we need to explicitly apply the change to the sibling.

		// if current column has fixed width then resize it
		if (!IsStarColumn(CurrentColumn()))
		{
			// No need to check for the Column Min width because it is automatically respected
			auto changed = SetColumnWidth(CurrentColumn(), currentChange, GridUnitType::Pixel);

			if (!IsStarColumn(SiblingColumn()))
			{
				changed = SetColumnWidth(SiblingColumn(), siblingChange, GridUnitType::Pixel);
			}

			return changed;
		}

		// if sibling column has fixed width then resize it
		else if (!IsStarColumn(SiblingColumn()))
		{
			return SetColumnWidth(SiblingColumn(), siblingChange, GridUnitType::Pixel);
		}

		// if both column haven't fixed width (auto *)
		else
		{
			// change current column width to the new width with respecting the auto
			// change sibling column width to the new width relative to current column
			// respect the other star column width by setting it's width to it's actual width with stars

			// We need to validate current and sibling width to not cause any unexpected behavior
			if (!IsValidColumnWidth(CurrentColumn(), currentChange) ||
				!IsValidColumnWidth(SiblingColumn(), siblingChange))
			{
				return false;
			}

			for (auto columnDefinition : Resizable().ColumnDefinitions())
			{
				if (columnDefinition == CurrentColumn())
				{
					SetColumnWidth(CurrentColumn(), currentChange, GridUnitType::Star);
				}
				else if (columnDefinition == SiblingColumn())
				{
					SetColumnWidth(SiblingColumn(), siblingChange, GridUnitType::Star);
				}
				else if (IsStarColumn(columnDefinition))
				{
					columnDefinition.Width(GridLength(columnDefinition.ActualWidth(), GridUnitType::Star));
				}
			}

			return true;
		}
	}

	FrameworkElement GridSplitter::TargetControl()
	{
		if (ParentLevel() == 0)
		{
			return *this;
		}

		// TODO: Can we just use our Visual/Logical Tree extensions for this?
		auto parent = Parent();
		for (int i = 2; i < ParentLevel(); i++) // TODO: Why is this 2? We need better documentation on ParentLevel
		{
			if (auto frameworkElement = parent.try_as<FrameworkElement>())
			{
				parent = frameworkElement.Parent();
			}
			else
			{
				break;
			}
		}

		return parent.try_as<FrameworkElement>();
	}


	Grid GridSplitter::Resizable()
	{
		if (auto target = TargetControl())
		{
			if (auto parent = target.Parent())
			{
				return parent.try_as<Grid>();
			}
		};

		return nullptr;
	}

	ColumnDefinition GridSplitter::CurrentColumn()
	{
		if (Resizable() == nullptr)
		{
			return nullptr;
		}

		auto gridSplitterTargetedColumnIndex = GetTargetedColumn();

		if ((gridSplitterTargetedColumnIndex >= 0)
			&& (static_cast<uint32_t>(gridSplitterTargetedColumnIndex) < Resizable().ColumnDefinitions().Size()))
		{
			return Resizable().ColumnDefinitions().GetAt(gridSplitterTargetedColumnIndex);
		}

		return nullptr;
	}

	ColumnDefinition GridSplitter::SiblingColumn()
	{
		if (Resizable() == nullptr)
		{
			return nullptr;
		}

		auto gridSplitterSiblingColumnIndex = GetSiblingColumn();

		if ((gridSplitterSiblingColumnIndex >= 0)
			&& (static_cast<uint32_t>(gridSplitterSiblingColumnIndex) < Resizable().ColumnDefinitions().Size()))
		{
			return Resizable().ColumnDefinitions().GetAt(gridSplitterSiblingColumnIndex);
		}

		return nullptr;
	}

	RowDefinition GridSplitter::CurrentRow()
	{
		if (Resizable() == nullptr)
		{
			return nullptr;
		}

		auto gridSplitterTargetedRowIndex = GetTargetedRow();

		if ((gridSplitterTargetedRowIndex >= 0)
			&& (static_cast<uint32_t>(gridSplitterTargetedRowIndex) < Resizable().RowDefinitions().Size()))
		{
			return Resizable().RowDefinitions().GetAt(gridSplitterTargetedRowIndex);
		}

		return nullptr;
	}

	RowDefinition GridSplitter::SiblingRow()
	{
		if (Resizable() == nullptr)
		{
			return nullptr;
		}

		auto gridSplitterSiblingRowIndex = GetSiblingRow();

		if ((gridSplitterSiblingRowIndex >= 0)
			&& (static_cast<uint32_t>(gridSplitterSiblingRowIndex) < Resizable().RowDefinitions().Size()))
		{
			return Resizable().RowDefinitions().GetAt(gridSplitterSiblingRowIndex);
		}

		return nullptr;
	}
}
