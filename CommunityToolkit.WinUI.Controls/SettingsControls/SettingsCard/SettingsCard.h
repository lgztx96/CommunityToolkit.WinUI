#pragma once

#include "SettingsCard.g.h"
#include "../Helpers/ControlHelper.h"
#include "SettingsCardAutomationPeer.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Microsoft::UI::Xaml::Input;

	template <typename D, typename... I>
	using SettingsCardT = SettingsCard_base<D, I...>;

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

		winrt::event_token _pointerEnteredToken{};
		winrt::event_token _pointerExitedToken{};
		winrt::event_token _pointerCaptureLostToken{};
		winrt::event_token _pointerCanceledToken{};
		winrt::event_token _previewKeyDownToken{};
		winrt::event_token _previewKeyUpToken{};

		class_type::IsEnabledChanged_revoker _enabledChangedRevoker;
		VisualStateGroup::CurrentStateChanged_revoker _currentStateChangedRevoker;

		SettingsCard();

		/// <inheritdoc />
		void OnApplyTemplate();

		void CheckInitialVisualState();

		// We automatically set the AutomationProperties.Name of the Content if not configured.
		void SetAccessibleContentName();

		void EnableButtonInteraction();

		void DisableButtonInteraction();

		void Control_PreviewKeyUp(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void Control_PreviewKeyDown(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void Control_PointerEntered(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void Control_PointerExited(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void Control_PointerCaptureLost(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void Control_PointerCanceled(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void OnPointerPressed(PointerRoutedEventArgs const& e);

		void OnPointerReleased(PointerRoutedEventArgs const& e);

		winrt::Microsoft::UI::Xaml::Automation::Peers::AutomationPeer OnCreateAutomationPeer();

		void OnIsClickEnabledChanged();

		void OnIsEnabledChanged(IInspectable const& sender, DependencyPropertyChangedEventArgs const& e);

		void CheckHeaderIconState();

		void OnActionIconChanged();

		void OnHeaderIconChanged();

		void OnDescriptionChanged();

		void OnHeaderChanged();

		void ContentAlignmentStates_Changed(IInspectable const& sender, VisualStateChangedEventArgs const& e);

		void CheckVerticalSpacingState(VisualState const& s);

		FrameworkElement GetFocusedElement();

		static bool IsNullOrEmptyString(IInspectable const& obj);

		void OnIsClickEnabledPropertyChanged(bool oldValue, bool newValue);

		void OnHeaderIconPropertyChanged(IconElement const& oldValue, IconElement const& newValue);

		void OnHeaderPropertyChanged(IInspectable const& oldValue, IInspectable const& newValue);

		void OnDescriptionPropertyChanged(IInspectable const& oldValue, IInspectable const& newValue);

		void OnIsActionIconVisiblePropertyChanged(bool oldValue, bool newValue);

		static inline const wil::single_threaded_property<DependencyProperty> HeaderProperty = DependencyProperty::Register(
			L"Header",
			winrt::xaml_typename<IInspectable>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, [](auto&& d, auto&& e)
				{ 
					auto self = winrt::get_self<SettingsCard>(d.template as<class_type>())->get_strong();
					self->OnHeaderPropertyChanged(e.OldValue().as<IInspectable>(), e.NewValue().as<IInspectable>());
				}));

		static inline const wil::single_threaded_property<DependencyProperty> DescriptionProperty = DependencyProperty::Register(
			L"Description",
			winrt::xaml_typename<IInspectable>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, [](auto&& d, auto&& e)
				{ 
					auto self = winrt::get_self<SettingsCard>(d.template as<class_type>())->get_strong();
					self->OnDescriptionPropertyChanged(e.OldValue().as<IInspectable>(), e.NewValue().as<IInspectable>());
				}));

		static inline const wil::single_threaded_property<DependencyProperty> HeaderIconProperty = DependencyProperty::Register(
			L"HeaderIcon",
			winrt::xaml_typename<IconElement>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, [](auto&& d, auto&& e)
				{ 
					auto self = winrt::get_self<SettingsCard>(d.template as<class_type>())->get_strong();
					self->OnHeaderIconPropertyChanged(e.OldValue().as<IconElement>(), e.NewValue().as<IconElement>());
				}));

		static inline const wil::single_threaded_property<DependencyProperty> ActionIconProperty = DependencyProperty::Register(
			L"ActionIcon",
			winrt::xaml_typename<IconElement>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(winrt::box_value(L"\ue974")));

		static inline const wil::single_threaded_property<DependencyProperty> ActionIconToolTipProperty = DependencyProperty::Register(
			L"ActionIconToolTip",
			winrt::xaml_typename<hstring>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<DependencyProperty> IsClickEnabledProperty = DependencyProperty::Register(
			L"IsClickEnabled",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(winrt::box_value(false), [](auto&& d, auto&& e)
				{ 
					auto self = winrt::get_self<SettingsCard>(d.template as<class_type>())->get_strong();
					self->OnIsClickEnabledPropertyChanged(e.OldValue().as<bool>(), e.NewValue().as<bool>());
				}));

		static inline const wil::single_threaded_property<DependencyProperty> ContentAlignmentProperty = DependencyProperty::Register(
			L"ContentAlignment",
			winrt::xaml_typename<CommunityToolkit::WinUI::Controls::ContentAlignment>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(winrt::box_value(CommunityToolkit::WinUI::Controls::ContentAlignment::Right)));

		static inline const wil::single_threaded_property<DependencyProperty> IsActionIconVisibleProperty = DependencyProperty::Register(
			L"IsActionIconVisible",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(winrt::box_value(true), [](auto&& d, auto&& e)
				{ 
					auto self = winrt::get_self<SettingsCard>(d.template as<class_type>())->get_strong();
					self->OnIsActionIconVisiblePropertyChanged(e.OldValue().as<bool>(), e.NewValue().as<bool>());
				}));

		IInspectable Header() const { return GetValue(HeaderProperty); }

		void Header(IInspectable const& value) const { SetValue(HeaderProperty, value); }

		IInspectable Description() const { return GetValue(DescriptionProperty); }

		void Description(IInspectable const& value) const { SetValue(DescriptionProperty, value); }

		IconElement HeaderIcon() const { return winrt::unbox_value<IconElement>(GetValue(HeaderIconProperty)); }

		void HeaderIcon(IconElement const& value) const { SetValue(HeaderIconProperty, value); }

		IconElement ActionIcon() const { return winrt::unbox_value<IconElement>(GetValue(ActionIconProperty)); }

		void ActionIcon(IconElement const& value) const { SetValue(ActionIconProperty, value); }

		winrt::hstring ActionIconToolTip() const { return winrt::unbox_value<winrt::hstring>(GetValue(ActionIconToolTipProperty)); }

		void ActionIconToolTip(winrt::hstring const& value) const { SetValue(ActionIconToolTipProperty, winrt::box_value(value)); }

		bool IsClickEnabled() const { return winrt::unbox_value<bool>(GetValue(IsClickEnabledProperty)); }

		void IsClickEnabled(bool value) const { SetValue(IsClickEnabledProperty, winrt::box_value(value)); }

		winrt::CommunityToolkit::WinUI::Controls::ContentAlignment ContentAlignment() const 
		{
			return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::ContentAlignment>(
				GetValue(ContentAlignmentProperty));
		}

		void ContentAlignment(winrt::CommunityToolkit::WinUI::Controls::ContentAlignment value) const
		{ 
			SetValue(ContentAlignmentProperty, winrt::box_value(value));
		}

		bool IsActionIconVisible() const { return winrt::unbox_value<bool>(GetValue(IsActionIconVisibleProperty)); }

		void IsActionIconVisible(bool value) const { SetValue(IsActionIconVisibleProperty, winrt::box_value(value)); }
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct SettingsCard : SettingsCardT<SettingsCard, implementation::SettingsCard>
	{
	};
}
