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

        static void CopyFrom(winrt::ResourceDictionary const& destination, winrt::ResourceDictionary const& source)
        {
            if (auto uri = source.Source())
            {
                destination.Source(uri);
            }
            else
            {
                // Clone theme dictionaries
                if (source.ThemeDictionaries() != nullptr)
                {
                    for (const auto& theme : source.ThemeDictionaries())
                    {
                        if (auto themedResource = theme.Value().try_as<winrt::ResourceDictionary>())
                        {
                            winrt::ResourceDictionary themeDictionary;
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
                if (source.MergedDictionaries())
                {
                    for (const auto& mergedResource : source.MergedDictionaries())
                    {
                        winrt::ResourceDictionary themeDictionary;
                        CopyFrom(themeDictionary, mergedResource);
                        destination.MergedDictionaries().Append(themeDictionary);
                    }
                }

                // Clone all contents
                for (const auto& item : source)
                {
                    destination.Insert(item.Key(), item.Value());
                }
            }
        }
    };
}
