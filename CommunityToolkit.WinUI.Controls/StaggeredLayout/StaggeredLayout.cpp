
// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "StaggeredLayout.h"
#if __has_include("StaggeredLayout.g.cpp")
#include "StaggeredLayout.g.cpp"
#endif
#include "StaggeredItem.h"
#include <algorithm>
#include <unordered_set>
#include <iostream>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void StaggeredLayout::ColumnSpacing(double value)
	{
		SetValue(ColumnSpacingProperty, winrt::box_value(value));
	}

	double StaggeredLayout::ColumnSpacing()
	{
		return winrt::unbox_value<double>(GetValue(ColumnSpacingProperty));
	}

	void StaggeredLayout::RowSpacing(double value)
	{
		SetValue(RowSpacingProperty, winrt::box_value(value));
	}

	double StaggeredLayout::RowSpacing()
	{
		return winrt::unbox_value<double>(GetValue(RowSpacingProperty));
	}

	void StaggeredLayout::DesiredColumnWidth(double value)
	{
		SetValue(DesiredColumnWidthProperty, winrt::box_value(value));
	}

	double StaggeredLayout::DesiredColumnWidth()
	{
		return winrt::unbox_value<double>(GetValue(DesiredColumnWidthProperty));
	}

	StaggeredLayoutItemsStretch StaggeredLayout::ItemsStretch()
	{
		return winrt::unbox_value<StaggeredLayoutItemsStretch>(GetValue(ItemsStretchProperty));
	}

	void StaggeredLayout::ItemsStretch(StaggeredLayoutItemsStretch value)
	{
		SetValue(ItemsStretchProperty, winrt::box_value(value));
	}

	void StaggeredLayout::OnDependencyPropertyChanged(
		DependencyObject const& sender,
		DependencyPropertyChangedEventArgs const&)
	{
		if (auto d = sender.try_as<Microsoft::UI::Xaml::Controls::ILayoutProtected>())
			d.InvalidateMeasure();
	}

	int StaggeredLayout::GetColumnIndex(std::span<const double> columnHeights)
	{
		int columnIndex = 0;
		double height = columnHeights[0];
		for (int j = 1; j < columnHeights.size(); j++)
		{
			if (columnHeights[j] < height)
			{
				columnIndex = j;
				height = columnHeights[j];
			}
		}

		return columnIndex;
	}

	void StaggeredLayout::InitializeForContextCore(Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context)
	{
		auto state = context.LayoutState();
		winrt::com_ptr<StaggeredLayoutState> stackState = nullptr;
		if (state)
		{
			stackState = GetAsStackState(state);
		}

		if (!stackState)
		{
			assert(state == NULL);

			// Custom deriving layouts could potentially be stateful.
			// If that is the case, we will just create the base state required by UniformGridLayout ourselves.
			stackState = winrt::make_self<StaggeredLayoutState>();
			stackState->InitializeForContext(context);
			context.LayoutState(*stackState);
		}
	}

	void StaggeredLayout::UninitializeForContextCore(Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context)
	{
		context.LayoutState(nullptr);
	}

	Windows::Foundation::Size StaggeredLayout::MeasureOverride(
		Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
		Windows::Foundation::Size const& availableSize)
	{
		if (context.ItemCount() == 0)
		{
			return Windows::Foundation::Size(availableSize.Width, 0);
		}

		if ((context.RealizationRect().Width == 0) && (context.RealizationRect().Height == 0))
		{
			return Windows::Foundation::Size(availableSize.Width, 0.0f);
		}

		const auto state = GetAsStackState(context.LayoutState());

		float availableWidth = availableSize.Width;
		float availableHeight = availableSize.Height;

		double columnWidth;
		int numColumns;
		if (ItemsStretch() == StaggeredLayoutItemsStretch::None)
		{
			columnWidth = std::isnan(DesiredColumnWidth()) ? availableWidth : std::min<double>(DesiredColumnWidth(), availableWidth);
			numColumns = std::max(1, static_cast<int>(std::floor(availableWidth / (columnWidth + ColumnSpacing()))));
		}
		else
		{
			if (std::isnan(DesiredColumnWidth()) || DesiredColumnWidth() > availableWidth)
			{
				columnWidth = availableWidth;
				numColumns = 1;
			}
			else
			{
				// 0.0001 is to prevent floating point errors
				auto tempAvailableWidth = availableWidth + ColumnSpacing() - 0.0001;
				numColumns = static_cast<int>(std::floor(tempAvailableWidth / (DesiredColumnWidth() + ColumnSpacing())));
				columnWidth = tempAvailableWidth / numColumns - ColumnSpacing();
			}
		}

		if (std::abs(columnWidth - state->ColumnWidth()) > std::numeric_limits<double>::epsilon())
		{
			// The items will need to be remeasured
			state->Clear();
		}

		state->ColumnWidth(columnWidth);

		double totalWidth = state->ColumnWidth() + ((numColumns - 1) * (state->ColumnWidth() + ColumnSpacing()));
		if (totalWidth > availableWidth)
		{
			numColumns--;
		}
		else if (std::isinf(availableWidth))
		{
			availableWidth = static_cast<float>(totalWidth);
		}

		if (numColumns != state->NumberOfColumns())
		{
			// The items will not need to be remeasured, but they will need to go into new columns
			state->ClearColumns();
		}

		if (std::abs(RowSpacing() - state->RowSpacing()) > std::numeric_limits<double>::epsilon())
		{
			// If the RowSpacing changes the height of the rows will be different.
			// The columns stores the height so we'll want to clear them out to
			// get the proper height
			state->ClearColumns();
			state->RowSpacing(RowSpacing());
		}

		auto columnHeights = std::make_unique<double[]>(numColumns);
		auto itemsPerColumn = std::make_unique<int[]>(numColumns);
		std::unordered_set<int> deadColumns;

		for (int i = 0; i < context.ItemCount(); i++)
		{
			int columnIndex = GetColumnIndex({ columnHeights.get(), static_cast<size_t>(numColumns) });

			bool measured = false;
			StaggeredItem* item = state->GetItemAt(i);
			if (item->Height() == 0)
			{
				// Item has not been measured yet. Get the element and store the values
				item->Element(context.GetOrCreateElementAt(i));
				item->Element().Measure(Windows::Foundation::Size(static_cast<float>(state->ColumnWidth()), static_cast<float>(availableHeight)));
				item->Height(item->Element().DesiredSize().Height);
				measured = true;
			}

			double spacing = itemsPerColumn[columnIndex] > 0 ? RowSpacing() : 0;
			item->Top(columnHeights[columnIndex] + spacing);
			double bottom = item->Top() + item->Height();
			columnHeights[columnIndex] = bottom;
			itemsPerColumn[columnIndex]++;
			state->AddItemToColumn(item, columnIndex);

			if (bottom < RectHelper::GetTop(context.RealizationRect()))
			{
				// The bottom of the element is above the realization area
				if (item->Element() != nullptr)
				{
					context.RecycleElement(item->Element());
					item->Element(nullptr);
				}
			}
			else if (item->Top() > RectHelper::GetBottom(context.RealizationRect()))
			{
				// The top of the element is below the realization area
				if (item->Element() != nullptr)
				{
					context.RecycleElement(item->Element());
					item->Element(nullptr);
				}

				deadColumns.insert(columnIndex);
			}
			else if (!measured)
			{
				// We ALWAYS want to measure an item that will be in the bounds
				item->Element(context.GetOrCreateElementAt(i));
				item->Element().Measure(Windows::Foundation::Size(static_cast<float>(state->ColumnWidth()), static_cast<float>(availableHeight)));
				if (item->Height() != item->Element().DesiredSize().Height)
				{
					// this item changed size; we need to recalculate layout for everything after this
					state->RemoveFromIndex(i + 1);
					item->Height(item->Element().DesiredSize().Height);
					columnHeights[columnIndex] = item->Top() + item->Height();
				}
			}

			if (deadColumns.size() == numColumns)
			{
				break;
			}
		}

		auto desiredHeight = static_cast<float>(state->GetHeight());

		return { availableWidth, desiredHeight };
	}

	Windows::Foundation::Size StaggeredLayout::ArrangeOverride(
		Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
		Windows::Foundation::Size const& finalSize)
	{
		if ((context.RealizationRect().Width == 0) && (context.RealizationRect().Height == 0))
		{
			return finalSize;
		}

		auto state = GetAsStackState(context.LayoutState());

		for (int columnIndex = 0; columnIndex < state->NumberOfColumns(); columnIndex++)
		{
			const auto& layout = state->GetColumnLayout(columnIndex);
			for (int i = 0; i < layout.size(); i++)
			{
				const StaggeredItem* item = layout[i];
				double bottom = item->Top() + item->Height();
				if (bottom < RectHelper::GetTop(context.RealizationRect()))
				{
					// element is above the realization bounds
					continue;
				}

				if (item->Top() <= RectHelper::GetBottom(context.RealizationRect()))
				{
					double itemHorizontalOffset = (state->ColumnWidth() * columnIndex) + (ColumnSpacing() * columnIndex);

					Windows::Foundation::Rect bounds = Windows::Foundation::Rect(
						static_cast<float>(itemHorizontalOffset),
						static_cast<float>(item->Top()),
						static_cast<float>(state->ColumnWidth()),
						static_cast<float>(item->Height()));
					UIElement element = context.GetOrCreateElementAt(item->Index());
					element.Arrange(bounds);
				}
				else
				{
					break;
				}
			}
		}

		return finalSize;
	}

	void StaggeredLayout::OnItemsChangedCore(
		Microsoft::UI::Xaml::Controls::VirtualizingLayoutContext const& context,
		Windows::Foundation::IInspectable const&,
		Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventArgs const& args)
	{
		const auto state = GetAsStackState(context.LayoutState());
		switch (args.Action())
		{
		case winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Add:
			state->RemoveFromIndex(args.NewStartingIndex());
			break;
		case winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Replace:
			state->RemoveFromIndex(args.NewStartingIndex());
			state->RecycleElementAt(args.NewStartingIndex());
			break;
		case winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Move:
		{
			int minIndex = std::min(args.NewStartingIndex(), args.OldStartingIndex());
			int maxIndex = std::max(args.NewStartingIndex(), args.OldStartingIndex());
			state->RemoveRange(minIndex, maxIndex);
		}
		break;
		case winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Remove:
			state->RemoveFromIndex(args.OldStartingIndex());
			break;
		case winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Reset:
			state->Clear();
			break;
		}
	}
}
