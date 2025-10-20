#include "pch.h"
#include "StyleExtensions.h"
#if __has_include("StyleExtensions.g.cpp")
#include "StyleExtensions.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	ResourceDictionary StyleExtensions::GetResources(Microsoft::UI::Xaml::Style obj)
	{
		return winrt::unbox_value<ResourceDictionary>(obj.GetValue(ResourcesProperty));
	}

	void StyleExtensions::SetResources(Microsoft::UI::Xaml::Style obj, ResourceDictionary value)
	{
		obj.SetValue(ResourcesProperty, winrt::box_value(value));
	}

	void StyleExtensions::ResourcesChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
	{
		auto frameworkElement = sender.try_as<FrameworkElement>();
		if (!frameworkElement)
		{
			return;
		}

		auto mergedDictionaries = frameworkElement.Resources().MergedDictionaries();
		if (mergedDictionaries == nullptr)
		{
			return;
		}

		for (uint32_t index = 0; index < mergedDictionaries.Size(); ++index)
		{
			if (auto&& value = mergedDictionaries.GetAt(index);
				value.try_as<CommunityToolkit::WinUI::Controls::StyleExtensionResourceDictionary>())
			{
				// Remove the existing resource dictionary
				mergedDictionaries.RemoveAt(index);
				break;
			}
		}

		if (auto resource = e.NewValue().try_as<ResourceDictionary>())
		{
			auto clonedResources = winrt::make<CommunityToolkit::WinUI::Controls::implementation::StyleExtensionResourceDictionary>();
			ResourceDictionaryExtensions::CopyFrom(clonedResources, resource);
			mergedDictionaries.Append(clonedResources);
		}

		if (frameworkElement.IsLoaded())
		{
			// Only force if the style was applied after the control was loaded
			ForceControlToReloadThemeResources(frameworkElement);
		}
	}

	void StyleExtensions::ForceControlToReloadThemeResources(FrameworkElement frameworkElement)
	{
		// To force the refresh of all resource references.
		// Note: Doesn't work when in high-contrast.
		auto currentRequestedTheme = frameworkElement.RequestedTheme();
		frameworkElement.RequestedTheme(currentRequestedTheme == ElementTheme::Dark
			? ElementTheme::Light
			: ElementTheme::Dark);
		frameworkElement.RequestedTheme(currentRequestedTheme);
	}
}
