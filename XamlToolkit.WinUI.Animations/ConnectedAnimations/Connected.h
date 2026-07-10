#pragma once

#include "Connected.g.h"
#include "ConnectedAnimationHelper.h"
#include "ConnectedAnimationProperties.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A helper class that allows Connected Animations to be enabled through XAML.
    /// </summary>
    struct Connected : ConnectedT<Connected>
    {
    public:
        static winrt::hstring GetKey(winrt::DependencyObject const& obj);
        static void SetKey(winrt::DependencyObject const& obj, winrt::hstring const& value);

        static winrt::UIElement GetAnchorElement(winrt::DependencyObject const& obj);
        static void SetAnchorElement(winrt::DependencyObject const& obj, winrt::UIElement const& value);

        static winrt::hstring GetListItemKey(winrt::DependencyObject const& obj);
        static void SetListItemKey(winrt::DependencyObject const& obj, winrt::hstring const& value);

        static winrt::hstring GetListItemElementName(winrt::DependencyObject const& obj);
        static void SetListItemElementName(winrt::DependencyObject const& obj, winrt::hstring const& value);

        static void RegisterElementForConnectedAnimation(
            winrt::Page const& page,
            winrt::hstring const& key,
            winrt::UIElement const& element,
            winrt::IIterable<winrt::UIElement> const& anchors);

        static void UnregisterElementForConnectedAnimation(
            winrt::Page const& page,
            winrt::hstring const& key);

        static void AttachAnchorElementForConnectedAnimation(
            winrt::Page const& page,
            winrt::UIElement const& element,
            winrt::UIElement const& anchor);

        static void RemoveAnchoredElementForConnectedAnimation(
            winrt::Page const& page,
            winrt::UIElement const& element,
            winrt::UIElement const& anchor);

        static void RegisterListItemForConnectedAnimation(
            winrt::Page const& page,
            winrt::ListViewBase const& listViewBase,
            winrt::hstring const& key,
            winrt::hstring const& elementName);

        static void UnregisterListItemForConnectedAnimation(
            winrt::Page const& page,
            winrt::ListViewBase const& listViewBase,
            winrt::hstring const& key);

        static void SetListDataItemForNextConnectedAnimation(
            winrt::Frame const& frame,
            winrt::IInspectable const& item);

        static std::map<winrt::hstring, ConnectedAnimationProperties>& GetPageConnectedAnimationProperties(
            winrt::Page const& page);

        static std::unordered_map<uintptr_t, std::vector<winrt::UIElement>>& GetPageCoordinatedAnimationElements(
            winrt::Page const& page);

        static std::shared_ptr<ConnectedAnimationHelper> GetConnectedAnimationHelper(
            winrt::Frame const& frame);

        static const wil::single_threaded_property<winrt::DependencyProperty> KeyProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> AnchorElementProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ListItemKeyProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ListItemElementNameProperty;

    private:
        static void OnKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void OnAnchorElementChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void OnListItemKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);
        static void OnListItemElementNameChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void GetParentFrameAndExecuteAction(
            winrt::FrameworkElement const& element,
            std::function<void(winrt::Controls::Frame const&)> const& action);

        static void AddListViewBaseItemAnimationDetails(
            winrt::Page const& page,
            winrt::ListViewBase const& listViewBase);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct Connected : ConnectedT<Connected, implementation::Connected> {};
}
