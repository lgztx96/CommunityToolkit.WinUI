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
#include "../Tree/DependencyObjectExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::com_ptr<EventContext> ListViewExtensions::GetEventContext(winrt::ListViewBase const& listViewBase)
	{
		const auto value = listViewBase.GetValue(EventContextProperty());
		if (!value)
		{
			return nullptr;
		}

		return winrt::get_self<EventContext>(value)->get_strong();
	}

	void ListViewExtensions::OnAlternateColorPropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (const auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto ctx = GetEventContext(listViewBase);
			if (ctx)
			{
				ctx->_colorContentChangingRevoker.revoke();
				ctx->_itemsVectorChangedRevoker.revoke();
			}

			if (GetAlternateColor(listViewBase))
			{
				if (!ctx)
				{
					ctx = winrt::make_self<EventContext>();
					listViewBase.SetValue(EventContextProperty(), *ctx);
				}

				ctx->_colorContentChangingRevoker = listViewBase.ContainerContentChanging(winrt::auto_revoke, { &ListViewExtensions::ColorContainerContentChanging });

				auto weakList = winrt::make_weak(listViewBase);
				ctx->_itemsVectorChangedRevoker = listViewBase.Items().VectorChanged(
					winrt::auto_revoke, [weakList](
						winrt::IObservableVector<winrt::IInspectable> const& items, 
						winrt::IVectorChangedEventArgs const& args)
					{
						if (const auto listViewBase = weakList.get())
						{
							ColorItemsVectorChanged(listViewBase, items, args);
						}
					});

				ctx->_unloadedRevoker = listViewBase.Unloaded(winrt::auto_revoke, &ListViewExtensions::OnListViewBaseUnloaded);
			}
		}
	}

	void ListViewExtensions::ColorContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args)
	{
		const auto itemContainer = args.ItemContainer().try_as<winrt::Control>();
		SetItemContainerBackground(sender, itemContainer, args.ItemIndex());
	}

	void ListViewExtensions::OnAlternateItemTemplatePropertyChanged(winrt::DependencyObject const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			auto ctx = GetEventContext(listViewBase);

			if (ctx)
			{
				ctx->_itemTemplateContentChangingRevoker.revoke();
			}

			if (GetAlternateItemTemplate(listViewBase))
			{
				if (!ctx)
				{
					ctx = winrt::make_self<EventContext>();
					listViewBase.SetValue(EventContextProperty(), *ctx);
				}

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
			auto ctx = GetEventContext(listViewBase);
			if (ctx)
			{
				ctx->_stretchDirectionContentChangingRevoker.revoke();
			}

			if (GetItemContainerStretchDirection(listViewBase))
			{
				if (!ctx)
				{
					ctx = winrt::make_self<EventContext>();
					listViewBase.SetValue(EventContextProperty(), *ctx);
				}

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
		if (const auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			listViewBase.ClearValue(EventContextProperty());
		}
	}

	void ListViewExtensions::ColorItemsVectorChanged(winrt::ListViewBase const& listViewBase, winrt::IObservableVector<winrt::IInspectable> const& sender, winrt::IVectorChangedEventArgs const& args)
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
			uint32_t index = args.Index();
			for (uint32_t i = index; i < sender.Size(); i++)
			{
				// Get item container or element at index
				winrt::Control itemContainer{ nullptr };

				if (const auto container = listViewBase.ContainerFromIndex(i).try_as<winrt::Control>())
				{
					itemContainer = container;
				}
				else if (const auto item = listViewBase.Items().GetAt(i).try_as<winrt::Control>())
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

	void ListViewExtensions::SetItemContainerBackground(winrt::ListViewBase const& sender, winrt::Control const& itemContainer, int itemIndex)
	{
		const auto brush = itemIndex % 2 == 0 ? GetAlternateColor(sender) : winrt::Brush{ nullptr };
		itemContainer.Background(brush);
		if (const auto rootBorder = DependencyObjectEx::FindDescendant<winrt::Border>(itemContainer))
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
		const auto listViewBase = sender.try_as<winrt::ListViewBase>();

		if (!listViewBase)
		{
			return;
		}

		if (const auto eventToken = GetCommandEventToken(listViewBase))
		{
			listViewBase.ItemClick(*eventToken);
			listViewBase.ClearValue(CommandEventTokenProperty());
		}

		if (const auto newCommand = args.NewValue().try_as<winrt::ICommand>())
		{
			const auto token = listViewBase.ItemClick(&ListViewExtensions::OnListViewBaseItemClick);
			SetCommandEventToken(listViewBase, token);
		}
	}

	void ListViewExtensions::OnListViewBaseItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e)
	{
		if (const auto listViewBase = sender.try_as<winrt::ListViewBase>())
		{
			const auto command = GetCommand(listViewBase);

			if (command && command.CanExecute(e.ClickedItem()))
			{
				command.Execute(e.ClickedItem());
			}
		}
	}

	winrt::IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithIndexAsync(winrt::ListViewBase listViewBase, int index, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
	{
		const auto items = listViewBase.Items();
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

		const auto scrollViewer = DependencyObjectEx::FindDescendant<winrt::ScrollViewer>(listViewBase);
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
			winrt::ScrollViewer::ViewChanged_revoker viewChangedRevoker
			{
				scrollViewer.ViewChanged(winrt::auto_revoke, [completionEvent](auto&...)
				{
					completionEvent.SetEvent();
				}) 
			};

			listViewBase.ScrollIntoView(items.GetAt(index), winrt::ScrollIntoViewAlignment::Leading);
			co_await winrt::resume_on_signal(completionEvent.get());
			co_await wil::resume_foreground(scrollViewer.DispatcherQueue());

			selectorItem = listViewBase.ContainerFromIndex(index).try_as<winrt::SelectorItem>();
		}

		const auto transform = selectorItem.TransformToVisual(scrollViewer.Content().try_as<winrt::UIElement>());
		const auto position = transform.TransformPoint(winrt::Point(0, 0));

		// Scrolling back to previous position
		if (isVirtualizing)
		{
			co_await ChangeViewAsync(scrollViewer, previousXOffset, previousYOffset, std::nullopt, true);
		}

		double listViewBaseWidth = listViewBase.ActualWidth();
		double selectorItemWidth = selectorItem.ActualWidth();
		double listViewBaseHeight = listViewBase.ActualHeight();
		double selectorItemHeight = selectorItem.ActualHeight();

		previousXOffset = scrollViewer.HorizontalOffset();
		previousYOffset = scrollViewer.VerticalOffset();

		double minXPosition = position.X - listViewBaseWidth + selectorItemWidth;
		double minYPosition = position.Y - listViewBaseHeight + selectorItemHeight;

		double maxXPosition = position.X;
		double maxYPosition = position.Y;

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
				double centreX = (listViewBaseWidth - selectorItemWidth) / 2.0;
				double centreY = (listViewBaseHeight - selectorItemHeight) / 2.0;
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

	winrt::IAsyncAction ListViewExtensions::SmoothScrollIntoViewWithItemAsync(winrt::ListViewBase listViewBase, winrt::IInspectable const& item, ScrollItemPlacement itemPlacement, bool disableAnimation, bool scrollIfVisible, int additionalHorizontalOffset, int additionalVerticalOffset)
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
		auto viewChanged = [completionEvent](winrt::IInspectable const&, winrt::ScrollViewerViewChangedEventArgs const& e)
		{
			if (!e.IsIntermediate())
			{
				completionEvent.SetEvent();
			}
		};

		viewChangedRevoker = scrollViewer.ViewChanged(winrt::auto_revoke, viewChanged);
		if (!scrollViewer.ChangeView(horizontalOffset, verticalOffset, zoomFactor, disableAnimation))
		{
			co_return;
		}

		co_await winrt::resume_on_signal(completionEvent.get());
		co_await wil::resume_foreground(scrollViewer.DispatcherQueue());
	}
}
