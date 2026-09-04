#include "pch.h"
#include "winrt_module_imports.h"
#include "StyleExtensions.h"
#if __has_include("StyleExtensions.g.cpp")
#include "StyleExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> StyleExtensions::ResourcesProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"Resources",
            winrt::xaml_typename<winrt::ResourceDictionary>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::StyleExtensions>(),
            winrt::PropertyMetadata(nullptr, &StyleExtensions::ResourcesChanged));

    winrt::ResourceDictionary StyleExtensions::GetResources(winrt::Style const& obj)
    {
        return winrt::unbox_value<winrt::ResourceDictionary>(obj.GetValue(ResourcesProperty()));
    }

    void StyleExtensions::SetResources(winrt::Style const& obj, winrt::ResourceDictionary const& value)
    {
        obj.SetValue(ResourcesProperty(), winrt::box_value(value));
    }

    void StyleExtensions::ResourcesChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        const auto frameworkElement = sender.try_as<winrt::FrameworkElement>();
        if (!frameworkElement)
        {
            return;
        }

        const auto mergedDictionaries = frameworkElement.Resources().MergedDictionaries();
        if (!mergedDictionaries)
        {
            return;
        }

        for (uint32_t index = 0; index < mergedDictionaries.Size(); ++index)
        {
            if (const auto& value = mergedDictionaries.GetAt(index);
                value.try_as<IStyleExtensionResourceDictionary>())
            {
                // Remove the existing resource dictionary
                mergedDictionaries.RemoveAt(index);
                break;
            }
        }

        if (const auto resource = e.NewValue().try_as<winrt::ResourceDictionary>())
        {
            const auto clonedResources = winrt::make<StyleExtensionResourceDictionary>();
            ResourceDictionaryExtensions::CopyFrom(clonedResources, resource);
            mergedDictionaries.Append(clonedResources);
        }

        if (frameworkElement.IsLoaded())
        {
            // Only force if the style was applied after the control was loaded
            ForceControlToReloadThemeResources(frameworkElement);
        }
    }

    void StyleExtensions::ForceControlToReloadThemeResources(winrt::FrameworkElement const& frameworkElement)
    {
        // To force the refresh of all resource references.
        // Note: Doesn't work when in high-contrast.
        auto currentRequestedTheme = frameworkElement.RequestedTheme();

        frameworkElement.RequestedTheme(
            currentRequestedTheme == winrt::ElementTheme::Dark
            ? winrt::ElementTheme::Light
            : winrt::ElementTheme::Dark);

        frameworkElement.RequestedTheme(currentRequestedTheme);
    }
}
