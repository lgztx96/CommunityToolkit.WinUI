#include "pch.h"
#include "ListViewExtensions.h"
#if __has_include("ListViewExtensions.g.cpp")
#include "ListViewExtensions.g.cpp"
#endif
#include "../common.h"

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::CommunityToolkit::WinUI::implementation
{
	void ListViewExtensions::OnAlternateColorPropertyChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
		{
			auto items = listViewBase.Items().try_as<IObservableVector<IInspectable>>();
			if (const auto& iter = _itemsForList.find(items); iter != _itemsForList.end())
			{
				_itemsForList.erase(iter);
			}

			auto ctx = std::make_unique<EventContext>();
			ctx->_listViewBase = listViewBase;

			if (AlternateColorProperty != nullptr)
			{
				ctx->_colorContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, { &ListViewExtensions::ColorContainerContentChanging });
				ctx->_itemsVectorChangedRevoker = listViewBase.Items().VectorChanged(winrt::auto_revoke, &ListViewExtensions::ColorItemsVectorChanged);
				ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
			}

			_itemsForList.try_emplace(items, std::move(ctx));
		}
	}

	void ListViewExtensions::ColorContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
	{
		auto itemContainer = args.ItemContainer().try_as<Control>();
		SetItemContainerBackground(sender, itemContainer, args.ItemIndex());
	}

	void ListViewExtensions::OnAlternateItemTemplatePropertyChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
		{
			for (const auto& [_, ctx] : _itemsForList)
			{
				if (ctx->_listViewBase == listViewBase)
				{
					ctx->_itemTemplateContentChangingRevoker.revoke();
					ctx->_unloadedRevoker.revoke();

					if (AlternateItemTemplateProperty != nullptr)
					{
						ctx->_itemTemplateContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, &ListViewExtensions::ItemTemplateContainerContentChanging);
						ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
					}

					break;
				}
			}
		}
	}

	void ListViewExtensions::ItemTemplateContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
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

	void ListViewExtensions::OnItemContainerStretchDirectionPropertyChanged(DependencyObject const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
		{
			for (const auto& [_, ctx] : _itemsForList) {
				if (ctx->_listViewBase == listViewBase)
				{
					ctx->_stretchDirectionContentChangingRevoker.revoke();
					ctx->_unloadedRevoker.revoke();

					if (ItemContainerStretchDirectionProperty != nullptr)
					{
						ctx->_stretchDirectionContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, &ListViewExtensions::ItemContainerStretchDirectionChanging);
						ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
					}

					break;
				}
			}
		}
	}

	void ListViewExtensions::ItemContainerStretchDirectionChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args)
	{
		auto stretchDirection = GetItemContainerStretchDirection(sender);

		if (stretchDirection == ItemContainerStretchDirection::Vertical || stretchDirection == ItemContainerStretchDirection::Both)
		{
			args.ItemContainer().VerticalContentAlignment(VerticalAlignment::Stretch);
		}

		if (stretchDirection == ItemContainerStretchDirection::Horizontal || stretchDirection == ItemContainerStretchDirection::Both)
		{
			args.ItemContainer().HorizontalContentAlignment(HorizontalAlignment::Stretch);
		}
	}

	void ListViewExtensions::OnListViewBaseUnloaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
		{
			_itemsForList.erase(listViewBase.Items());
		}
	}

	void ListViewExtensions::ColorItemsVectorChanged(IObservableVector<IInspectable> const& sender, IVectorChangedEventArgs const& args)
	{
		// If the index is at the end we can ignore
		if (args.Index() == (sender.Size() - 1))
		{
			return;
		}

		// Only need to handle Inserted and Removed because we'll handle everything else in the
		// ColorContainerContentChanging method
		if ((args.CollectionChange() == CollectionChange::ItemInserted) || (args.CollectionChange() == CollectionChange::ItemRemoved))
		{
			auto iter = _itemsForList.find(sender);
			if (iter == _itemsForList.end())
			{
				return;
			}

			if (const auto& listViewBase = iter->second->_listViewBase)
			{
				uint32_t index = args.Index();
				for (uint32_t i = index; i < sender.Size(); i++)
				{
					auto itemContainer = listViewBase.ContainerFromIndex(i).try_as<Control>();
					if (itemContainer != nullptr)
					{
						SetItemContainerBackground(listViewBase, itemContainer, i);
					}
				}
			}
		}
	}

	void ListViewExtensions::SetItemContainerBackground(ListViewBase const& sender, Control const& itemContainer, int itemIndex)
	{
		if (itemIndex % 2 == 0)
		{
			itemContainer.Background(GetAlternateColor(sender));
			auto rootBorder = DependencyObjectEx::FindDescendant<Border>(itemContainer);
			if (rootBorder != nullptr)
			{
				rootBorder.Background(GetAlternateColor(sender));
			}
		}
		else
		{
			itemContainer.Background(nullptr);
			auto rootBorder = DependencyObjectEx::FindDescendant<Border>(itemContainer);
			if (rootBorder != nullptr)
			{
				rootBorder.Background(nullptr);
			}
		}
	}

	void ListViewExtensions::DeselectAll(ListViewBase const& list)
	{
		switch (list.SelectionMode())
		{
		case ListViewSelectionMode::Single:
			list.SelectedItem(nullptr);
			break;
		case ListViewSelectionMode::Multiple:
		case ListViewSelectionMode::Extended:
			list.DeselectRange(ItemIndexRange(0, list.Items().Size()));
			break;
		}
	}

	void ListViewExtensions::SelectAllSafe(ListViewBase const& list)
	{
		switch (list.SelectionMode())
		{
		case ListViewSelectionMode::Single:
			list.SelectedItem(list.Items().Size() > 0 ? list.Items().GetAt(0) : nullptr);
			break;
		case ListViewSelectionMode::Multiple:
		case ListViewSelectionMode::Extended:
			list.SelectRange(ItemIndexRange(0, list.Items().Size()));
			break;
		}
	}

	void ListViewExtensions::OnCommandPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args)
	{
		auto listViewBase = sender.try_as<ListViewBase>();

		if (listViewBase == nullptr)
		{
			return;
		}

		if (auto iter = _commandTokens.find(listViewBase); iter != _commandTokens.end())
		{
			_commandTokens.erase(iter);
		}

		if (auto oldCommand = args.OldValue().try_as<ICommand>())
		{

		}

		if (auto newCommand = args.NewValue().try_as<ICommand>())
		{
			auto revoker = std::make_unique<ListViewBase::ItemClick_revoker>();
			*revoker = listViewBase.ItemClick(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseItemClick);
			
			_commandTokens.emplace(listViewBase, std::move(revoker));
		}
	}

	void ListViewExtensions::OnListViewBaseItemClick(IInspectable const& sender, ItemClickEventArgs const& e)
	{
		if (auto listViewBase = sender.try_as<ListViewBase>())
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

	IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithIndexAsync(ListViewBase const& listViewBase, int index, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
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

		auto scrollViewer = DependencyObjectEx::FindDescendant<ScrollViewer>(listViewBase);
		auto selectorItem = listViewBase.ContainerFromIndex(index).try_as<SelectorItem>();

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

			auto tcs = std::make_shared<winrt::handle>(CreateEventW(nullptr, true, false, nullptr));
			ScrollViewer::ViewChanged_revoker viewChangedRevoker;
			auto ViewChanged = [tcs](IInspectable const&, ScrollViewerViewChangedEventArgs const&)
				{
					SetEvent(tcs->get());
				};

			try
			{
				winrt::apartment_context context;
				viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, ViewChanged);
				listViewBase.ScrollIntoView(items.GetAt(index), ScrollIntoViewAlignment::Leading);
				co_await winrt::resume_on_signal(tcs->get());
				co_await context;
			}
			catch (...)
			{

			}

			selectorItem = listViewBase.ContainerFromIndex(index).try_as<SelectorItem>();
		}

		auto transform = selectorItem.TransformToVisual(scrollViewer.Content().try_as<UIElement>());
		auto position = transform.TransformPoint(Point(0, 0));

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

	IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithItemAsync(ListViewBase const& listViewBase, IInspectable const& item, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
	{
		uint32_t index;
		if (listViewBase.Items().IndexOf(item, index))
		{
			co_await SmoothScrollIntoViewWithIndexAsync(listViewBase, index, itemPlacement, disableAnimation, scrollIfVisible, additionalHorizontalOffset, additionalVerticalOffset);
		}
	}

	IAsyncAction ListViewExtensions::ChangeViewAsync(ScrollViewer const& scrollViewer, std::optional<double> horizontalOffset, std::optional<double> verticalOffset, std::optional<float> zoomFactor, bool disableAnimation)
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

		auto tcs = std::make_shared<winrt::handle>(CreateEventW(nullptr, true, false, nullptr));
		ScrollViewer::ViewChanged_revoker viewChangedRevoker;
		auto ViewChanged = [tcs](IInspectable const&, ScrollViewerViewChangedEventArgs const& e)
			{
				if (!e.IsIntermediate())
				{
					SetEvent(tcs->get());
				}
			};

		winrt::apartment_context context;
		viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, ViewChanged);
		scrollViewer.ChangeView(horizontalOffset, verticalOffset, zoomFactor, disableAnimation);
		co_await winrt::resume_on_signal(tcs->get());
		co_await context;
	}
}
