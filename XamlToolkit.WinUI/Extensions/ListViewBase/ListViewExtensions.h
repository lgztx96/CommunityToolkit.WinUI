#pragma once

#include "ListViewExtensions.g.h"

#ifdef __INTELLISENSE__
#include <optional>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#endif

namespace winrt
{
	using namespace ::winrt::Windows::ApplicationModel;
	using namespace ::winrt::Windows::Foundation;
	using namespace ::winrt::Windows::Foundation::Collections;
	using namespace ::winrt::Microsoft::UI::Xaml;
	using namespace ::winrt::Microsoft::UI::Xaml::Input;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls;
	using namespace ::winrt::Microsoft::UI::Xaml::Media;
	using namespace ::winrt::Microsoft::UI::Xaml::Data;
	using namespace ::winrt::Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct EventContext : winrt::implements<EventContext, winrt::IInspectable>
	{
		winrt::ListViewBase::ContainerContentChanging_revoker _colorContentChangingRevoker;
		winrt::ListViewBase::ContainerContentChanging_revoker _itemTemplateContentChangingRevoker;
		winrt::ListViewBase::ContainerContentChanging_revoker _stretchDirectionContentChangingRevoker;
		winrt::IObservableVector<winrt::IInspectable>::VectorChanged_revoker _itemsVectorChangedRevoker;
		winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;
	};

	struct CommandContext : winrt::implements<CommandContext, winrt::IInspectable>
	{
		winrt::ListViewBase::ItemClick_revoker _itemClickRevoker;
	};

	struct ListViewExtensions
	{
		static void OnItemContainerStretchDirectionPropertyChanged(
			winrt::DependencyObject const& d,
			winrt::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ItemContainerStretchDirectionProperty =
			winrt::DependencyProperty::RegisterAttached(
				L"ItemContainerStretchDirection",
				winrt::xaml_typename<ItemContainerStretchDirection>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata{ nullptr, &ListViewExtensions::OnItemContainerStretchDirectionPropertyChanged });

		static std::optional<ItemContainerStretchDirection> GetItemContainerStretchDirection(
			winrt::ListViewBase const& obj)
		{
			return obj.GetValue(ItemContainerStretchDirectionProperty()).try_as<ItemContainerStretchDirection>();
		}

		static void SetItemContainerStretchDirection(
			winrt::ListViewBase const& obj,
			winrt::IReference<ItemContainerStretchDirection> const& value)
		{
			obj.SetValue(ItemContainerStretchDirectionProperty(), value);
		}

		static void DeselectAll(winrt::ListViewBase const& list);

		static void SelectAllSafe(winrt::ListViewBase const& list);

#pragma region AlternateRows
	private:
		static inline const wil::single_threaded_property<winrt::DependencyProperty> EventContextProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"EventContext",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr));

		static winrt::com_ptr<EventContext> GetEventContext(winrt::ListViewBase const& listViewBase);

	public:
		static void OnAlternateColorPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static void OnAlternateItemTemplatePropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AlternateColorProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"AlternateColor",
				winrt::xaml_typename<winrt::Brush>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateColorPropertyChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AlternateItemTemplateProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"AlternateItemTemplate",
				winrt::xaml_typename<winrt::DataTemplate>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateItemTemplatePropertyChanged));

		static winrt::Brush GetAlternateColor(winrt::ListViewBase const& obj)
		{
			return obj.GetValue(AlternateColorProperty()).try_as<winrt::Brush>();
		}

		static void SetAlternateColor(winrt::ListViewBase const& obj, winrt::Brush const& value)
		{
			obj.SetValue(AlternateColorProperty(), value);
		}

		static winrt::DataTemplate GetAlternateItemTemplate(winrt::ListViewBase const& obj)
		{
			return obj.GetValue(AlternateItemTemplateProperty()).try_as<winrt::DataTemplate>();
		}

		static void SetAlternateItemTemplate(winrt::ListViewBase const& obj, winrt::DataTemplate const& value)
		{
			obj.SetValue(AlternateItemTemplateProperty(), value);
		}

	private:
		static void ColorContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void ItemTemplateContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void ItemContainerStretchDirectionChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void OnListViewBaseUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		static void ColorItemsVectorChanged(winrt::ListViewBase const& listViewBase, winrt::IObservableVector<winrt::IInspectable> const& sender, winrt::IVectorChangedEventArgs const& args);

		static void SetItemContainerBackground(winrt::ListViewBase const& sender, winrt::Control const& itemContainer, int itemIndex);
#pragma endregion

#pragma region Command
	public:
		static void OnCommandPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CommandProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"Command",
				winrt::xaml_typename<winrt::ICommand>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnCommandPropertyChanged));

		static winrt::ICommand GetCommand(winrt::ListViewBase obj)
		{
			return obj.GetValue(CommandProperty()).try_as<winrt::ICommand>();
		}

		static void SetCommand(winrt::ListViewBase const& obj, winrt::ICommand const& value)
		{
			obj.SetValue(CommandProperty(), value);
		}

	private:
		static inline const wil::single_threaded_property<winrt::DependencyProperty> CommandContextProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"CommandContext",
				winrt::xaml_typename<winrt::IInspectable>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr));

		static winrt::com_ptr<CommandContext> GetCommandContext(winrt::ListViewBase const& listViewBase);

		static void OnListViewBaseItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e);
#pragma endregion

#pragma region SmoothScrollIntoView
	public:
		static winrt::IAsyncAction SmoothScrollIntoViewWithIndexAsync(winrt::ListViewBase listViewBase, int index, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);

		static winrt::IAsyncAction SmoothScrollIntoViewWithItemAsync(winrt::ListViewBase listViewBase, winrt::IInspectable const& item, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);

	private:
		static winrt::IAsyncAction ChangeViewAsync(winrt::ScrollViewer const& scrollViewer, std::optional<double> horizontalOffset, std::optional<double> verticalOffset, std::optional<float> zoomFactor, bool disableAnimation);
#pragma endregion
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct ListViewExtensions : ListViewExtensionsT<ListViewExtensions, implementation::ListViewExtensions>
	{
	};
}
