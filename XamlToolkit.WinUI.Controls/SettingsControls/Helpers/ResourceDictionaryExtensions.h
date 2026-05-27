#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	struct ResourceDictionaryExtensions
	{
		ResourceDictionaryExtensions() = default;

		static void CopyFrom(ResourceDictionary destination, ResourceDictionary source)
		{
			if (source.Source() != nullptr)
			{
				destination.Source(source.Source());
			}
			else
			{
				// Clone theme dictionaries
				if (source.ThemeDictionaries() != nullptr)
				{
					for (auto theme : source.ThemeDictionaries())
					{
						if (auto themedResource = theme.Value().try_as<ResourceDictionary>())
						{
							auto themeDictionary = ResourceDictionary();
							CopyFrom(themeDictionary, themedResource);
							destination.ThemeDictionaries().Insert(theme.Key(), themeDictionary);
						}
						else
						{
							destination.ThemeDictionaries().Insert(theme.Key(), theme.Value());
						}
					}
				}

				// Clone merged dictionaries
				if (source.MergedDictionaries() != nullptr)
				{
					for (auto mergedResource : source.MergedDictionaries())
					{
						auto themeDictionary = ResourceDictionary();
						CopyFrom(themeDictionary, mergedResource);
						destination.MergedDictionaries().Append(themeDictionary);
					}
				}

				// Clone all contents
				for (auto item : source)
				{
					destination.Insert(item.Key(), item.Value());
				}
			}
		}
	};
}
