#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsCard.h"
#if __has_include("SettingsCard.g.cpp")
#include "SettingsCard.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::HeaderProperty =
		winrt::DependencyProperty::Register(
			L"Header",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(nullptr, [](auto&& d, auto&& e)
			{
				auto self = winrt::get_self<SettingsCard>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>())->get_strong();
				self->OnHeaderPropertyChanged(e.OldValue().as<winrt::IInspectable>(), e.NewValue().as<winrt::IInspectable>());
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::DescriptionProperty =
		winrt::DependencyProperty::Register(
			L"Description",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(nullptr, [](auto&& d, auto&& e)
			{
				auto self = winrt::get_self<SettingsCard>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>())->get_strong();
				self->OnDescriptionPropertyChanged(e.OldValue().as<winrt::IInspectable>(), e.NewValue().as<winrt::IInspectable>());
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::HeaderIconProperty =
		winrt::DependencyProperty::Register(
			L"HeaderIcon",
			winrt::xaml_typename<winrt::IconElement>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(nullptr, [](auto&& d, auto&& e)
			{
				auto self = winrt::get_self<SettingsCard>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>())->get_strong();
				self->OnHeaderIconPropertyChanged(e.OldValue().as<winrt::IconElement>(), e.NewValue().as<winrt::IconElement>());
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::ActionIconProperty =
		winrt::DependencyProperty::Register(
			L"ActionIcon",
			winrt::xaml_typename<winrt::IconElement>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::ActionIconToolTipProperty =
		winrt::DependencyProperty::Register(
			L"ActionIconToolTip",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::IsClickEnabledProperty =
		winrt::DependencyProperty::Register(
			L"IsClickEnabled",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(winrt::box_value(false), [](auto&& d, auto&& e)
			{
				auto self = winrt::get_self<SettingsCard>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>())->get_strong();
				self->OnIsClickEnabledPropertyChanged(e.OldValue().as<bool>(), e.NewValue().as<bool>());
			}));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::ContentAlignmentProperty =
		winrt::DependencyProperty::Register(
			L"ContentAlignment",
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::ContentAlignment>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(winrt::box_value(winrt::XamlToolkit::WinUI::Controls::ContentAlignment::Right)));

	const wil::single_threaded_property<winrt::DependencyProperty> SettingsCard::IsActionIconVisibleProperty =
		winrt::DependencyProperty::Register(
			L"IsActionIconVisible",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::SettingsCard>(),
			winrt::PropertyMetadata(winrt::box_value(true), [](auto&& d, auto&& e)
			{
				auto self = winrt::get_self<SettingsCard>(d.template as<winrt::XamlToolkit::WinUI::Controls::SettingsCard>())->get_strong();
				self->OnIsActionIconVisiblePropertyChanged(e.OldValue().as<bool>(), e.NewValue().as<bool>());
			}));

	SettingsCard::SettingsCard()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// This is perhaps indicative of an issue with the action icon requiring a concrete FontElement instead of a template
		winrt::FontIcon fontIcon;
		fontIcon.Glyph(L"\ue974");
		fontIcon.MirroredWhenRightToLeft(true);
		ActionIcon(fontIcon);
	}

	void SettingsCard::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();

		OnActionIconChanged();
		OnHeaderChanged();
		OnHeaderIconChanged();
		OnDescriptionChanged();
		OnIsClickEnabledChanged();
		CheckInitialVisualState();
		SetAccessibleContentName();

		_enabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &SettingsCard::OnIsEnabledChanged });
	}

	void SettingsCard::CheckInitialVisualState()
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);

		if (auto contentAlignmentStatesGroup = GetTemplateChild(ContentAlignmentStates).try_as<winrt::VisualStateGroup>())
		{
			CheckVerticalSpacingState(contentAlignmentStatesGroup.CurrentState());
			_currentStateChangedRevoker = contentAlignmentStatesGroup.CurrentStateChanged(winrt::auto_revoke, { this, &SettingsCard::ContentAlignmentStates_Changed });
		}

		CheckHeaderIconState();
	}

	// We automatically set the AutomationProperties.Name of the Content if not configured.
	void SettingsCard::SetAccessibleContentName()
	{
		if (auto headerString = Header().try_as<winrt::hstring>(); headerString && !headerString->empty())
		{
			// We don't want to override an AutomationProperties.Name that is manually set, or if the Content basetype is of type ButtonBase (the ButtonBase.Content will be used then)
			if (auto element = Content().try_as<winrt::UIElement>(); element && winrt::Microsoft::UI::Xaml::Automation::AutomationProperties::GetName(element).empty() && !element.try_as<winrt::ButtonBase>() && !element.try_as<winrt::TextBlock>())
			{
				winrt::Microsoft::UI::Xaml::Automation::AutomationProperties::SetName(element, headerString.value());
			}
		}
	}

	void SettingsCard::EnableButtonInteraction()
	{
		DisableButtonInteraction();

		IsTabStop(true);
		_pointerEnteredToken = PointerEntered({ this, &SettingsCard::Control_PointerEntered });
		_pointerExitedToken = PointerExited({ this, &SettingsCard::Control_PointerExited });
		_pointerCaptureLostToken = PointerCaptureLost({ this, &SettingsCard::Control_PointerCaptureLost });
		_pointerCanceledToken = PointerCanceled({ this, &SettingsCard::Control_PointerCanceled });
		_previewKeyDownToken = PreviewKeyDown({ this, &SettingsCard::Control_PreviewKeyDown });
		_previewKeyUpToken = PreviewKeyUp({ this, &SettingsCard::Control_PreviewKeyUp });
	}

	void SettingsCard::DisableButtonInteraction()
	{
		IsTabStop(false);
		PointerEntered(_pointerEnteredToken);
		PointerExited(_pointerExitedToken);
		PointerCaptureLost(_pointerCaptureLostToken);
		PointerCanceled(_pointerCanceledToken);
		PreviewKeyDown(_previewKeyDownToken);
		PreviewKeyUp(_previewKeyUpToken);
	}

	void SettingsCard::Control_PreviewKeyUp([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		if (e.Key() == winrt::VirtualKey::Enter ||
			e.Key() == winrt::VirtualKey::Space ||
			e.Key() == winrt::VirtualKey::GamepadA)
		{
			winrt::VisualStateManager::GoToState(*this, NormalState, true);
		}
	}

	void SettingsCard::Control_PreviewKeyDown([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		if (e.Key() == winrt::VirtualKey::Enter ||
			e.Key() == winrt::VirtualKey::Space ||
			e.Key() == winrt::VirtualKey::GamepadA)
		{
			// Check if the active focus is on the card itself - only then we show the pressed state.
			if (GetFocusedElement().try_as<class_type>())
			{
				winrt::VisualStateManager::GoToState(*this, PressedState, true);
			}
		}
	}

	void SettingsCard::Control_PointerEntered([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerEntered(e);
		winrt::VisualStateManager::GoToState(*this, PointerOverState, true);
	}

	void SettingsCard::Control_PointerExited([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerExited(e);
		winrt::VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::Control_PointerCaptureLost([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerCaptureLost(e);
		winrt::VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::Control_PointerCanceled([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerCanceled(e);
		winrt::VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::OnPointerPressed(winrt::PointerRoutedEventArgs const& e)
	{
		if (IsClickEnabled())
		{
			base_type::OnPointerPressed(e);
			winrt::VisualStateManager::GoToState(*this, PressedState, true);
		}
	}

	void SettingsCard::OnPointerReleased(winrt::PointerRoutedEventArgs const& e)
	{
		if (IsClickEnabled())
		{
			base_type::OnPointerReleased(e);
			winrt::VisualStateManager::GoToState(*this, NormalState, true);
		}
	}

	winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer SettingsCard::OnCreateAutomationPeer()
	{
		return winrt::make<winrt::XamlToolkit::WinUI::Controls::implementation::SettingsCardAutomationPeer>(*this);
	}

	void SettingsCard::OnIsClickEnabledChanged()
	{
		OnActionIconChanged();
		if (IsClickEnabled())
		{
			EnableButtonInteraction();
		}
		else
		{
			DisableButtonInteraction();
		}
	}

	void SettingsCard::OnIsEnabledChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);

		CheckHeaderIconState();
	}

	void SettingsCard::CheckHeaderIconState()
	{
		// The Disabled visual state will only set the right Foreground brush, but for images we need to lower the opacity so it looks disabled.
		if (HeaderIcon().try_as<winrt::BitmapIcon>())
		{
			winrt::VisualStateManager::GoToState(*this, IsEnabled() ? BitmapHeaderIconEnabledState : BitmapHeaderIconDisabledState, true);
		}
	}

	void SettingsCard::OnActionIconChanged()
	{
		if (auto actionIconPresenter = GetTemplateChild(ActionIconPresenterHolder).try_as<winrt::FrameworkElement>())
		{
			if (IsClickEnabled() && IsActionIconVisible())
			{
				actionIconPresenter.Visibility(winrt::Visibility::Visible);
			}
			else
			{
				actionIconPresenter.Visibility(winrt::Visibility::Collapsed);
			}
		}
	}

	void SettingsCard::OnHeaderIconChanged()
	{
		if (auto headerIconPresenter = GetTemplateChild(HeaderIconPresenterHolder).try_as<winrt::FrameworkElement>())
		{
			headerIconPresenter.Visibility(HeaderIcon()
				? winrt::Visibility::Visible
				: winrt::Visibility::Collapsed);
		}
	}

	void SettingsCard::OnDescriptionChanged()
	{
		if (auto descriptionPresenter = GetTemplateChild(DescriptionPresenter).try_as<winrt::FrameworkElement>())
		{
			descriptionPresenter.Visibility(IsNullOrEmptyString(Description())
				? winrt::Visibility::Collapsed
				: winrt::Visibility::Visible);
		}
	}

	void SettingsCard::OnHeaderChanged()
	{
		if (auto headerPresenter = GetTemplateChild(HeaderPresenter).try_as<winrt::FrameworkElement>())
		{
			headerPresenter.Visibility(IsNullOrEmptyString(Header())
				? winrt::Visibility::Collapsed
				: winrt::Visibility::Visible);
		}
	}

	void SettingsCard::ContentAlignmentStates_Changed([[maybe_unused]] winrt::IInspectable const& sender, winrt::VisualStateChangedEventArgs const& e)
	{
		CheckVerticalSpacingState(e.NewState());
	}

	void SettingsCard::CheckVerticalSpacingState(winrt::VisualState const& s)
	{
		// On state change, checking if the Content should be wrapped (e.g. when the card is made smaller or the ContentAlignment is set to Vertical). If the Content and the Header or Description are not null, we add spacing between the Content and the Header/Description.
		if (s != nullptr && (s.Name() == RightWrappedState || s.Name() == RightWrappedNoIconState || s.Name() == VerticalState) && (Content() != nullptr) && (!IsNullOrEmptyString(Header()) || !IsNullOrEmptyString(Description())))
		{
			winrt::VisualStateManager::GoToState(*this, ContentSpacingState, true);
		}
		else
		{
			winrt::VisualStateManager::GoToState(*this, NoContentSpacingState, true);
		}
	}

	winrt::FrameworkElement SettingsCard::GetFocusedElement()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot() != nullptr)
		{
			return winrt::FocusManager::GetFocusedElement(XamlRoot()).try_as<winrt::FrameworkElement>();
		}
		else
		{
			return winrt::FocusManager::GetFocusedElement().try_as<winrt::FrameworkElement>();
		}
	}

	bool SettingsCard::IsNullOrEmptyString(winrt::IInspectable const& obj)
	{
		if (obj == nullptr)
		{
			return true;
		}

		if (auto objString = obj.try_as<winrt::hstring>())
		{
			return objString && objString->empty();
		}

		return false;
	}

	void SettingsCard::OnIsClickEnabledPropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnIsClickEnabledChanged();
	}

	void SettingsCard::OnHeaderIconPropertyChanged([[maybe_unused]] winrt::IconElement const& oldValue, [[maybe_unused]] winrt::IconElement const& newValue)
	{
		OnHeaderIconChanged();
	}

	void SettingsCard::OnHeaderPropertyChanged([[maybe_unused]] winrt::IInspectable const& oldValue, [[maybe_unused]] winrt::IInspectable const& newValue)
	{
		OnHeaderChanged();
	}

	void SettingsCard::OnDescriptionPropertyChanged([[maybe_unused]] winrt::IInspectable const& oldValue, [[maybe_unused]] winrt::IInspectable const& newValue)
	{
		OnDescriptionChanged();
	}

	void SettingsCard::OnIsActionIconVisiblePropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnActionIconChanged();
	}
}
