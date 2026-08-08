#pragma once

#include "ListViewExtensions.g.h"

#ifdef __INTELLISENSE__
#include <map>
#include <memory>
#include <optional>
#include <unordered_map>
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
	using namespace Windows::ApplicationModel;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace winrt::Input;
	using namespace winrt::Controls;
	using namespace winrt::Media;
	using namespace winrt::Data;
	using namespace winrt::Primitives;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
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

		struct EventContext
		{
			EventContext() = default;
			winrt::ListViewBase _listViewBase{ nullptr };
			winrt::ListViewBase::ContainerContentChanging_revoker _colorContentChangingRevoker;
			winrt::ListViewBase::ContainerContentChanging_revoker _itemTemplateContentChangingRevoker;
			winrt::ListViewBase::ContainerContentChanging_revoker _stretchDirectionContentChangingRevoker;
			winrt::IObservableVector<winrt::IInspectable>::VectorChanged_revoker _itemsVectorChangedRevoker;
			winrt::FrameworkElement::Unloaded_revoker _unloadedRevoker;
		};

		static inline std::map<winrt::IObservableVector<winrt::IInspectable>, std::unique_ptr<EventContext>> _trackedListViews;

		static inline std::map<winrt::ListViewBase, std::unique_ptr<EventContext>> _listViewEventContexts;

	public:
		static void OnAlternateColorPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static void OnAlternateItemTemplatePropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AlternateColorProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"AlternateColor",
				winrt::xaml_typename<Brush>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateColorPropertyChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AlternateItemTemplateProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"AlternateItemTemplate",
				winrt::xaml_typename<DataTemplate>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateItemTemplatePropertyChanged));

		static winrt::Brush GetAlternateColor(winrt::ListViewBase const& obj)
		{
			return obj.GetValue(AlternateColorProperty).try_as<winrt::Brush>();
		}

		static void SetAlternateColor(winrt::ListViewBase const& obj, winrt::Brush const& value)
		{
			obj.SetValue(AlternateColorProperty, value);
		}

		static winrt::DataTemplate GetAlternateItemTemplate(winrt::ListViewBase const& obj)
		{
			return obj.GetValue(AlternateItemTemplateProperty).try_as<winrt::DataTemplate>();
		}

		static void SetAlternateItemTemplate(winrt::ListViewBase const& obj, winrt::DataTemplate const& value)
		{
			obj.SetValue(AlternateItemTemplateProperty, value);
		}

	private:
		static void ColorContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void ItemTemplateContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void ItemContainerStretchDirectionChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& args);

		static void OnListViewBaseUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		static void OnListViewBaseUnloaded_AltRow(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e);

		static void ColorItemsVectorChanged(winrt::IObservableVector<winrt::IInspectable> const& sender, winrt::IVectorChangedEventArgs const& args);

		static void SetItemContainerBackground(winrt::ListViewBase const& sender, winrt::Control const& itemContainer, int itemIndex);
#pragma endregion


#pragma region Command
	public:
		static void OnCommandPropertyChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CommandProperty
			= winrt::DependencyProperty::RegisterAttached(
				L"Command",
				winrt::xaml_typename<ICommand>(),
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::ListViewExtensions>(),
				winrt::PropertyMetadata(nullptr, &ListViewExtensions::OnCommandPropertyChanged));

		static winrt::ICommand GetCommand(winrt::ListViewBase obj)
		{
			return obj.GetValue(CommandProperty).try_as<winrt::ICommand>();
		}

		static void SetCommand(winrt::ListViewBase const& obj, winrt::ICommand const& value)
		{
			obj.SetValue(CommandProperty, value);
		}

	private:
		struct WeakListViewHash
		{
			size_t operator()(winrt::weak_ref<winrt::ListViewBase> const& wref) const noexcept
			{
				if (auto ref = wref.get())
				{
					return std::hash<void*>{}(winrt::get_abi(ref));
				}

				return 0;
			}
		};

		static inline std::unordered_map<winrt::weak_ref<ListViewBase>, std::unique_ptr<winrt::ListViewBase::ItemClick_revoker>, WeakListViewHash> _commandTokens;

		static void OnListViewBaseItemClick(winrt::IInspectable const& sender, winrt::ItemClickEventArgs const& e);
#pragma endregion

#pragma region SmoothScrollIntoView
	public:
		static winrt::IAsyncAction SmoothScrollIntoViewWithIndexAsync(winrt::ListViewBase const& listViewBase, int index, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);


		static winrt::IAsyncAction SmoothScrollIntoViewWithItemAsync(winrt::ListViewBase const& listViewBase, winrt::IInspectable const& item, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);

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
