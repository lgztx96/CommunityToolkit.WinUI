#pragma once

#include "Ribbon.g.h"

#ifdef __INTELLISENSE__
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct Ribbon : RibbonT<Ribbon>
	{
	private:
		static constexpr std::wstring_view PanelTemplatePart = L"Panel";
		static constexpr std::wstring_view ScrollViewerTemplatePart = L"ScrollViewer";
		static constexpr std::wstring_view ScrollDecrementButtonTempatePart = L"ScrollDecrementButton";
		static constexpr std::wstring_view ScrollIncrementButtonTempatePart = L"ScrollIncrementButton";
		static constexpr std::wstring_view ScrollButtonGroupNameTemplatePart = L"ScrollButtonGroup";
		static constexpr std::wstring_view NoButtonsStateTemplatePart = L"NoButtons";
		static constexpr std::wstring_view DecrementButtonStateTemplatePart = L"DecrementButton";
		static constexpr std::wstring_view IncrementButtonStateTemplatePart = L"IncrementButton";
		static constexpr std::wstring_view BothButtonsStateTemplatePart = L"BothButtons";

		Panel _panel{ nullptr };
		ScrollViewer _scrollViewer{ nullptr };
		ButtonBase _decrementButton{ nullptr };
		ButtonBase _incrementButton{ nullptr };
		IObservableVector<UIElement> _items{ nullptr };

		Panel::SizeChanged_revoker _panelSizeChangedRevoker;
		ButtonBase::Click_revoker _incrementButtonClickRevoker;
		ButtonBase::Click_revoker _decrementButtonClickRevoker;
		ScrollViewer::ViewChanged_revoker _scrollViewerViewChangedRevoker;
		ScrollViewer::SizeChanged_revoker _scrollViewerSizeChangedRevoker;

	public:
		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ScrollStepProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"ScrollStep",
				winrt::xaml_typename<double>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(20.0) });

		double ScrollStep() const
		{
			return winrt::unbox_value<double>(GetValue(ScrollStepProperty));
		}

		void ScrollStep(double value)
		{
			SetValue(ScrollStepProperty, winrt::box_value(value));
		}

	public:
		Ribbon();

		IVector<UIElement> Items() { return _items; }

		void OnApplyTemplate();

	private:
		void OnItemsVectorChanged(IObservableVector<UIElement> const& sender, IVectorChangedEventArgs const& args);

        void OnViewChanged(IInspectable const& sender, ScrollViewerViewChangedEventArgs const& e);

        void OnSizeChanged(IInspectable const& sender, SizeChangedEventArgs const& e);

        void UpdateScrollButtonsState();

        void OnDecrementScrollViewer(IInspectable const& sender, RoutedEventArgs const& e);

        void OnIncrementScrollViewer(IInspectable const& sender, RoutedEventArgs const& e);
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct Ribbon : RibbonT<Ribbon, implementation::Ribbon>
	{
	};
}
