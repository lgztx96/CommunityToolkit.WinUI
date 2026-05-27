#include "pch.h"
#include "winrt_module_imports.h"
#include "VisualStateUtilities.h"
#if __has_include("VisualStateUtilities.g.cpp")
#include "VisualStateUtilities.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    bool VisualStateUtilities::GoToState(winrt::Controls::Control const& control, winrt::hstring const& stateName, bool useTransitions)
    {
        if (control == nullptr)
        {
            throw winrt::hresult_invalid_argument(L"control");
        }

        if (stateName.empty())
        {
            throw winrt::hresult_invalid_argument(L"stateName");
        }

        control.ApplyTemplate();
        return winrt::VisualStateManager::GoToState(control, stateName, useTransitions);
    }

    winrt::IVector<winrt::VisualStateGroup> VisualStateUtilities::GetVisualStateGroups(winrt::FrameworkElement const& element)
    {
        if (element == nullptr)
        {
            throw winrt::hresult_invalid_argument(L"element");
        }

        auto visualStateGroups = winrt::VisualStateManager::GetVisualStateGroups(element);

        if (visualStateGroups == nullptr || visualStateGroups.Size() == 0)
        {
            int childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);
            if (childrenCount > 0)
            {
                auto childElement = winrt::VisualTreeHelper::GetChild(element, 0).try_as<winrt::FrameworkElement>();
                if (childElement != nullptr)
                {
                    visualStateGroups = winrt::VisualStateManager::GetVisualStateGroups(childElement);
                }
            }
        }

        return visualStateGroups;
    }

    winrt::Controls::Control VisualStateUtilities::FindNearestStatefulControl(winrt::FrameworkElement const& element)
    {
        if (element == nullptr)
        {
            throw winrt::hresult_invalid_argument(L"element");
        }

        auto currentElement = element;
        auto parent = currentElement.Parent().try_as<winrt::FrameworkElement>();

        while (!VisualStateUtilities::HasVisualStateGroupsDefined(currentElement) && VisualStateUtilities::ShouldContinueTreeWalk(parent))
        {
            currentElement = parent;
            parent = currentElement.Parent().try_as<winrt::FrameworkElement>();
        }

        if (VisualStateUtilities::HasVisualStateGroupsDefined(currentElement))
        {
            auto templatedParent = winrt::VisualTreeHelper::GetParent(currentElement).try_as<winrt::Controls::Control>();
            if (templatedParent != nullptr)
            {
                return templatedParent;
            }

            return currentElement.try_as<winrt::Controls::Control>();
        }

        return nullptr;
    }

    bool VisualStateUtilities::HasVisualStateGroupsDefined(winrt::FrameworkElement const& element)
    {
        return element != nullptr && winrt::VisualStateManager::GetVisualStateGroups(element).Size() != 0;
    }

    bool VisualStateUtilities::ShouldContinueTreeWalk(winrt::FrameworkElement const& element)
    {
        if (element == nullptr)
        {
            return false;
        }

        if (element.try_as<winrt::Controls::UserControl>() != nullptr)
        {
            return false;
        }

        if (element.Parent() == nullptr)
        {
            auto templatedParent = winrt::VisualTreeHelper::GetParent(element).try_as<winrt::FrameworkElement>();
            if (templatedParent == nullptr || (templatedParent.try_as<winrt::Controls::Control>() == nullptr && templatedParent.try_as<winrt::Controls::ContentPresenter>() == nullptr))
            {
                return false;
            }
        }

        return true;
    }
}
