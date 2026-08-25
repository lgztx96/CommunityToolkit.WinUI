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
    using namespace ::winrt::Windows::Foundation;
    using namespace ::winrt::Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    /// <summary>
    /// A collection of animations that can be grouped together.
    /// </summary>
    struct AnimationSet : AnimationSetT<AnimationSet, winrt::cloaked<winrt::XamlToolkit::WinUI::Animations::IAnimationSetOwner>>
    {
        struct ExecutionContext { winrt::IAsyncAction operation{ nullptr }; };

        bool IsSequential() const noexcept;
        void IsSequential(bool value) noexcept;

        winrt::UIElement ParentReference() const noexcept { return parentReference.get(); }
        void ParentReference(winrt::weak_ref<winrt::UIElement> const& value) noexcept { parentReference = value; }

        wil::untyped_event<winrt::IInspectable> Started;

		wil::untyped_event<winrt::IInspectable> Completed;

        winrt::fire_and_forget Start();
        winrt::fire_and_forget Start(winrt::UIElement const& element);

        winrt::IAsyncAction StartAsync();
        winrt::IAsyncAction StartAsync(winrt::UIElement const& element);
        winrt::IAsyncAction StartAsync(winrt::UIElement element, std::shared_ptr<ExecutionContext> token);

        void Stop();
        void Stop(winrt::UIElement const& element);

    private:
        winrt::UIElement GetParent() const;

        bool TryAppendTimelineNode(
            winrt::IInspectable const& node,
            winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
            winrt::UIElement const& element);

        std::unordered_map<void*, std::shared_ptr<ExecutionContext>> executionContexts;
        std::mutex executionContextMutex;
        winrt::weak_ref<winrt::UIElement> parentReference{ nullptr };
        bool isSequential{ false };
    };
}

namespace winrt::XamlToolkit::WinUI::Animations::factory_implementation
{
    struct AnimationSet : AnimationSetT<AnimationSet, implementation::AnimationSet>
    {
    };
}
