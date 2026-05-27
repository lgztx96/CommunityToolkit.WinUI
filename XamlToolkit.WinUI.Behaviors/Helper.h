#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Behaviors
{
    /// <summary>
    /// Finds the first ancestor of the specified type in the visual tree.
    /// </summary>
    template<typename T>
    T FindAscendant(winrt::DependencyObject const& element)
    {
        if (!element)
        {
            return nullptr;
        }

        auto parent = winrt::VisualTreeHelper::GetParent(element);
        while (parent)
        {
            if (auto result = parent.try_as<T>())
            {
                return result;
            }
            parent = winrt::VisualTreeHelper::GetParent(parent);
        }

        return nullptr;
    }

    /// <summary>
    /// Finds the first descendant of the specified type in the visual tree.
    /// </summary>
    template<typename T>
    T FindDescendant(winrt::DependencyObject const& element)
    {
        if (!element)
        {
            return nullptr;
        }

        int32_t childrenCount = winrt::VisualTreeHelper::GetChildrenCount(element);
        for (int32_t i = 0; i < childrenCount; i++)
        {
            auto child = winrt::VisualTreeHelper::GetChild(element, i);
            if (auto result = child.try_as<T>())
            {
                return result;
            }

            if (auto descendant = FindDescendant<T>(child))
            {
                return descendant;
            }
        }

        return nullptr;
    }
}
