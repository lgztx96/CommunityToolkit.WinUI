#pragma once

#include "RibbonCollapsibleGroup.g.h"
#include "RibbonGroup.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct RibbonCollapsibleGroup : RibbonCollapsibleGroupT<RibbonCollapsibleGroup, implementation::RibbonGroup>
	{
	private:
		static constexpr std::wstring_view VisibleContentContainerTemplatePart = L"VisibleContentContainer";
		static constexpr std::wstring_view CollapsedButtonTemplatePart = L"CollapsedButton";
		static constexpr std::wstring_view CollapsedFlyoutTemplatePart = L"CollapsedFlyout";
		static constexpr std::wstring_view CollapsedContentPresenterTemplatePart = L"CollapsedContentPresenter";

		ContentControl _visibleContentContainer{ nullptr };
		ContentControl _collapsedContentContainer{ nullptr };
		Button _collapsedButton{ nullptr };
		Flyout _collapsedFlyout{ nullptr };

		IInspectable _contaionerPointerEventHandler{ nullptr };
		IInspectable _contaionerKeyEventHandler{ nullptr };

		Flyout::Opened_revoker _flyoutOpenedRevoker;

		static void OnStatePropertyChanged(
			winrt::Microsoft::UI::Xaml::DependencyObject const& sender,
			winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const&);

		static void OnRequestedWidthsChanged(
			winrt::Microsoft::UI::Xaml::DependencyObject const& sender,
			winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

	public:
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IconSourceProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IconSource",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconSource>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr });

		winrt::Microsoft::UI::Xaml::Controls::IconSource IconSource() const
		{
			return GetValue(IconSourceProperty).try_as<winrt::Microsoft::UI::Xaml::Controls::IconSource>();
		}

		void IconSource(winrt::Microsoft::UI::Xaml::Controls::IconSource const& value)
		{
			SetValue(IconSourceProperty, value);
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> StateProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"State",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Visibility>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(winrt::Microsoft::UI::Xaml::Visibility::Visible), &RibbonCollapsibleGroup::OnStatePropertyChanged });

		winrt::Microsoft::UI::Xaml::Visibility State() const
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Visibility>(GetValue(StateProperty));
		}

		void State(winrt::Microsoft::UI::Xaml::Visibility const& value)
		{
			SetValue(StateProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AutoCloseFlyoutProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"AutoCloseFlyout",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(true) });

		bool AutoCloseFlyout() const
		{
			return winrt::unbox_value<bool>(GetValue(AutoCloseFlyoutProperty));
		}

		void AutoCloseFlyout(bool value)
		{
			SetValue(AutoCloseFlyoutProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> PriorityProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Priority",
				winrt::xaml_typename<int>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0) });

		int Priority() const
		{
			return winrt::unbox_value<int>(GetValue(PriorityProperty));
		}

		void Priority(int value)
		{
			SetValue(PriorityProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CollapsedAccessKeyProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"CollapsedAccessKey",
				winrt::xaml_typename<hstring>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(L"") });

		winrt::hstring CollapsedAccessKey() const
		{
			return winrt::unbox_value<winrt::hstring>(GetValue(CollapsedAccessKeyProperty));
		}

		void CollapsedAccessKey(winrt::hstring const& value)
		{
			SetValue(CollapsedAccessKeyProperty, winrt::box_value(value));
		}

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> RequestedWidthsProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"RequestedWidths",
				winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::DoubleVector>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ nullptr, &RibbonCollapsibleGroup::OnRequestedWidthsChanged });

		winrt::XamlToolkit::Labs::WinUI::DoubleVector RequestedWidths() const
		{
			return GetValue(RequestedWidthsProperty).try_as<winrt::XamlToolkit::Labs::WinUI::DoubleVector>();
		}

		void RequestedWidths(winrt::XamlToolkit::Labs::WinUI::DoubleVector const& value)
		{
			SetValue(RequestedWidthsProperty, value);
		}

		RibbonCollapsibleGroup();

		void OnApplyTemplate();

	private:
		template<typename T>
		T Get(std::wstring_view templatePart) { return GetTemplateChild(templatePart).try_as<T>(); }

		void OnFlyoutOpened(IInspectable const& sender, IInspectable const& e);

		void OnFlyoutPointerReleased(IInspectable const& sender, PointerRoutedEventArgs const& e);

		void OnFlyoutKeyUp(IInspectable const& sender, KeyRoutedEventArgs const& e);

		void AutoCollapseFlyout(bool eventHasBeenHandled, IInspectable const& originalSource);

		bool DoesRoutedEventOriginateFromAFlyoutHost(UIElement source);

		void UpdateState();
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct RibbonCollapsibleGroup : RibbonCollapsibleGroupT<RibbonCollapsibleGroup, implementation::RibbonCollapsibleGroup>
	{
	};
}
