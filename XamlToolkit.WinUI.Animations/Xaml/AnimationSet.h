// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "AnimationSet.g.h"
#include "../Builders/AnimationBuilder.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A collection of animations that can be grouped together.
    /// </summary>
    struct AnimationSet : AnimationSetT<AnimationSet, winrt::cloaked<winrt::XamlToolkit::WinUI::Animations::IAnimationSetOwner>>
    {
    private:
        std::unordered_map<uintptr_t, std::shared_ptr<std::atomic<bool>>> cancellationStateMap;
        std::mutex cancellationStateMapMutex;
        winrt::weak_ref<winrt::UIElement> parentReference{ nullptr };
        bool isSequential{ false };

        winrt::UIElement GetParent() const
        {
            if (auto parent = parentReference.get())
            {
                return parent;
            }

            throw winrt::hresult_access_denied(L"The current AnimationSet object isn't bound to a parent UIElement instance.");
        }

        static void ThrowArgumentException()
        {
            throw winrt::hresult_invalid_argument(L"An animation set can only contain timeline nodes or IActivity nodes");
        }

        winrt::IAsyncAction StartAsync(winrt::UIElement element, std::shared_ptr<std::atomic<bool>> cancellationState);

    public:
        bool IsSequential() const noexcept;
        void IsSequential(bool value) noexcept;

        winrt::weak_ref<winrt::UIElement> ParentReference() { return parentReference; }
        void ParentReference(winrt::weak_ref<winrt::UIElement> const& value) { parentReference = value; }

        winrt::UIElement Parent() const noexcept
        {
            if (auto strongParent = parentReference.get())
            {
                return strongParent;
            }

            return nullptr;
        }

        wil::untyped_event<winrt::IInspectable> Started;

		wil::untyped_event<winrt::IInspectable> Completed;

        winrt::fire_and_forget Start();
        winrt::fire_and_forget Start(winrt::UIElement const& element);

        winrt::IAsyncAction StartAsync();
        winrt::IAsyncAction StartAsync(winrt::UIElement const& element);

        void Stop();
        void Stop(winrt::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationSet : AnimationSetT<AnimationSet, implementation::AnimationSet>
    {
    };
}
