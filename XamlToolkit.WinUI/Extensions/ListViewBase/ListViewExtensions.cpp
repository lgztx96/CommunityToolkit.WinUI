#include "pch.h"
#include "winrt_module_imports.h"
#include "ListViewExtensions.h"
#if __has_include("ListViewExtensions.g.cpp")
#include "ListViewExtensions.g.cpp"
#endif
#ifdef __INTELLISENSE__
#include <memory>
#include <wil/resource.h>
#endif
#include "../common.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
	void ListViewExtensions::OnAlternateColorPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto items = listViewBase.Items().try_as<winrt::IObservableVector<winrt::IInspectable>>();
			if (const auto& iter = _trackedListViews.find(items); iter != _trackedListViews.end())
			{
				_trackedListViews.erase(iter);
			}
		
			if (GetAlternateColor(listViewBase))
			{
				auto ctx = std::make_unique<EventContext>();
				ctx->_listViewBase = listViewBase;

				ctx->_colorContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, { &ListViewExtensions::ColorContainerContentChanging });
				ctx->_itemsVectorChangedRevoker = listViewBase.Items().VectorChanged(winrt::auto_revoke, &ListViewExtensions::ColorItemsVectorChanged);
				ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded_AltRow);

				_trackedListViews.try_emplace(items, std::move(ctx));
			}
		}
	}

	void ListViewExtensions::ColorContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args)
	{
		auto itemContainer = args.ItemContainer().try_as<winrt::Control>();
		SetItemContainerBackground(sender, itemContainer, args.ItemIndex());
	}

	void ListViewExtensions::OnAlternateItemTemplatePropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto iter = _listViewEventContexts.find(listViewBase);
			
			if (iter != _listViewEventContexts.end())
			{
				iter->second->_itemTemplateContentChangingRevoker.revoke();
			}

			if (GetAlternateItemTemplate(listViewBase))
			{
				EventContext* ctx = iter != _listViewEventContexts.end() ? iter->second.get()
					: _listViewEventContexts.emplace(listViewBase, new EventContext()).first->second.get();
				ctx->_itemTemplateContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, &ListViewExtensions::ItemTemplateContainerContentChanging);
				ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
			}
		}
	}

	void ListViewExtensions::ItemTemplateContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args)
	{
		if (args.ItemIndex() % 2 == 0)
		{
			args.ItemContainer().ContentTemplate(GetAlternateItemTemplate(sender));
		}
		else
		{
			args.ItemContainer().ContentTemplate(sender.ItemTemplate());
		}
	}

	void ListViewExtensions::OnItemContainerStretchDirectionPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto iter = _listViewEventContexts.find(listViewBase);
			if (iter != _listViewEventContexts.end())
			{
				iter->second->_stretchDirectionContentChangingRevoker.revoke();
			}

			if (GetAlternateItemTemplate(listViewBase))
			{
				EventContext* ctx = iter != _listViewEventContexts.end() ? iter->second.get()
					: _listViewEventContexts.emplace(listViewBase, new EventContext()).first->second.get();
				ctx->_stretchDirectionContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, &ListViewExtensions::ItemContainerStretchDirectionChanging);
				ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
			}
		}
	}

	void ListViewExtensions::ItemContainerStretchDirectionChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args)
	{
		auto stretchDirection = GetItemContainerStretchDirection(sender);

		if (stretchDirection == ItemContainerStretchDirection::Vertical || stretchDirection == ItemContainerStretchDirection::Both)
		{
			args.ItemContainer().VerticalContentAlignment(winrt::VerticalAlignment::Stretch);
		}

		if (stretchDirection == ItemContainerStretchDirection::Horizontal || stretchDirection == ItemContainerStretchDirection::Both)
		{
			args.ItemContainer().HorizontalContentAlignment(winrt::HorizontalAlignment::Stretch);
		}
	}

	void ListViewExtensions::OnListViewBaseUnloaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			_listViewEventContexts.erase(listViewBase);
		}
	}

	void ListViewExtensions::OnListViewBaseUnloaded_AltRow(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			_trackedListViews.erase(listViewBase.Items());
		}
	}

	void ListViewExtensions::ColorItemsVectorChanged(winrt::IObservableVector<winrt::IInspectable> const& sender, winrt::IVectorChangedEventArgs const& args)
	{
		// If the index is at the end we can ignore
		if (args.Index() == (sender.Size() - 1))
		{
			return;
		}

		// Only need to handle Inserted and Removed because we'll handle everything else in the
		// ColorContainerContentChanging method
		if ((args.CollectionChange() == winrt::CollectionChange::ItemInserted) || (args.CollectionChange() == winrt::CollectionChange::ItemRemoved))
		{
			auto iter = _trackedListViews.find(sender);
			if (iter == _trackedListViews.end())
			{
				return;
			}

			if (const auto& listViewBase = iter->second->_listViewBase)
			{
				uint32_t index = args.Index();
				for (uint32_t i = index; i < sender.Size(); i++)
				{
					// Get item container or element at index
					winrt::Control itemContainer{ nullptr };

					if (auto container = listViewBase.ContainerFromIndex(i).try_as<winrt::Control>())
					{
						itemContainer = container;
					}
					else if (auto item = listViewBase.Items().GetAt(i).try_as<winrt::Control>())
					{
						itemContainer = item;
					}

					if (itemContainer)
					{
						SetItemContainerBackground(listViewBase, itemContainer, i);
					}
				}
			}
		}
	}

	void ListViewExtensions::SetItemContainerBackground(winrt::ListViewBase const& sender, winrt::Control const& itemContainer, int itemIndex)
	{
		auto brush = itemIndex % 2 == 0 ? GetAlternateColor(sender) : nullptr;
		itemContainer.Background(brush);
		if (auto rootBorder = DependencyObjectEx::FindDescendant<winrt::Border>(itemContainer))
		{
			rootBorder.Background(brush);
		}
	}

	void ListViewExtensions::DeselectAll(winrt::ListViewBase const& list)
	{
		switch (list.SelectionMode())
		{
		case winrt::ListViewSelectionMode::Single:
			list.SelectedItem(nullptr);
			break;
		case winrt::ListViewSelectionMode::Multiple:
		case winrt::ListViewSelectionMode::Extended:
			list.DeselectRange(winrt::ItemIndexRange(0, list.Items().Size()));
			break;
		}
	}

	void ListViewExtensions::SelectAllSafe(winrt::ListViewBase const& list)
	{
		switch (list.SelectionMode())
		{
		case winrt::ListViewSelectionMode::Single:
			list.SelectedItem(list.Items().Size() > 0 ? list.Items().GetAt(0) : nullptr);
			break;
		case winrt::ListViewSelectionMode::Multiple:
		case winrt::ListViewSelectionMode::Extended:
			list.SelectRange(winrt::ItemIndexRange(0, list.Items().Size()));
			break;
		}
	}

	void ListViewExtensions::OnCommandPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		auto listViewBase = sender.try_as<winrt::ListViewBase>();

		if (listViewBase == nullptr)
		{
			return;
		}

		if (auto oldCommand = args.OldValue().try_as<winrt::ICommand>())
		{
			if (auto iter = _commandTokens.find(listViewBase); iter != _commandTokens.end())
			{
				_commandTokens.erase(iter);
			}
		}

		if (auto newCommand = args.NewValue().try_as<winrt::ICommand>())
		{
			auto revoker = std::make_unique<winrt::ListViewBase::ItemClick_revoker>();
			*revoker = listViewBase.ItemClick(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseItemClick);
			
			_commandTokens.insert_or_assign(listViewBase, std::move(revoker));
		}
	}

	void ListViewExtensions::OnListViewBaseItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto command = GetCommand(listViewBase);
			if (listViewBase == nullptr || command == nullptr)
			{
				return;
			}

			if (command.CanExecute(e.ClickedItem()))
			{
				command.Execute(e.ClickedItem());
			}
		}
	}

	winrt::IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithIndexAsync(winrt::ListViewBase const& listViewBase, int index, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
	{
		auto items = listViewBase.Items();
		if (index > (static_cast<int>(items.Size()) - 1))
		{
			index = items.Size() - 1;
		}

		if (index < -static_cast<int>(items.Size()))
		{
			index = -static_cast<int>(items.Size());
		}

		index = (index < 0) ? (index + items.Size()) : index;

		bool isVirtualizing = false;
		double previousXOffset = false, previousYOffset = false;

		auto scrollViewer = DependencyObjectEx::FindDescendant<winrt::ScrollViewer>(listViewBase);
		auto selectorItem = listViewBase.ContainerFromIndex(index).try_as<winrt::SelectorItem>();

		if (scrollViewer == nullptr)
		{
			co_return;
		}

		// If selectorItem is null then the panel is virtualized.
		// So in order to get the container of the item we need to scroll to that item first and then use ContainerFromIndex
		if (selectorItem == nullptr)
		{
			isVirtualizing = true;

			previousXOffset = scrollViewer.HorizontalOffset();
			previousYOffset = scrollViewer.VerticalOffset();

			wil::shared_event completionEvent(wil::EventOptions::ManualReset);
			winrt::ScrollViewer::ViewChanged_revoker viewChangedRevoker;
			auto ViewChanged = [completionEvent](winrt::IInspectable const&, winrt::ScrollViewerViewChangedEventArgs const&)
			{
				completionEvent.SetEvent();
			};

			try
			{
				winrt::apartment_context context;
				viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, ViewChanged);
				listViewBase.ScrollIntoView(items.GetAt(index), winrt::ScrollIntoViewAlignment::Leading);
				co_await winrt::resume_on_signal(completionEvent.get());
				co_await context;
			}
			catch (...)
			{

			}

			selectorItem = listViewBase.ContainerFromIndex(index).try_as<winrt::SelectorItem>();
		}

		auto transform = selectorItem.TransformToVisual(scrollViewer.Content().try_as<winrt::UIElement>());
		auto position = transform.TransformPoint(winrt::Point(0, 0));

		// Scrolling back to previous position
		if (isVirtualizing)
		{
			co_await ChangeViewAsync(scrollViewer, previousXOffset, previousYOffset, std::nullopt, true);
		}

		auto listViewBaseWidth = listViewBase.ActualWidth();
		auto selectorItemWidth = selectorItem.ActualWidth();
		auto listViewBaseHeight = listViewBase.ActualHeight();
		auto selectorItemHeight = selectorItem.ActualHeight();

		previousXOffset = scrollViewer.HorizontalOffset();
		previousYOffset = scrollViewer.VerticalOffset();

		auto minXPosition = position.X - listViewBaseWidth + selectorItemWidth;
		auto minYPosition = position.Y - listViewBaseHeight + selectorItemHeight;

		auto maxXPosition = position.X;
		auto maxYPosition = position.Y;

		double finalXPosition, finalYPosition;

		// If the Item is in view and scrollIfVisible is false then we don't need to scroll
		if (!scrollIfVisible && (previousXOffset <= maxXPosition && previousXOffset >= minXPosition) && (previousYOffset <= maxYPosition && previousYOffset >= minYPosition))
		{
			finalXPosition = previousXOffset;
			finalYPosition = previousYOffset;
		}
		else
		{
			switch (itemPlacement)
			{
			case ScrollItemPlacement::Default:
				if (previousXOffset <= maxXPosition && previousXOffset >= minXPosition)
				{
					finalXPosition = previousXOffset + additionalHorizontalOffset;
				}
				else if (std::abs(previousXOffset - minXPosition) < std::abs(previousXOffset - maxXPosition))
				{
					finalXPosition = minXPosition + additionalHorizontalOffset;
				}
				else
				{
					finalXPosition = maxXPosition + additionalHorizontalOffset;
				}

				if (previousYOffset <= maxYPosition && previousYOffset >= minYPosition)
				{
					finalYPosition = previousYOffset + additionalVerticalOffset;
				}
				else if (std::abs(previousYOffset - minYPosition) < std::abs(previousYOffset - maxYPosition))
				{
					finalYPosition = minYPosition + additionalVerticalOffset;
				}
				else
				{
					finalYPosition = maxYPosition + additionalVerticalOffset;
				}

				break;

			case ScrollItemPlacement::Left:
				finalXPosition = maxXPosition + additionalHorizontalOffset;
				finalYPosition = previousYOffset + additionalVerticalOffset;
				break;

			case ScrollItemPlacement::Top:
				finalXPosition = previousXOffset + additionalHorizontalOffset;
				finalYPosition = maxYPosition + additionalVerticalOffset;
				break;

			case ScrollItemPlacement::Center:
			{
				auto centreX = (listViewBaseWidth - selectorItemWidth) / 2.0;
				auto centreY = (listViewBaseHeight - selectorItemHeight) / 2.0;
				finalXPosition = maxXPosition - centreX + additionalHorizontalOffset;
				finalYPosition = maxYPosition - centreY + additionalVerticalOffset;
				break;
			}
			case ScrollItemPlacement::Right:
				finalXPosition = minXPosition + additionalHorizontalOffset;
				finalYPosition = previousYOffset + additionalVerticalOffset;
				break;

			case ScrollItemPlacement::Bottom:
				finalXPosition = previousXOffset + additionalHorizontalOffset;
				finalYPosition = minYPosition + additionalVerticalOffset;
				break;

			default:
				finalXPosition = previousXOffset + additionalHorizontalOffset;
				finalYPosition = previousYOffset + additionalVerticalOffset;
				break;
			}
		}

		co_await ChangeViewAsync(scrollViewer, finalXPosition, finalYPosition, std::nullopt, disableAnimation);
	}

	winrt::IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithItemAsync(winrt::ListViewBase const& listViewBase, winrt::IInspectable const& item, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
	{
		uint32_t index;
		if (listViewBase.Items().IndexOf(item, index))
		{
			co_await SmoothScrollIntoViewWithIndexAsync(listViewBase, index, itemPlacement, disableAnimation, scrollIfVisible, additionalHorizontalOffset, additionalVerticalOffset);
		}
	}

	winrt::IAsyncAction ListViewExtensions::ChangeViewAsync(winrt::ScrollViewer const& scrollViewer, std::optional<double> horizontalOffset, std::optional<double> verticalOffset, std::optional<float> zoomFactor, bool disableAnimation)
	{
		if (horizontalOffset > scrollViewer.ScrollableWidth())
		{
			horizontalOffset = scrollViewer.ScrollableWidth();
		}
		else if (horizontalOffset < 0)
		{
			horizontalOffset = 0;
		}

		if (verticalOffset > scrollViewer.ScrollableHeight())
		{
			verticalOffset = scrollViewer.ScrollableHeight();
		}
		else if (verticalOffset < 0)
		{
			verticalOffset = 0;
		}

		// MUST check this and return immediately, otherwise this async task will never complete because ViewChanged event won't get triggered
		if (horizontalOffset == scrollViewer.HorizontalOffset() && verticalOffset == scrollViewer.VerticalOffset())
		{
			co_return;
		}

		wil::shared_event completionEvent(wil::EventOptions::ManualReset);
		winrt::ScrollViewer::ViewChanged_revoker viewChangedRevoker;
		auto ViewChanged = [completionEvent](winrt::IInspectable const&, winrt::ScrollViewerViewChangedEventArgs const& e)
		{
			if (!e.IsIntermediate())
			{
				completionEvent.SetEvent();
			}
		};

		winrt::apartment_context context;
		viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, ViewChanged);
		scrollViewer.ChangeView(horizontalOffset, verticalOffset, zoomFactor, disableAnimation);
		co_await winrt::resume_on_signal(completionEvent.get());
		co_await context;
	}
}
