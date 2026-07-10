// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Composition.h>
#include <map>
#include <memory>
#include <mutex>
#include <functional>
#else
import std;
import winrt.Microsoft.UI.Composition;
#endif

namespace winrt
{
	using namespace winrt::Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::WinUI::Media::Helpers
{
    /// <summary>
    /// A class used to cache reusable CompositionObject instances in each UI thread.
    /// Pure C++ template class - no IDL exposure.
    /// </summary>
    template<typename T>
    class CompositionObjectCache
    {
    public:
        /// <summary>
        /// Tries to retrieve a valid T instance from the cache, and uses the provided factory if an existing item is not found.
        /// </summary>
        /// <param name="compositor">The current Compositor instance to get the value for.</param>
        /// <param name="producer">A function used to produce a T instance.</param>
        /// <returns>A T instance that is linked to compositor.</returns>
        T GetValue(winrt::Compositor const& compositor, std::function<T(winrt::Compositor)> const& producer)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            // Try to find existing entry
            if (auto it = _cache.find(compositor); it != _cache.end())
            {
                auto strongRef = it->second.get();
                if (strongRef)
                {
                    return strongRef;
                }
                // Weak reference expired, remove entry
                _cache.erase(it);
            }

            // Create new instance
            auto instance = producer(compositor);
            _cache.emplace(compositor, instance);

            return instance;
        }

        /// <summary>
        /// Clears all cached entries.
        /// </summary>
        void Clear()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _cache.clear();
        }

    private:
        std::mutex _mutex;
        std::map<winrt::Compositor, winrt::weak_ref<T>> _cache;
    };

    /// <summary>
    /// A class used to cache reusable CompositionObject instances with an associated key.
    /// Pure C++ template class - no IDL exposure.
    /// </summary>
    template<typename TKey, typename TValue>
    class CompositionObjectCacheWithKey
    {
    public:
        /// <summary>
        /// Tries to retrieve a valid instance from the cache.
        /// </summary>
        /// <param name="compositor">The current Compositor instance.</param>
        /// <param name="key">The key to look for.</param>
        /// <param name="result">The resulting value, if existing.</param>
        /// <returns>True if the target value has been found, false otherwise.</returns>
        bool TryGetValue(winrt::Compositor const& compositor, TKey const& key, TValue& result)
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (auto compositorIt = _cache.find(compositor); compositorIt != _cache.end())
            {
                auto& innerMap = compositorIt->second;
                auto keyIt = innerMap.find(key);
                if (keyIt != innerMap.end())
                {
                    auto strongRef = keyIt->second.get();
                    if (strongRef)
                    {
                        result = strongRef;
                        return true;
                    }
                    // Weak reference expired
                    innerMap.erase(keyIt);
                }
            }

            return false;
        }

        /// <summary>
        /// Adds or updates a value with the specified key to the cache.
        /// </summary>
        /// <param name="compositor">The current Compositor instance.</param>
        /// <param name="key">The key of the item to add.</param>
        /// <param name="value">The value to add.</param>
        void AddOrUpdate(winrt::Compositor const& compositor, TKey const& key, TValue const& value)
        {
            std::lock_guard lock(_mutex);

            auto [compositorIt, _] = _cache.try_emplace(compositor);

            compositorIt->second.insert_or_assign(key, value);
        }

        /// <summary>
        /// Clears all cached entries.
        /// </summary>
        void Clear()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _cache.clear();
        }

    private:
        std::mutex _mutex;
        std::map<winrt::Compositor, std::map<TKey, winrt::weak_ref<TValue>>> _cache;
    };
}