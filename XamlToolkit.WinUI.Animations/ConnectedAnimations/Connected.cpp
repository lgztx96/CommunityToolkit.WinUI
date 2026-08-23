#include "pch.h"
#include "winrt_module_imports.h"
#include "Connected.h"
#if __has_include("Connected.g.cpp")
#include "Connected.g.cpp"
#endif
#include "../../XamlToolkit.WinUI/common.h"

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
            winrt::xaml_typename<winrt::UIElement>(),
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

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::ConnectedAnimationHelperProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"ConnectedAnimationHelper",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::PageConnectedAnimationPropertiesProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"PageConnectedAnimationProperties",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> Connected::PageCoordinatedAnimationElementsProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"PageCoordinatedAnimationElements",
            winrt::xaml_typename<winrt::IInspectable>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Animations::Connected>(),
            winrt::PropertyMetadata(nullptr));

    winrt::hstring Connected::GetKey(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(KeyProperty()), L"");
    }

    void Connected::SetKey(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(KeyProperty(), winrt::box_value(value));
    }

    winrt::UIElement Connected::GetAnchorElement(winrt::DependencyObject const& obj)
    {
        return obj.GetValue(AnchorElementProperty()).try_as<winrt::UIElement>();
    }

    void Connected::SetAnchorElement(winrt::DependencyObject const& obj, winrt::UIElement const& value)
    {
        obj.SetValue(AnchorElementProperty(), value);
    }

    winrt::hstring Connected::GetListItemKey(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(ListItemKeyProperty()), L"");
    }

    void Connected::SetListItemKey(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(ListItemKeyProperty(), winrt::box_value(value));
    }

    winrt::hstring Connected::GetListItemElementName(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value_or<winrt::hstring>(obj.GetValue(ListItemElementNameProperty()), L"");
    }

    void Connected::SetListItemElementName(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(ListItemElementNameProperty(), winrt::box_value(value));
    }

    winrt::com_ptr<ConnectedAnimationHelper> Connected::GetConnectedAnimationHelper(winrt::Frame const& frame)
    {
        const auto value = frame.GetValue(ConnectedAnimationHelperProperty());
        if (!value)
        {
            const auto helper = winrt::make_self<ConnectedAnimationHelper>(frame);
            frame.SetValue(ConnectedAnimationHelperProperty(), *helper);
            return helper;
        }

        return winrt::get_self<ConnectedAnimationHelper>(value)->get_strong();
    }

    winrt::com_ptr<ConnectedAnimationPropertiesMap> Connected::GetPageConnectedAnimationProperties(winrt::Page const& page)
    {
        const auto value = page.GetValue(PageConnectedAnimationPropertiesProperty());
        if (!value)
        {
            const auto props = winrt::make_self<ConnectedAnimationPropertiesMap>();
            page.SetValue(PageConnectedAnimationPropertiesProperty(), *props);
            return props;
        }

        return winrt::get_self<ConnectedAnimationPropertiesMap>(value)->get_strong();
    }

    winrt::com_ptr<ConnectedAnimationElementsMap> Connected::GetPageCoordinatedAnimationElements(winrt::Page const& page)
    {
        const auto value = page.GetValue(PageCoordinatedAnimationElementsProperty());
        if (!value)
        {
            const auto elements = winrt::make_self<ConnectedAnimationElementsMap>();
            page.SetValue(PageCoordinatedAnimationElementsProperty(), *elements);
            return elements;
        }

        return winrt::get_self<ConnectedAnimationElementsMap>(value)->get_strong();
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

        const auto props = GetPageConnectedAnimationProperties(page);

        ConnectedAnimationProperties animation{ .Key = key, .Element = element };
        props->insert_or_assign(key, animation);

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

        const auto props = GetPageConnectedAnimationProperties(page);
        props->erase(key);
    }

    void Connected::AttachAnchorElementForConnectedAnimation(winrt::Page const& page, winrt::UIElement const& element, winrt::UIElement const& anchor)
    {
        if (!page || !element || !anchor)
        {
            return;
        }

        const auto coordinatedElements = GetPageCoordinatedAnimationElements(page);
        coordinatedElements->operator[](anchor).push_back(element);
    }

    void Connected::RemoveAnchoredElementForConnectedAnimation(winrt::Page const& page, winrt::UIElement const& element, winrt::UIElement const& anchor)
    {
        if (!page || !element || !anchor)
        {
            return;
        }

        const auto coordinatedElements = GetPageCoordinatedAnimationElements(page);

        auto it = coordinatedElements->find(anchor);
        if (it == coordinatedElements->end())
        {
            return;
        }

        auto& anchored = it->second;

        std::erase_if(anchored, [&](winrt::UIElement const& current)
        {
            return current == element;
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

        const auto props = GetPageConnectedAnimationProperties(page);

        auto it = props->find(key);
        if (it == props->end())
        {
            ConnectedAnimationProperties value{ .Key = key };
            it = props->emplace(key, value).first;
        }

        auto& listAnimProps = it->second.ListAnimProperties;

        if (!std::ranges::any_of(listAnimProps, [&](auto const& item)
            {
                return item.ListViewBase == listViewBase &&
                    item.ElementName == elementName;
            }))
        {
            listAnimProps.emplace_back(elementName, listViewBase);
        }
    }

    void Connected::UnregisterListItemForConnectedAnimation(winrt::Page const& page, winrt::ListViewBase const& listViewBase, winrt::hstring const& key)
    {
        if (!page || !listViewBase || key.empty())
        {
            return;
        }

        const auto props = GetPageConnectedAnimationProperties(page);

        const auto it = props->find(key);
        if (it == props->end())
        {
            return;
        }

        auto& listAnimProps = it->second.ListAnimProperties;

        std::erase_if(listAnimProps, [&](ConnectedAnimationListProperty const& item)
        {
            return item.ListViewBase == listViewBase;
        });

        if (listAnimProps.empty())
        {
            props->erase(it);
        }
    }

    void Connected::PrepareListDataItemForNextConnectedAnimation(winrt::Frame const& frame, winrt::IInspectable const& item)
    {
        if (auto helper = GetConnectedAnimationHelper(frame))
        {
            helper->SetParameterForNextFrameNavigation(item);
        }
    }

    void Connected::OnKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        const auto element = d.try_as<winrt::FrameworkElement>();

        if (!element)
        {
            return;
        }

        GetParentFrameAndExecuteAction(element, [element, e](winrt::Frame const& frame)
        {
            const auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            GetConnectedAnimationHelper(frame);

            if (const auto oldKey = e.OldValue().try_as<winrt::hstring>(); oldKey && !oldKey->empty())
            {
                UnregisterElementForConnectedAnimation(page, *oldKey);
            }

            if (const auto newKey = e.NewValue().try_as<winrt::hstring>(); newKey && !newKey->empty())
            {
                RegisterElementForConnectedAnimation(page, *newKey, element, nullptr);
            }
        });
    }

    void Connected::OnAnchorElementChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        const auto element = d.try_as<winrt::FrameworkElement>();

        if (!element)
        {
            return;
        }

        GetParentFrameAndExecuteAction(element, [element, e](winrt::Frame const& frame)
        {
            const auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            GetConnectedAnimationHelper(frame);

            if (const auto oldAnchor = e.OldValue().try_as<winrt::UIElement>())
            {
                RemoveAnchoredElementForConnectedAnimation(page, element, oldAnchor);
            }

            if (const auto newAnchor = e.NewValue().try_as<winrt::UIElement>())
            {
                AttachAnchorElementForConnectedAnimation(page, element, newAnchor);
            }
        });
    }

    void Connected::OnListItemKeyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        const auto listViewBase = d.try_as<winrt::ListViewBase>();

        if (!listViewBase)
        {
            return;
        }

        GetParentFrameAndExecuteAction(listViewBase, [listViewBase, e](winrt::Frame const& frame)
        {
            const auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            GetConnectedAnimationHelper(frame);

            const auto oldKey = winrt::unbox_value_or<winrt::hstring>(e.OldValue(), L"");
            if (!oldKey.empty())
            {
                UnregisterListItemForConnectedAnimation(page, listViewBase, oldKey);
            }

            AddListViewBaseItemAnimationDetails(page, listViewBase);
        });
    }

    void Connected::OnListItemElementNameChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        const auto listViewBase = d.try_as<winrt::ListViewBase>();

        if (!listViewBase)
        {
            return;
        }

        GetParentFrameAndExecuteAction(listViewBase, [listViewBase, e](winrt::Frame const& frame)
        {
            const auto page = frame.Content().try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            GetConnectedAnimationHelper(frame);

            const auto oldElementName = winrt::unbox_value_or<winrt::hstring>(e.OldValue(), L"");
            if (!oldElementName.empty())
            {
                const auto key = GetListItemKey(listViewBase);
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
        const auto frame = DependencyObjectEx::FindAscendant<winrt::Frame>(element);

        if (frame)
        {
            action(frame);
            return;
        }

        const auto token = std::make_shared<winrt::event_token>();
        *token = element.Loaded([token, action](winrt::IInspectable const& s, auto&&)
        {
            auto element = s.as<winrt::FrameworkElement>();
            element.Loaded(*token);

            if (const auto frame = DependencyObjectEx::FindAscendant<winrt::Frame>(element))
            {
                action(frame);
            }
        });
    }

    void Connected::AddListViewBaseItemAnimationDetails(winrt::Page const& page, winrt::ListViewBase const& listViewBase)
    {
        if (!page || !listViewBase)
        {
            return;
        }

        const auto elementName = GetListItemElementName(listViewBase);
        const auto key = GetListItemKey(listViewBase);

        if (elementName.empty() || key.empty())
        {
            return;
        }

        RegisterListItemForConnectedAnimation(page, listViewBase, key, elementName);
    }
}
