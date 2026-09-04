#pragma once

#include "GridSplitter.g.h"
#include "../SizerBase.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct GridSplitter : GridSplitterT<GridSplitter, implementation::SizerBase>
	{
		GridSplitter();

		static void OnResizeDirectionPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

		GridResizeDirection ResizeDirection() const { return winrt::unbox_value<GridResizeDirection>(GetValue(ResizeDirectionProperty())); }
		void ResizeDirection(GridResizeDirection const& value) { SetValue(ResizeDirectionProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> ResizeDirectionProperty;

		GridResizeBehavior ResizeBehavior() const { return winrt::unbox_value<GridResizeBehavior>(GetValue(ResizeBehaviorProperty())); }
		void ResizeBehavior(GridResizeBehavior const& value) { SetValue(ResizeBehaviorProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> ResizeBehaviorProperty;

		int ParentLevel() const { return winrt::unbox_value<int>(GetValue(ParentLevelProperty())); }
		void ParentLevel(int value) { SetValue(ParentLevelProperty(), winrt::box_value(value)); }

		static const wil::single_threaded_property<winrt::DependencyProperty> ParentLevelProperty;

		static bool IsStarColumn(winrt::ColumnDefinition const& definition);

		static bool IsStarRow(winrt::RowDefinition const& definition);

		bool SetColumnWidth(winrt::ColumnDefinition const& columnDefinition, double newWidth, winrt::GridUnitType unitType);

		bool IsValidColumnWidth(winrt::ColumnDefinition const& columnDefinition, double newWidth);

		bool SetRowHeight(winrt::RowDefinition const& rowDefinition, double newHeight, winrt::GridUnitType unitType);

		bool IsValidRowHeight(winrt::RowDefinition const& rowDefinition, double newHeight);

		// Return the targeted Column based on the resize behavior
		int GetTargetedColumn() const;

		// Return the sibling Row based on the resize behavior
		int GetTargetedRow() const;

		// Return the sibling Column based on the resize behavior
		int GetSiblingColumn() const;

		// Return the sibling Row based on the resize behavior
		int GetSiblingRow() const;

		// Gets index based on resize behavior for first targeted row/column
		int GetTargetIndex(int currentIndex) const;

		// Gets index based on resize behavior for second targeted row/column
		int GetSiblingIndex(int currentIndex) const;

		// Checks the control alignment and Width/Height to detect the control resize direction columns/rows
		GridResizeDirection GetResizeDirection() const;

		// Get the resize behavior (Which columns/rows should be resized) based on alignment and Direction
		GridResizeBehavior GetResizeBehavior() const;

		void OnLoaded(winrt::RoutedEventArgs const& e);

		void OnDragStarting();

		bool OnDragVertical(double verticalChange);

		bool OnDragHorizontal(double horizontalChange);

		winrt::FrameworkElement TargetControl() const;

		winrt::Grid Resizable() const;

		winrt::ColumnDefinition CurrentColumn() const;

		winrt::ColumnDefinition SiblingColumn() const;

		winrt::RowDefinition CurrentRow() const;

		winrt::RowDefinition SiblingRow() const;

	private:
		double _currentSize;
		double _siblingSize;

		GridResizeDirection _resizeDirection;
		GridResizeBehavior _resizeBehavior;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct GridSplitter : GridSplitterT<GridSplitter, implementation::GridSplitter>
	{
	};
}
