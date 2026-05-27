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
	using namespace Microsoft::UI::Xaml;
	using namespace Windows::ApplicationModel;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Data;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct ListViewExtensions : ListViewExtensionsT<ListViewExtensions>
	{
		static void OnItemContainerStretchDirectionPropertyChanged(
			Microsoft::UI::Xaml::DependencyObject const& d,
			Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> ItemContainerStretchDirectionProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::RegisterAttached(
				L"ItemContainerStretchDirection",
				winrt::xaml_typename<ItemContainerStretchDirection>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{ nullptr, &ListViewExtensions::OnItemContainerStretchDirectionPropertyChanged });

		static std::optional<ItemContainerStretchDirection> GetItemContainerStretchDirection(
			Microsoft::UI::Xaml::Controls::ListViewBase const& obj)
		{
			return obj.GetValue(ItemContainerStretchDirectionProperty()).try_as<ItemContainerStretchDirection>();
		}

		static void SetItemContainerStretchDirection(
			Microsoft::UI::Xaml::Controls::ListViewBase const& obj,
			winrt::Windows::Foundation::IReference<ItemContainerStretchDirection> const& value)
		{
			obj.SetValue(ItemContainerStretchDirectionProperty(), value);
		}

		static void DeselectAll(ListViewBase const& list);

		static void SelectAllSafe(ListViewBase const& list);

#pragma region AlternateRows
	private:

		struct EventContext
		{
			EventContext() = default;
			ListViewBase _listViewBase{ nullptr };
			ListViewBase::ContainerContentChanging_revoker _colorContentChangingRevoker;
			ListViewBase::ContainerContentChanging_revoker _itemTemplateContentChangingRevoker;
			ListViewBase::ContainerContentChanging_revoker _stretchDirectionContentChangingRevoker;
			IObservableVector<IInspectable>::VectorChanged_revoker _itemsVectorChangedRevoker;
			FrameworkElement::Unloaded_revoker _unloadedRevoker;
		};

		static inline std::map<IObservableVector<IInspectable>, std::unique_ptr<EventContext>> _trackedListViews;

		static inline std::map<ListViewBase, std::unique_ptr<EventContext>> _listViewEventContexts;

	public:
		static void OnAlternateColorPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args);

		static void OnAlternateItemTemplatePropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlternateColorProperty
			= DependencyProperty::RegisterAttached(
				L"AlternateColor",
				winrt::xaml_typename<Brush>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateColorPropertyChanged));

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> AlternateItemTemplateProperty
			= DependencyProperty::RegisterAttached(
				L"AlternateItemTemplate",
				winrt::xaml_typename<DataTemplate>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(nullptr, &ListViewExtensions::OnAlternateItemTemplatePropertyChanged));

		static Brush GetAlternateColor(ListViewBase const& obj)
		{
			return obj.GetValue(AlternateColorProperty).try_as<Brush>();
		}

		static void SetAlternateColor(ListViewBase const& obj, Brush const& value)
		{
			obj.SetValue(AlternateColorProperty, value);
		}

		static DataTemplate GetAlternateItemTemplate(ListViewBase const& obj)
		{
			return obj.GetValue(AlternateItemTemplateProperty).try_as<DataTemplate>();
		}

		static void SetAlternateItemTemplate(ListViewBase const& obj, DataTemplate const& value)
		{
			obj.SetValue(AlternateItemTemplateProperty, value);
		}

	private:
		static void ColorContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args);

		static void ItemTemplateContainerContentChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args);

		static void ItemContainerStretchDirectionChanging(ListViewBase const& sender, ContainerContentChangingEventArgs const& args);

		static void OnListViewBaseUnloaded(IInspectable const& sender, RoutedEventArgs const& e);

		static void OnListViewBaseUnloaded_AltRow(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e);

		static void ColorItemsVectorChanged(IObservableVector<IInspectable> const& sender, IVectorChangedEventArgs const& args);

		static void SetItemContainerBackground(ListViewBase const& sender, Control const& itemContainer, int itemIndex);
#pragma endregion


#pragma region Command
	public:
		static void OnCommandPropertyChanged(DependencyObject const& sender, DependencyPropertyChangedEventArgs const& args);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> CommandProperty
			= DependencyProperty::RegisterAttached(
				L"Command",
				winrt::xaml_typename<ICommand>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata(nullptr, &ListViewExtensions::OnCommandPropertyChanged));

		static ICommand GetCommand(ListViewBase obj)
		{
			return obj.GetValue(CommandProperty).try_as<ICommand>();
		}

		static void SetCommand(ListViewBase const& obj, ICommand const& value)
		{
			obj.SetValue(CommandProperty, value);
		}

	private:
		struct WeakListViewHash
		{
			size_t operator()(winrt::weak_ref<ListViewBase> const& wref) const noexcept
			{
				if (auto ref = wref.get())
				{
					return std::hash<void*>{}(winrt::get_abi(ref));
				}

				return 0;
			}
		};

		static inline std::unordered_map<winrt::weak_ref<ListViewBase>, std::unique_ptr<ListViewBase::ItemClick_revoker>, WeakListViewHash> _commandTokens;

		static void OnListViewBaseItemClick(IInspectable const& sender, ItemClickEventArgs const& e);
#pragma endregion

#pragma region SmoothScrollIntoView
	public:
		static IAsyncAction SmoothScrollIntoViewWithIndexAsync(ListViewBase const& listViewBase, int index, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);


		static IAsyncAction SmoothScrollIntoViewWithItemAsync(ListViewBase const& listViewBase, IInspectable const& item, ScrollItemPlacement itemPlacement = ScrollItemPlacement::Default, bool disableAnimation = false, bool scrollIfVisible = true, int additionalHorizontalOffset = 0, int additionalVerticalOffset = 0);

	private:
		static IAsyncAction ChangeViewAsync(ScrollViewer const& scrollViewer, std::optional<double> horizontalOffset, std::optional<double> verticalOffset, std::optional<float> zoomFactor, bool disableAnimation);
#pragma endregion
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct ListViewExtensions : ListViewExtensionsT<ListViewExtensions, implementation::ListViewExtensions>
	{
	};
}
