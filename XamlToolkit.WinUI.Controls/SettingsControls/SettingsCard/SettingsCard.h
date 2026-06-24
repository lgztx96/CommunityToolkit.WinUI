#pragma once

#include "SettingsCard.g.h"
#include "../Helpers/ControlHelper.h"
#include "SettingsCardAutomationPeer.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::System;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SettingsCard : SettingsCardT<SettingsCard>
	{
		static constexpr auto CommonStates = L"CommonStates";
		static constexpr auto NormalState = L"Normal";
		static constexpr auto PointerOverState = L"PointerOver";
		static constexpr auto PressedState = L"Pressed";
		static constexpr auto DisabledState = L"Disabled";

		static constexpr auto BitmapHeaderIconStates = L"BitmapHeaderIconStates";
		static constexpr auto BitmapHeaderIconEnabledState = L"BitmapHeaderIconEnabled";
		static constexpr auto BitmapHeaderIconDisabledState = L"BitmapHeaderIconDisabled";

		static constexpr auto ContentAlignmentStates = L"ContentAlignmentStates";
		static constexpr auto RightState = L"Right";
		static constexpr auto RightWrappedState = L"RightWrapped";
		static constexpr auto RightWrappedNoIconState = L"RightWrappedNoIcon";
		static constexpr auto LeftState = L"Left";
		static constexpr auto VerticalState = L"Vertical";

		static constexpr auto ContentSpacingStates = L"ContentSpacingStates";
		static constexpr auto NoContentSpacingState = L"NoContentSpacing";
		static constexpr auto ContentSpacingState = L"ContentSpacing";

		static constexpr auto ActionIconPresenterHolder = L"PART_ActionIconPresenterHolder";
		static constexpr auto HeaderPresenter = L"PART_HeaderPresenter";
		static constexpr auto DescriptionPresenter = L"PART_DescriptionPresenter";
		static constexpr auto HeaderIconPresenterHolder = L"PART_HeaderIconPresenterHolder";

		SettingsCard();

		void OnApplyTemplate();

		void CheckInitialVisualState();

		// We automatically set the AutomationProperties.Name of the Content if not configured.
		void SetAccessibleContentName();

		void EnableButtonInteraction();

		void DisableButtonInteraction();

		void Control_PreviewKeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void Control_PreviewKeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void Control_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		void Control_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		void Control_PointerCaptureLost(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		void Control_PointerCanceled(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

		void OnPointerPressed(winrt::PointerRoutedEventArgs const& e);

		void OnPointerReleased(winrt::PointerRoutedEventArgs const& e);

		winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();

		void OnIsClickEnabledChanged();

		void OnIsEnabledChanged(winrt::IInspectable const& sender, winrt::DependencyPropertyChangedEventArgs const& e);

		void CheckHeaderIconState();

		void OnActionIconChanged();

		void OnHeaderIconChanged();

		void OnDescriptionChanged();

		void OnHeaderChanged();

		void ContentAlignmentStates_Changed(winrt::IInspectable const& sender, winrt::VisualStateChangedEventArgs const& e);

		void CheckVerticalSpacingState(winrt::VisualState const& s);

		winrt::FrameworkElement GetFocusedElement();

		static bool IsNullOrEmptyString(winrt::IInspectable const& obj);

		void OnIsClickEnabledPropertyChanged(bool oldValue, bool newValue);

		void OnHeaderIconPropertyChanged(winrt::IconElement const& oldValue, winrt::IconElement const& newValue);

		void OnHeaderPropertyChanged(winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

		void OnDescriptionPropertyChanged(winrt::IInspectable const& oldValue, winrt::IInspectable const& newValue);

		void OnIsActionIconVisiblePropertyChanged(bool oldValue, bool newValue);

		static const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> DescriptionProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> HeaderIconProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ActionIconProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ActionIconToolTipProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> IsClickEnabledProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> ContentAlignmentProperty;

		static const wil::single_threaded_property<winrt::DependencyProperty> IsActionIconVisibleProperty;

		winrt::IInspectable Header() const { return GetValue(HeaderProperty); }

		void Header(winrt::IInspectable const& value) const { SetValue(HeaderProperty, value); }

		winrt::IInspectable Description() const { return GetValue(DescriptionProperty); }

		void Description(winrt::IInspectable const& value) const { SetValue(DescriptionProperty, value); }

		winrt::IconElement HeaderIcon() const { return winrt::unbox_value<winrt::IconElement>(GetValue(HeaderIconProperty)); }

		void HeaderIcon(winrt::IconElement const& value) const { SetValue(HeaderIconProperty, value); }

		winrt::IconElement ActionIcon() const { return winrt::unbox_value<winrt::IconElement>(GetValue(ActionIconProperty)); }

		void ActionIcon(winrt::IconElement const& value) const { SetValue(ActionIconProperty, value); }

		winrt::hstring ActionIconToolTip() const { return winrt::unbox_value<winrt::hstring>(GetValue(ActionIconToolTipProperty)); }

		void ActionIconToolTip(winrt::hstring const& value) const { SetValue(ActionIconToolTipProperty, winrt::box_value(value)); }

		bool IsClickEnabled() const { return winrt::unbox_value<bool>(GetValue(IsClickEnabledProperty)); }

		void IsClickEnabled(bool value) const { SetValue(IsClickEnabledProperty, winrt::box_value(value)); }

		winrt::XamlToolkit::WinUI::Controls::ContentAlignment ContentAlignment() const
		{
			return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::ContentAlignment>(
				GetValue(ContentAlignmentProperty));
		}

		void ContentAlignment(winrt::XamlToolkit::WinUI::Controls::ContentAlignment value) const
		{
			SetValue(ContentAlignmentProperty, winrt::box_value(value));
		}

		bool IsActionIconVisible() const { return winrt::unbox_value<bool>(GetValue(IsActionIconVisibleProperty)); }

		void IsActionIconVisible(bool value) const { SetValue(IsActionIconVisibleProperty, winrt::box_value(value)); }

	private:
		winrt::event_token _pointerEnteredToken{};
		winrt::event_token _pointerExitedToken{};
		winrt::event_token _pointerCaptureLostToken{};
		winrt::event_token _pointerCanceledToken{};
		winrt::event_token _previewKeyDownToken{};
		winrt::event_token _previewKeyUpToken{};

		class_type::IsEnabledChanged_revoker _enabledChangedRevoker;
		winrt::VisualStateGroup::CurrentStateChanged_revoker _currentStateChangedRevoker;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SettingsCard : SettingsCardT<SettingsCard, implementation::SettingsCard>
	{
	};
}
