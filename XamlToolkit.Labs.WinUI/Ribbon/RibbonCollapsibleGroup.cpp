#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RibbonCollapsibleGroup.h"
#if __has_include("RibbonCollapsibleGroup.g.cpp")
#include "RibbonCollapsibleGroup.g.cpp"
#endif
#include "DoubleVector.h"

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Windows::System;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	RibbonCollapsibleGroup::RibbonCollapsibleGroup()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		_contaionerPointerEventHandler = winrt::box_value(PointerEventHandler({ this, &RibbonCollapsibleGroup::OnFlyoutPointerReleased }));
		_contaionerKeyEventHandler = winrt::box_value(KeyEventHandler({ this, &RibbonCollapsibleGroup::OnFlyoutKeyUp }));
	}

	void RibbonCollapsibleGroup::OnApplyTemplate()
	{
		if (_collapsedFlyout)
		{
			_flyoutOpenedRevoker.revoke();
		}

		if (_collapsedContentContainer)
		{
			_collapsedContentContainer.RemoveHandler(UIElement::PointerReleasedEvent(), _contaionerPointerEventHandler);
			_collapsedContentContainer.RemoveHandler(UIElement::KeyUpEvent(), _contaionerKeyEventHandler);
		}

		_visibleContentContainer = Get<ContentControl>(VisibleContentContainerTemplatePart);
		_collapsedContentContainer = Get<ContentControl>(CollapsedContentPresenterTemplatePart);
		_collapsedButton = Get<Button>(CollapsedButtonTemplatePart);
		_collapsedFlyout = Get<Flyout>(CollapsedFlyoutTemplatePart);

		if (_collapsedFlyout)
		{
			_flyoutOpenedRevoker = _collapsedFlyout.Opened(winrt::auto_revoke, { this, &RibbonCollapsibleGroup::OnFlyoutOpened });
		}

		if (_collapsedContentContainer)
		{
			_collapsedContentContainer.AddHandler(UIElement::PointerReleasedEvent(), _contaionerPointerEventHandler, true);
			_collapsedContentContainer.AddHandler(UIElement::KeyUpEvent(), _contaionerKeyEventHandler, true);
		}

		UpdateState();
	}

	void RibbonCollapsibleGroup::OnRequestedWidthsChanged([[maybe_unused]] DependencyObject const& sender, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto newValue = e.NewValue().try_as<winrt::XamlToolkit::Labs::WinUI::DoubleVector>())
		{
			auto vector = winrt::get_self<DoubleVector>(newValue)->get_strong();
			std::ranges::sort(vector->get_container());
		}
	}

	void RibbonCollapsibleGroup::OnFlyoutOpened([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] IInspectable const& e)
	{
		if (_collapsedContentContainer) _collapsedContentContainer.Focus(FocusState::Programmatic);
	}

	void RibbonCollapsibleGroup::OnFlyoutPointerReleased([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		AutoCollapseFlyout(e.Handled(), e.OriginalSource());
	}

	void RibbonCollapsibleGroup::OnFlyoutKeyUp([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (e.Key() != VirtualKey::Enter && e.Key() != VirtualKey::Space)
		{
			return;
		}

		AutoCollapseFlyout(e.Handled(), e.OriginalSource());
	}

	void RibbonCollapsibleGroup::AutoCollapseFlyout(bool eventHasBeenHandled, IInspectable const& originalSource)
	{
		// We only consider events which have been processed since it usually means
		// that a control has processed the event and that the click is not in an
		// empty/non-interactive area.
		if (eventHasBeenHandled &&
			AutoCloseFlyout() &&
			_collapsedFlyout &&
			_collapsedFlyout.IsOpen() &&
			!DoesRoutedEventOriginateFromAFlyoutHost(originalSource.try_as<UIElement>()))
		{
			_collapsedFlyout.Hide();
		}
	}

	bool RibbonCollapsibleGroup::DoesRoutedEventOriginateFromAFlyoutHost(UIElement source)
	{
		if (_collapsedContentContainer == nullptr)
		{
			return false;
		}

		while (source && source != _collapsedContentContainer)
		{
			if (source.try_as<DropDownButton>() ||
				source.try_as<ComboBox>() ||
				source.try_as<ComboBoxItem>())
			{
				return true;
			}

			if (auto buttonSource = source.try_as<Button>(); buttonSource && buttonSource.Flyout())
			{
				return true;
			}

			if (auto frameworkSource = source.try_as<FrameworkElement>())
			{
				if (FlyoutBase::GetAttachedFlyout(frameworkSource))
				{
					return true;
				}
			}

			source = VisualTreeHelper::GetParent(source).try_as<UIElement>();
		}

		return false;
	}

	void RibbonCollapsibleGroup::OnStatePropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		auto group = winrt::get_self<implementation::RibbonCollapsibleGroup>(d.as<class_type>());
		group->UpdateState();
	}

	void RibbonCollapsibleGroup::UpdateState()
	{
		switch (State())
		{
		case Visibility::Visible:
			if (_collapsedFlyout)
			{
				_collapsedFlyout.Hide();
			}

			if (_collapsedContentContainer && _visibleContentContainer)
			{
				_collapsedContentContainer.Content(nullptr);
				_visibleContentContainer.Content(Content());
			}

			if (_collapsedButton && _visibleContentContainer)
			{
				_collapsedButton.Visibility(Visibility::Collapsed);
				_visibleContentContainer.Visibility(Visibility::Visible);
			}
			break;

		case Visibility::Collapsed:
			if (_collapsedContentContainer && _visibleContentContainer)
			{
				_visibleContentContainer.Content(nullptr);
				_collapsedContentContainer.Content(Content());
			}

			if (_collapsedButton && _visibleContentContainer)
			{
				_visibleContentContainer.Visibility(Visibility::Collapsed);
				_collapsedButton.Visibility(Visibility::Visible);
			}
			break;

		default:
			throw winrt::hresult_invalid_argument(L"Invalid state");
		}
	}
}
