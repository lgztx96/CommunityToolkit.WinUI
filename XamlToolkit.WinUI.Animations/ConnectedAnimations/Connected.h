#pragma once

#include "Connected.g.h"
#include "ConnectedAnimationHelper.h"
#include "ConnectedAnimationProperties.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#endif

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A helper class that allows Connected Animations to be enabled through XAML.
    /// </summary>
    struct Connected : ConnectedT<Connected>
    {
    public:
        static winrt::hstring GetKey(Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetKey(Microsoft::UI::Xaml::DependencyObject const& obj, winrt::hstring const& value);

        static Microsoft::UI::Xaml::UIElement GetAnchorElement(Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetAnchorElement(Microsoft::UI::Xaml::DependencyObject const& obj, Microsoft::UI::Xaml::UIElement const& value);

        static winrt::hstring GetListItemKey(Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetListItemKey(Microsoft::UI::Xaml::DependencyObject const& obj, winrt::hstring const& value);

        static winrt::hstring GetListItemElementName(Microsoft::UI::Xaml::DependencyObject const& obj);
        static void SetListItemElementName(Microsoft::UI::Xaml::DependencyObject const& obj, winrt::hstring const& value);

        static void RegisterElementForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            winrt::hstring const& key,
            Microsoft::UI::Xaml::UIElement const& element,
            winrt::Windows::Foundation::Collections::IIterable<Microsoft::UI::Xaml::UIElement> const& anchors);

        static void UnregisterElementForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            winrt::hstring const& key);

        static void AttachAnchorElementForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            Microsoft::UI::Xaml::UIElement const& element,
            Microsoft::UI::Xaml::UIElement const& anchor);

        static void RemoveAnchoredElementForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            Microsoft::UI::Xaml::UIElement const& element,
            Microsoft::UI::Xaml::UIElement const& anchor);

        static void RegisterListItemForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            Microsoft::UI::Xaml::Controls::ListViewBase const& listViewBase,
            winrt::hstring const& key,
            winrt::hstring const& elementName);

        static void UnregisterListItemForConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Page const& page,
            Microsoft::UI::Xaml::Controls::ListViewBase const& listViewBase,
            winrt::hstring const& key);

        static void SetListDataItemForNextConnectedAnimation(
            Microsoft::UI::Xaml::Controls::Frame const& frame,
            winrt::Windows::Foundation::IInspectable const& item);

        static std::map<winrt::hstring, ConnectedAnimationProperties>& GetPageConnectedAnimationProperties(
            Microsoft::UI::Xaml::Controls::Page const& page);

        static std::unordered_map<uintptr_t, std::vector<Microsoft::UI::Xaml::UIElement>>& GetPageCoordinatedAnimationElements(
            Microsoft::UI::Xaml::Controls::Page const& page);

        static std::shared_ptr<ConnectedAnimationHelper> GetConnectedAnimationHelper(
            Microsoft::UI::Xaml::Controls::Frame const& frame);

        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> KeyProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> AnchorElementProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ListItemKeyProperty;
        static const wil::single_threaded_property<Microsoft::UI::Xaml::DependencyProperty> ListItemElementNameProperty;

    private:
        static void OnKeyChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void OnAnchorElementChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void OnListItemKeyChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);
        static void OnListItemElementNameChanged(Microsoft::UI::Xaml::DependencyObject const& d, Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e);

        static void GetParentFrameAndExecuteAction(
            Microsoft::UI::Xaml::FrameworkElement const& element,
            std::function<void(Microsoft::UI::Xaml::Controls::Frame const&)> const& action);

        static void AddListViewBaseItemAnimationDetails(
            Microsoft::UI::Xaml::Controls::Page const& page,
            Microsoft::UI::Xaml::Controls::ListViewBase const& listViewBase);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Connected : ConnectedT<Connected, implementation::Connected> {};
}
