#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#endif
#include "Connected.h"
#if __has_include("Connected.g.cpp")
#include "Connected.g.cpp"
#endif

namespace
{
    uintptr_t GetObjectKey(winrt::IInspectable const& value)
    {
        return reinterpret_cast<uintptr_t>(winrt::get_abi(value));
    }

    winrt::Frame FindAncestorFrame(winrt::DependencyObject current)
    {
        while (current)
        {
            if (auto frame = current.try_as<winrt::Frame>())
            {
                return frame;
            }

            current = winrt::VisualTreeHelper::GetParent(current);
        }

        return nullptr;
    }

    std::unordered_map<uintptr_t, std::shared_ptr<winrt::XamlToolkit::WinUI::Animations::ConnectedAnimationHelper>> frameHelpers;
    std::unordered_map<uintptr_t, std::map<winrt::hstring, winrt::XamlToolkit::WinUI::Animations::ConnectedAnimationProperties>> pageConnectedAnimationProperties;
    std::unordered_map<uintptr_t, std::unordered_map<uintptr_t, std::vector<winrt::UIElement>>> pageCoordinatedAnimationElements;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> Connected::KeyProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Key",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr, &Connected::OnKeyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::AnchorElementProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"AnchorElement",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr, &Connected::OnAnchorElementChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::ListItemKeyProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ListItemKey",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr, &Connected::OnListItemKeyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::ListItemElementNameProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ListItemElementName",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr, &Connected::OnListItemElementNameChanged));

