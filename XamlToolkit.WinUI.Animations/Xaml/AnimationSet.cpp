#include "pch.h"
#include "winrt_module_imports.h"
#include "AnimationSet.h"
#if __has_include("AnimationSet.g.cpp")
#include "AnimationSet.g.cpp"
#endif
#include "Abstract/Animation.h"
#include "AnimationScope.h"
#include "Interfaces/ITimeline.h"
#include "Interfaces/IAttachedTimeline.h"

namespace winrt::XamlToolkit::WinUI::Animations::implementation
{
    bool AnimationSet::IsSequential() const noexcept
    {
        return isSequential;
    }

    void AnimationSet::IsSequential(bool value) noexcept
    {
        isSequential = value;
    }

    winrt::UIElement AnimationSet::GetParent() const
    {
        if (auto parent = parentReference.get())
        {
            return parent;
        }

        throw winrt::hresult_access_denied(L"The current AnimationSet object isn't bound to a parent UIElement instance.");
    }

    bool AnimationSet::TryAppendTimelineNode(
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

    void AnimationSet::Start()
    {
        StartAsync();
    }

    void AnimationSet::Start(winrt::UIElement const& element)
    {
        StartAsync(element);
    }

    winrt::IAsyncAction AnimationSet::StartAsync()
    {
        return StartAsync(GetParent());
    }

    winrt::IAsyncAction AnimationSet::StartAsync(winrt::UIElement const& element)
    {
        Stop(element);

        auto state = std::make_shared<ExecutionContext>();

        {
            std::unique_lock lock(executionContextMutex);
            executionContexts.insert_or_assign(winrt::get_abi(element), state);
        }

        auto operation = StartAsync(element, state);

        state->operation = operation;

        return operation;
    }

    winrt::IAsyncAction AnimationSet::StartAsync(winrt::UIElement element, std::shared_ptr<ExecutionContext> state)
    {
        auto cancellation_token = co_await winrt::get_cancellation_token();
        cancellation_token.enable_propagation();
        cancellation_token.originate_on_cancel(false);

        auto strongThis = get_strong();

        auto cleanup = wil::scope_exit([=] 
        {
            std::unique_lock lock(strongThis->executionContextMutex);

            auto it = strongThis->executionContexts.find(winrt::get_abi(element));

            if (it != strongThis->executionContexts.end() && it->second == state)
            {
                strongThis->executionContexts.erase(it);
            }
        });

		Started.invoke(*this, nullptr);

        if (strongThis->IsSequential())
        {
            uint32_t count = strongThis->Size();

            for (uint32_t i = 0; i < count; ++i)
            {
                auto node = strongThis->GetAt(i);
                AnimationBuilder builder = AnimationBuilder::Create();

                if (TryAppendTimelineNode(node, builder, element))
                {
                    try
                    {
                        co_await builder.StartAsync(element);
                    }
                    catch (winrt::hresult_canceled const&) { break; }
                }
                else if (auto activity = node.try_as<IActivity>())
                {
                    try
                    {
                        co_await activity.InvokeAsync(element);
                    }
                    catch (winrt::hresult_canceled const&) { break; }
                }
                else
                {
                    throw winrt::hresult_invalid_argument(L"An animation set can only contain timeline nodes or IActivity nodes");
                }

                if (cancellation_token()) 
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
                    throw winrt::hresult_invalid_argument(L"An animation set can only contain timeline nodes or IActivity nodes");
                }
            }

            try
            {
                co_await builder.StartAsync(element);
            }
            catch (winrt::hresult_canceled const&) {}
        }

		Completed.invoke(*this, nullptr);
    }

    void AnimationSet::Stop()
    {
        Stop(GetParent());
    }

    void AnimationSet::Stop(winrt::UIElement const& element)
    {
        winrt::IAsyncAction operation{ nullptr };

        {
            std::unique_lock lock(executionContextMutex);

            const auto it = executionContexts.find(winrt::get_abi(element));

            if (it != executionContexts.end())
            {
                operation = it->second->operation;
            }
        }

        if (operation)
        {
            operation.Cancel();
        }
    }
}
