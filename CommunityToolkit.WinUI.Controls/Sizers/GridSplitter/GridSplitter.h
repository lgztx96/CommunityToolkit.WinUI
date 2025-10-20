#pragma once

#include "GridSplitter.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include "../SizerBase.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;

	struct GridSplitter : GridSplitterT<GridSplitter, implementation::SizerBase>
	{
		double _currentSize;
		double _siblingSize;

		GridResizeDirection _resizeDirection;
		GridResizeBehavior _resizeBehavior;

		GridSplitter()
			: _currentSize(0)
			, _siblingSize(0)
			, _resizeDirection(GridResizeDirection::Auto)
			, _resizeBehavior(GridResizeBehavior::BasedOnAlignment) {
		}

		static void OnResizeDirectionPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

		GridResizeDirection ResizeDirection() { return winrt::unbox_value<GridResizeDirection>(GetValue(ResizeDirectionProperty)); }
		void ResizeDirection(GridResizeDirection value) { SetValue(ResizeDirectionProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> ResizeDirectionProperty =
			DependencyProperty::Register(L"GridResizeDirection", winrt::xaml_typename<GridResizeDirection>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(GridResizeDirection::Auto)));

		GridResizeBehavior ResizeBehavior() { return winrt::unbox_value<GridResizeBehavior>(GetValue(ResizeBehaviorProperty)); }
		void ResizeBehavior(GridResizeBehavior  value) { SetValue(ResizeBehaviorProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> ResizeBehaviorProperty =
			DependencyProperty::Register(L"GridResizeBehavior", winrt::xaml_typename<GridResizeBehavior>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(GridResizeBehavior::BasedOnAlignment)));

		int ParentLevel() { return winrt::unbox_value<int>(GetValue(ParentLevelProperty)); }
		void ParentLevel(int value) { SetValue(ParentLevelProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> ParentLevelProperty =
			DependencyProperty::Register(L"ParentLevel", winrt::xaml_typename<int>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0)));

		static bool IsStarColumn(ColumnDefinition const& definition);

		static bool IsStarRow(RowDefinition const& definition);

		bool SetColumnWidth(ColumnDefinition const& columnDefinition, double newWidth, GridUnitType unitType);

		bool IsValidColumnWidth(ColumnDefinition const& columnDefinition, double newWidth);

		bool SetRowHeight(RowDefinition const& rowDefinition, double newHeight, GridUnitType unitType);

		bool IsValidRowHeight(RowDefinition const& rowDefinition, double newHeight);

		// Return the targeted Column based on the resize behavior
		int GetTargetedColumn();

		// Return the sibling Row based on the resize behavior
		int GetTargetedRow();

		// Return the sibling Column based on the resize behavior
		int GetSiblingColumn();

		// Return the sibling Row based on the resize behavior
		int GetSiblingRow();

		// Gets index based on resize behavior for first targeted row/column
		int GetTargetIndex(int currentIndex);

		// Gets index based on resize behavior for second targeted row/column
		int GetSiblingIndex(int currentIndex);

		// Checks the control alignment and Width/Height to detect the control resize direction columns/rows
		GridResizeDirection GetResizeDirection();

		// Get the resize behavior (Which columns/rows should be resized) based on alignment and Direction
		GridResizeBehavior GetResizeBehavior();

		void OnLoaded(RoutedEventArgs const& e);

		void OnDragStarting();

		bool OnDragVertical(double verticalChange);

		bool OnDragHorizontal(double horizontalChange);

		FrameworkElement TargetControl();

		Grid Resizable();

		ColumnDefinition CurrentColumn();

		ColumnDefinition SiblingColumn();

		RowDefinition CurrentRow();

		RowDefinition SiblingRow();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct GridSplitter : GridSplitterT<GridSplitter, implementation::GridSplitter>
	{
	};
}