    winrt::hstring Connected::GetKey(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(KeyProperty), L"");
    }

    void Connected::SetKey(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(KeyProperty, winrt::box_value(value));
    }

    winrt::UIElement Connected::GetAnchorElement(winrt::DependencyObject const& obj)
    {
        return obj.GetValue(AnchorElementProperty).try_as<winrt::UIElement>();
    }

    void Connected::SetAnchorElement(winrt::DependencyObject const& obj, winrt::UIElement const& value)
    {
        obj.SetValue(AnchorElementProperty, value);
    }

    winrt::hstring Connected::GetListItemKey(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(ListItemKeyProperty), L"");
    }

    void Connected::SetListItemKey(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(ListItemKeyProperty, winrt::box_value(value));
    }

    winrt::hstring Connected::GetListItemElementName(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(ListItemElementNameProperty), L"");
    }

    void Connected::SetListItemElementName(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(ListItemElementNameProperty, winrt::box_value(value));
    }

    std::shared_ptr<ConnectedAnimationHelper> Connected::GetConnectedAnimationHelper(winrt::Frame const& frame)
    {
        if (!frame)
        {
            return nullptr;
        }

        uintptr_t frameKey = GetObjectKey(frame);
        auto it = frameHelpers.find(frameKey);

        if (it != frameHelpers.end())
        {
            return it->second;
        }

        auto helper = std::make_shared<winrt::XamlToolkit::WinUI::Animations::ConnectedAnimationHelper>(frame);
        frameHelpers[frameKey] = helper;

        return helper;
    }

    std::map<winrt::hstring, ConnectedAnimationProperties>& Connected::GetPageConnectedAnimationProperties(winrt::Page const& page)
    {
        return pageConnectedAnimationProperties[GetObjectKey(page)];
    }

    std::unordered_map<uintptr_t, std::vector<winrt::UIElement>>& Connected::GetPageCoordinatedAnimationElements(winrt::Page const& page)
    {
        return pageCoordinatedAnimationElements[GetObjectKey(page)];
    }

    void Connected::RegisterElementForConnectedAnimation(
        winrt::Page const& page,
        winrt::hstring const& key,
        winrt::UIElement const& element,
        winrt::IIterable<winrt::UIElement> const& anchors)
    {
        if (!page || key.empty() || !element)
        {
            return;
        }

        auto& props = GetPageConnectedAnimationProperties(page);

        ConnectedAnimationProperties animation;
        animation.Key = key;
        animation.Element = element;
        props[key] = animation;

        if (anchors)
        {
            for (const auto& anchor : anchors)
            {
                AttachAnchorElementForConnectedAnimation(page, element, anchor);
            }
        }
    }

    void Connected::UnregisterElementForConnectedAnimation(winrt::Page const& page, winrt::hstring const& key)
    {
        if (!page || key.empty())
        {
            return;
        }

        auto& props = GetPageConnectedAnimationProperties(page);
        props.erase(key);
    }

    void Connected::AttachAnchorElementForConnectedAnimation(winrt::Page const& page, winrt::UIElement const& element, winrt::UIElement const& anchor)
    {
        if (!page || !element || !anchor)
        {
            return;
        }

        auto& coordinatedElements = GetPageCoordinatedAnimationElements(page);
        coordinatedElements[GetObjectKey(anchor)].push_back(element);
    }

    void Connected::RemoveAnchoredElementForConnectedAnimation(winrt::Page const& page, winrt::UIElement const& element, winrt::UIElement const& anchor)
    {
        if (!page || !element || !anchor)
        {
            return;
        }

        auto& coordinatedElements = GetPageCoordinatedAnimationElements(page);

        auto it = coordinatedElements.find(GetObjectKey(anchor));
        if (it == coordinatedElements.end())
        {
            return;
        }

        auto& anchored = it->second;

        std::erase_if(anchored, [&](winrt::UIElement const& current)
        {
            return GetObjectKey(current) == GetObjectKey(element);
        });
    }

    void Connected::RegisterListItemForConnectedAnimation(
        winrt::Page const& page,
        winrt::ListViewBase const& listViewBase,
        winrt::hstring const& key,
        winrt::hstring const& elementName)
    {
        if (!page || !listViewBase || key.empty() || elementName.empty())
        {
            return;
        }

        auto& props = GetPageConnectedAnimationProperties(page);

        auto it = props.find(key);
        if (it == props.end())
        {
            ConnectedAnimationProperties value;
            value.Key = key;
            props.emplace(key, value);
            it = props.find(key);
        }

        auto& listProps = it->second.ListAnimProperties;

        auto existing = std::find_if(
            listProps.begin(),
            listProps.end(),
            [&](ConnectedAnimationListProperty const& item)
            {
                return GetObjectKey(item.ListViewBase) == GetObjectKey(listViewBase) && item.ElementName == elementName;
            });

        if (existing == listProps.end())
        {
            ConnectedAnimationListProperty prop;
            prop.ListViewBase = listViewBase;
            prop.ElementName = elementName;
            listProps.push_back(prop);
        }
    }

    void Connected::UnregisterListItemForConnectedAnimation(winrt::Page const& page, winrt::ListViewBase const& listViewBase, winrt::hstring const& key)
    {
        if (!page || !listViewBase || key.empty())
        {
            return;
        }

        auto& props = GetPageConnectedAnimationProperties(page);

        auto it = props.find(key);
        if (it == props.end())
        {
            return;
        }

        auto& listProps = it->second.ListAnimProperties;

        std::erase_if(listProps, [&](ConnectedAnimationListProperty const& item)
        {
            return GetObjectKey(item.ListViewBase) == GetObjectKey(listViewBase);
        });

        if (listProps.empty() && !it->second.Element)
        {
            props.erase(it);
        }
    }

    void Connected::SetListDataItemForNextConnectedAnimation(winrt::Frame const& frame, winrt::IInspectable const& item)
    {
        if (auto helper = GetConnectedAnimationHelper(frame))
        {
            helper->SetParameterForNextFrameNavigation(item);
        }
    }

    void Connected::OnKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::FrameworkElement>();

        if (!element)
        {
            return;
        }

        GetParentFrameAndExecuteAction(element, [element, e](winrt::Frame const& frame)
        {
            auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            (void)GetConnectedAnimationHelper(frame);

            auto oldKey = winrt::unbox_value_or<winrt::hstring>(e.OldValue(), L"");
            if (!oldKey.empty())
            {
                UnregisterElementForConnectedAnimation(page, oldKey);
            }

            auto newKey = winrt::unbox_value_or<winrt::hstring>(e.NewValue(), L"");
            if (!newKey.empty())
            {
                RegisterElementForConnectedAnimation(page, newKey, element, nullptr);
            }
        });
    }

    void Connected::OnAnchorElementChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::FrameworkElement>();

        if (!element)
        {
            return;
        }

        GetParentFrameAndExecuteAction(element, [element, e](winrt::Frame const& frame)
        {
            auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            (void)GetConnectedAnimationHelper(frame);

            if (auto oldAnchor = e.OldValue().try_as<winrt::UIElement>())
            {
                RemoveAnchoredElementForConnectedAnimation(page, element, oldAnchor);
            }

            if (auto newAnchor = e.NewValue().try_as<winrt::UIElement>())
            {
                AttachAnchorElementForConnectedAnimation(page, element, newAnchor);
            }
        });
    }

    void Connected::OnListItemKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto listViewBase = d.try_as<winrt::ListViewBase>();

        if (!listViewBase)
        {
            return;
        }

        GetParentFrameAndExecuteAction(listViewBase, [listViewBase, e](winrt::Frame const& frame)
        {
            auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            (void)GetConnectedAnimationHelper(frame);

            auto oldKey = winrt::unbox_value_or<winrt::hstring>(e.OldValue(), L"");
            if (!oldKey.empty())
            {
                UnregisterListItemForConnectedAnimation(page, listViewBase, oldKey);
            }

            AddListViewBaseItemAnimationDetails(page, listViewBase);
        });
    }

    void Connected::OnListItemElementNameChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto listViewBase = d.try_as<winrt::ListViewBase>();

        if (!listViewBase)
        {
            return;
        }

        GetParentFrameAndExecuteAction(listViewBase, [listViewBase, e](winrt::Frame const& frame)
        {
            auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            (void)GetConnectedAnimationHelper(frame);

            auto oldElementName = winrt::unbox_value_or<winrt::hstring>(e.OldValue(), L"");
            if (!oldElementName.empty())
            {
                auto key = GetListItemKey(listViewBase);
                if (!key.empty())
                {
                    UnregisterListItemForConnectedAnimation(page, listViewBase, key);
                }
            }

            AddListViewBaseItemAnimationDetails(page, listViewBase);
        });
    }

    void Connected::GetParentFrameAndExecuteAction(
        winrt::FrameworkElement const& element,
        std::function<void(winrt::Frame const&)> const& action)
    {
        if (!element)
        {
            return;
        }

        auto frame = FindAncestorFrame(element);

        if (frame)
        {
            action(frame);
            return;
        }

        auto weakElement = winrt::make_weak(element);
        auto token = std::make_shared<winrt::event_token>();

        *token = element.Loaded([weakElement, token, action](winrt::IInspectable const&, winrt::RoutedEventArgs const&)
        {
            if (auto value = weakElement.get())
            {
                value.Loaded(*token);

                if (auto frame = FindAncestorFrame(value))
                {
                    action(frame);
                }
            }
        });
    }

    void Connected::AddListViewBaseItemAnimationDetails(winrt::Page const& page, winrt::ListViewBase const& listViewBase)
    {
        if (!page || !listViewBase)
        {
            return;
        }

        auto elementName = GetListItemElementName(listViewBase);
        auto key = GetListItemKey(listViewBase);

        if (elementName.empty() || key.empty())
        {
            return;
        }

        RegisterListItemForConnectedAnimation(page, listViewBase, key, elementName);
    }
}
