#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <atomic>
#include <memory>
#include <mutex>
#endif
#include "AnimationSet.h"
#if __has_include("AnimationSet.g.cpp")
#include "AnimationSet.g.cpp"
#endif
#include "Abstract/Animation.h"
#include "AnimationScope.h"
#include "Interfaces/ITimeline.h"
#include "Interfaces/IAttachedTimeline.h"

namespace
{
    bool TryAppendTimelineNode(
        winrt::IInspectable const& node,
        winrt::XamlToolkit::WinUI::Animations::AnimationBuilder& builder,
        winrt::UIElement const& element)
    {
        if (auto animation = node.try_as<winrt::XamlToolkit::WinUI::Animations::Animation>())
        {
            if (auto attachedTimeline = animation.try_as<winrt::XamlToolkit::WinUI::Animations::implementation::IAttachedTimeline>())
            {
                attachedTimeline->AppendToBuilder(builder, element);
                return true;
            }
            else if (auto timeline = animation.try_as<winrt::XamlToolkit::WinUI::Animations::implementation::ITimeline>())
            {
                timeline->AppendToBuilder(builder);
                return true;
            }
        }

        if (auto scope = node.try_as<winrt::XamlToolkit::WinUI::Animations::AnimationScope>())
        {
            auto impl = winrt::get_self<winrt::XamlToolkit::WinUI::Animations::implementation::AnimationScope>(scope);
            impl->AppendToBuilder(builder);
            return true;
        }

        return false;
    }
}

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    bool AnimationSet::IsSequential() const noexcept
    {
        return isSequential;
    }

    void AnimationSet::IsSequential(bool value)
    {
        isSequential = value;
    }

    winrt::fire_and_forget AnimationSet::Start()
    {
        co_await StartAsync();
    }

    winrt::fire_and_forget AnimationSet::Start(winrt::UIElement const& element)
    {
        co_await StartAsync(element);
    }

    winrt::IAsyncAction AnimationSet::StartAsync()
    {
        return StartAsync(GetParent());
    }

    winrt::IAsyncAction AnimationSet::StartAsync(winrt::UIElement const& element)
    {
        Stop(element);

        auto cancellationState = std::make_shared<std::atomic<bool>>(false);

        {
            std::scoped_lock lock(cancellationStateMapMutex);
            cancellationStateMap[reinterpret_cast<uintptr_t>(winrt::get_abi(element))] = cancellationState;
        }

        return StartAsync(element, cancellationState);
    }

    winrt::IAsyncAction AnimationSet::StartAsync(winrt::UIElement element, std::shared_ptr<std::atomic<bool>> cancellationState)
    {
        auto strongThis = get_strong();

		Started.invoke(*this, nullptr);

        if (strongThis->IsSequential())
        {
            uint32_t count = strongThis->Size();

            for (uint32_t i = 0; i < count; ++i)
            {
                if (cancellationState->load(std::memory_order_acquire))
                {
                    break;
                }

                auto node = strongThis->GetAt(i);
                AnimationBuilder builder = AnimationBuilder::Create();

                if (TryAppendTimelineNode(node, builder, element))
                {
                    co_await builder.StartAsync(element);
                }
                else if (auto activity = node.try_as<IActivity>())
                {
                    try
                    {
                        co_await activity.InvokeAsync(element);
                    }
                    catch (winrt::hresult_canceled)
                    {
                        break;
                    }
                }
                else
                {
                    ThrowArgumentException();
                }

                if (cancellationState->load(std::memory_order_acquire))
                {
                    break;
                }
            }
        }
        else
        {
            AnimationBuilder builder = AnimationBuilder::Create();
            uint32_t count = strongThis->Size();

            for (uint32_t i = 0; i < count; ++i)
            {
                auto node = strongThis->GetAt(i);

                if (TryAppendTimelineNode(node, builder, element))
                {
                    continue;
                }

                if (auto activity = node.try_as<IActivity>())
                {
                    activity.InvokeAsync(element);
                }
                else
                {
                    ThrowArgumentException();
                }
            }

            co_await builder.StartAsync(element);
        }

		Completed.invoke(*this, nullptr);

        {
            std::scoped_lock lock(strongThis->cancellationStateMapMutex);
            auto it = strongThis->cancellationStateMap.find(reinterpret_cast<uintptr_t>(winrt::get_abi(element)));
            if (it != strongThis->cancellationStateMap.end() && it->second == cancellationState)
            {
                strongThis->cancellationStateMap.erase(it);
            }
        }
    }

    void AnimationSet::Stop()
    {
        Stop(GetParent());
    }

    void AnimationSet::Stop(winrt::UIElement const& element)
    {
        std::shared_ptr<std::atomic<bool>> cancellationState;

        {
            std::scoped_lock lock(cancellationStateMapMutex);
            auto it = cancellationStateMap.find(reinterpret_cast<uintptr_t>(winrt::get_abi(element)));
            if (it != cancellationStateMap.end())
            {
                cancellationState = it->second;
            }
        }

        if (cancellationState)
        {
            cancellationState->store(true, std::memory_order_release);
        }
    }
}
