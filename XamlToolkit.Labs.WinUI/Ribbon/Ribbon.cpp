#include "pch.h"
#include "winrt_module_imports.h"
#include "Ribbon.h"
#if __has_include("Ribbon.g.cpp")
#include "Ribbon.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	Ribbon::Ribbon()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		_items = winrt::single_threaded_observable_vector<UIElement>();
		_items.VectorChanged({ this, &Ribbon::OnItemsVectorChanged });
	}

	void Ribbon::OnApplyTemplate()
	{
		_panelSizeChangedRevoker.revoke();
		_decrementButtonClickRevoker.revoke();
		_incrementButtonClickRevoker.revoke();
		_scrollViewerViewChangedRevoker.revoke();
		_scrollViewerSizeChangedRevoker.revoke();

		_panel = GetTemplateChild(PanelTemplatePart).try_as<Panel>();
		if (_panel)
		{
			for (const auto& item : _items)
			{
				_panel.Children().Append(item);
			}

			_panelSizeChangedRevoker = _panel.SizeChanged(winrt::auto_revoke, { this, &Ribbon::OnSizeChanged });
		}

		_decrementButton = GetTemplateChild(ScrollDecrementButtonTempatePart).try_as<ButtonBase>();
		if (_decrementButton)
		{
			_decrementButtonClickRevoker = _decrementButton.Click(winrt::auto_revoke, { this, &Ribbon::OnDecrementScrollViewer });
		}

		_incrementButton = GetTemplateChild(ScrollIncrementButtonTempatePart).try_as<ButtonBase>();
		if (_incrementButton)
		{
			_incrementButtonClickRevoker = _incrementButton.Click(winrt::auto_revoke, { this, &Ribbon::OnIncrementScrollViewer });
		}

		_scrollViewer = GetTemplateChild(ScrollViewerTemplatePart).try_as<ScrollViewer>();
		if (_scrollViewer)
		{
			_scrollViewerViewChangedRevoker = _scrollViewer.ViewChanged(winrt::auto_revoke, { this, &Ribbon::OnViewChanged });
			_scrollViewerSizeChangedRevoker = _scrollViewer.SizeChanged(winrt::auto_revoke, { this, &Ribbon::OnSizeChanged });
			UpdateScrollButtonsState();
		}
	}

	void Ribbon::OnItemsVectorChanged(
		IObservableVector<UIElement> const& sender,
		IVectorChangedEventArgs const& args)
	{
		if (_panel == nullptr)
			return;

		auto children = _panel.Children();
		auto change = args.CollectionChange();
		uint32_t index = args.Index();

		switch (change)
		{
		case CollectionChange::ItemInserted:
		{
			auto element = sender.GetAt(index);
			if (!element)
				throw hresult_invalid_argument(L"Item must not be null");
			children.InsertAt(index, element);
		}
		break;

		case CollectionChange::ItemRemoved:
		{
			children.RemoveAt(index);
		}
		break;

		case CollectionChange::ItemChanged:
		{
			auto element = sender.GetAt(index);
			if (!element)
				throw hresult_invalid_argument(L"Item must not be null");
			children.SetAt(index, element);
		}
		break;

		case CollectionChange::Reset:
		{
			children.Clear();
			for (auto const& item : sender)
			{
				children.Append(item);
			}
		}
		break;

		default:
			throw hresult_invalid_argument(L"Invalid CollectionChange value");
		}
	}

	void Ribbon::OnViewChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] ScrollViewerViewChangedEventArgs const& e)
	{ 
		UpdateScrollButtonsState();
	}

	void Ribbon::OnSizeChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		UpdateScrollButtonsState();
	}

	void Ribbon::UpdateScrollButtonsState()
	{
		if (_scrollViewer == nullptr)
		{
			return;
		}

		if (_scrollViewer.ExtentWidth() <= _scrollViewer.ViewportWidth())
		{
			VisualStateManager::GoToState(*this, NoButtonsStateTemplatePart, true);
			return;
		}

		auto showDecrement = _scrollViewer.HorizontalOffset() >= 1;
		auto showIncrement = _scrollViewer.ExtentWidth() - _scrollViewer.HorizontalOffset() - _scrollViewer.ViewportWidth() >= 1;
		if (showDecrement && showIncrement)
		{
			VisualStateManager::GoToState(*this, BothButtonsStateTemplatePart, true);
		}
		else if (showDecrement)
		{
			VisualStateManager::GoToState(*this, DecrementButtonStateTemplatePart, true);
		}
		else if (showIncrement)
		{
			VisualStateManager::GoToState(*this, IncrementButtonStateTemplatePart, true);
		}
		else
		{
			VisualStateManager::GoToState(*this, NoButtonsStateTemplatePart, true);
		}
	}

	void Ribbon::OnDecrementScrollViewer([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_scrollViewer) _scrollViewer.ChangeView(_scrollViewer.HorizontalOffset() - ScrollStep(), nullptr, nullptr);
	}

	void Ribbon::OnIncrementScrollViewer([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
	{
		if (_scrollViewer) _scrollViewer.ChangeView(_scrollViewer.HorizontalOffset() + ScrollStep(), nullptr, nullptr);
	}
}
