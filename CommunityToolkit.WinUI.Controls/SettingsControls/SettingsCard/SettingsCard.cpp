#include "pch.h"
#include "SettingsCard.h"
#if __has_include("SettingsCard.g.cpp")
#include "SettingsCard.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml;

	SettingsCard::SettingsCard()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
	}

	/// <inheritdoc />
	void SettingsCard::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		// if (m_isEnabledChanged) IsEnabledChanged(m_isEnabledChanged);
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
		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);

		if (auto contentAlignmentStatesGroup = GetTemplateChild(ContentAlignmentStates).try_as<VisualStateGroup>())
		{
			// contentAlignmentStatesGroup.CurrentStateChanged(m_contentAlignmentStatesChanged);
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
			if (auto element = Content().try_as<UIElement>(); element && winrt::Microsoft::UI::Xaml::Automation::AutomationProperties::GetName(element).empty() && !element.try_as<ButtonBase>() && !element.try_as<TextBlock>())
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

	void SettingsCard::Control_PreviewKeyUp([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (e.Key() == Windows::System::VirtualKey::Enter ||
			e.Key() == Windows::System::VirtualKey::Space ||
			e.Key() == Windows::System::VirtualKey::GamepadA)
		{
			VisualStateManager::GoToState(*this, NormalState, true);
		}
	}

	void SettingsCard::Control_PreviewKeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		if (e.Key() == Windows::System::VirtualKey::Enter ||
			e.Key() == Windows::System::VirtualKey::Space ||
			e.Key() == Windows::System::VirtualKey::GamepadA)
		{
			// Check if the active focus is on the card itself - only then we show the pressed state.
			if (GetFocusedElement().try_as<class_type>())
			{
				VisualStateManager::GoToState(*this, PressedState, true);
			}
		}
	}

	void SettingsCard::Control_PointerEntered([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerEntered(e);
		VisualStateManager::GoToState(*this, PointerOverState, true);
	}

	void SettingsCard::Control_PointerExited([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerExited(e);
		VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::Control_PointerCaptureLost([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerCaptureLost(e);
		VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::Control_PointerCanceled([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
	{
		base_type::OnPointerCanceled(e);
		VisualStateManager::GoToState(*this, NormalState, true);
	}

	void SettingsCard::OnPointerPressed(PointerRoutedEventArgs const& e)
	{
		if (IsClickEnabled())
		{
			base_type::OnPointerPressed(e);
			VisualStateManager::GoToState(*this, PressedState, true);
		}
	}

	void SettingsCard::OnPointerReleased(PointerRoutedEventArgs const& e)
	{
		if (IsClickEnabled())
		{
			base_type::OnPointerReleased(e);
			VisualStateManager::GoToState(*this, NormalState, true);
		}
	}

	winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer SettingsCard::OnCreateAutomationPeer()
	{
		return winrt::make<winrt::CommunityToolkit::WinUI::Controls::implementation::SettingsCardAutomationPeer>(*this);
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

	void SettingsCard::OnIsEnabledChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		VisualStateManager::GoToState(*this, IsEnabled() ? NormalState : DisabledState, true);

		CheckHeaderIconState();
	}

	void SettingsCard::CheckHeaderIconState()
	{
		// The Disabled visual state will only set the right Foreground brush, but for images we need to lower the opacity so it looks disabled.
		if (HeaderIcon().try_as<BitmapIcon>())
		{
			VisualStateManager::GoToState(*this, IsEnabled() ? BitmapHeaderIconEnabledState : BitmapHeaderIconDisabledState, true);
		}
	}

	void SettingsCard::OnActionIconChanged()
	{
		if (auto actionIconPresenter = GetTemplateChild(ActionIconPresenterHolder).try_as<FrameworkElement>())
		{
			if (IsClickEnabled() && IsActionIconVisible())
			{
				actionIconPresenter.Visibility(Visibility::Visible);
			}
			else
			{
				actionIconPresenter.Visibility(Visibility::Collapsed);
			}
		}
	}

	void SettingsCard::OnHeaderIconChanged()
	{
		if (auto headerIconPresenter = GetTemplateChild(HeaderIconPresenterHolder).try_as<FrameworkElement>())
		{
			headerIconPresenter.Visibility(HeaderIcon()
				? Visibility::Visible
				: Visibility::Collapsed);
		}
	}

	void SettingsCard::OnDescriptionChanged()
	{
		if (auto descriptionPresenter = GetTemplateChild(DescriptionPresenter).try_as<FrameworkElement>())
		{
			descriptionPresenter.Visibility(IsNullOrEmptyString(Description())
				? Visibility::Collapsed
				: Visibility::Visible);
		}
	}

	void SettingsCard::OnHeaderChanged()
	{
		if (auto headerPresenter = GetTemplateChild(HeaderPresenter).try_as<FrameworkElement>())
		{
			headerPresenter.Visibility(IsNullOrEmptyString(Header())
				? Visibility::Collapsed
				: Visibility::Visible);
		}
	}

	void SettingsCard::ContentAlignmentStates_Changed([[maybe_unused]] IInspectable const& sender, VisualStateChangedEventArgs const& e)
	{
		CheckVerticalSpacingState(e.NewState());
	}

	void SettingsCard::CheckVerticalSpacingState(VisualState const& s)
	{
		// On state change, checking if the Content should be wrapped (e.g. when the card is made smaller or the ContentAlignment is set to Vertical). If the Content and the Header or Description are not null, we add spacing between the Content and the Header/Description.
		if (s != nullptr && (s.Name() == RightWrappedState || s.Name() == RightWrappedNoIconState || s.Name() == VerticalState) && (Content() != nullptr) && (!IsNullOrEmptyString(Header()) || !IsNullOrEmptyString(Description())))
		{
			VisualStateManager::GoToState(*this, ContentSpacingState, true);
		}
		else
		{
			VisualStateManager::GoToState(*this, NoContentSpacingState, true);
		}
	}

	FrameworkElement SettingsCard::GetFocusedElement()
	{
		if (ControlHelpers::IsXamlRootAvailable() && XamlRoot() != nullptr)
		{
			return FocusManager::GetFocusedElement(XamlRoot()).try_as<FrameworkElement>();
		}
		else
		{
			return FocusManager::GetFocusedElement().try_as<FrameworkElement>();
		}
	}

	bool SettingsCard::IsNullOrEmptyString(IInspectable const& obj)
	{
		if (obj == nullptr)
		{
			return true;
		}

		if (auto objString = obj.try_as<winrt::hstring>(); objString && objString->empty())
		{
			return true;
		}

		return false;
	}

	void SettingsCard::OnIsClickEnabledPropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnIsClickEnabledChanged();
	}

	void SettingsCard::OnHeaderIconPropertyChanged([[maybe_unused]] IconElement const& oldValue, [[maybe_unused]] IconElement const& newValue)
	{
		OnHeaderIconChanged();
	}

	void SettingsCard::OnHeaderPropertyChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue)
	{
		OnHeaderChanged();
	}

	void SettingsCard::OnDescriptionPropertyChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue)
	{
		OnDescriptionChanged();
	}

	void SettingsCard::OnIsActionIconVisiblePropertyChanged([[maybe_unused]] bool oldValue, [[maybe_unused]] bool newValue)
	{
		OnActionIconChanged();
	}
}
