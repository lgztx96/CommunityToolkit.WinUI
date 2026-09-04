#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <memory>
#include <winrt/Microsoft.UI.Dispatching.h>
#else
import winrt.Microsoft.UI.Dispatching;
#endif
#include "ConnectedAnimationHelper.h"
#include "Connected.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
    ConnectedAnimationHelper::ConnectedAnimationHelper(winrt::Frame const& frame)
    {
        if (!frame)
        {
            throw winrt::hresult_invalid_argument(L"frame cannot be null");
        }

        _navigatingRevoker = frame.Navigating(winrt::auto_revoke, { get_weak(), &ConnectedAnimationHelper::Frame_Navigating});
        _navigatedRevoker = frame.Navigated(winrt::auto_revoke, { get_weak(), &ConnectedAnimationHelper::Frame_Navigated });
    }

    void ConnectedAnimationHelper::SetParameterForNextFrameNavigation(winrt::IInspectable const& parameter)
    {
        _nextParameter = parameter;
    }

    void ConnectedAnimationHelper::Frame_Navigating(
        winrt::IInspectable const& sender,
        winrt::NavigatingCancelEventArgs const& e)
    {
        const auto frame = sender.as<winrt::Frame>();
        winrt::IInspectable parameter{ nullptr };

        if (_nextParameter)
        {
            parameter = _nextParameter;
        }
        else if (auto value = e.Parameter())
        {
            if (const auto text = value.try_as<winrt::hstring>(); text && !text->empty())
            {
                parameter = value;
            }
        }

        const auto cas = winrt::ConnectedAnimationService::GetForCurrentView();
        const auto page = frame.Content().try_as<winrt::Page>();

        if (!page)
        {
            return;
        }

        const auto connectedAnimationsProps = implementation::Connected::GetPageConnectedAnimationProperties(page);

        for (const auto& [key, props] : *connectedAnimationsProps)
        {
            winrt::ConnectedAnimation animation{ nullptr };

            if (props.IsListAnimation() && parameter)
            {
                for (const auto& listAnimProperty : props.ListAnimProperties)
                {
                    if (!listAnimProperty.ListViewBase || listAnimProperty.ElementName.empty())
                    {
                        continue;
                    }

                    try
                    {
                        animation = listAnimProperty.ListViewBase.PrepareConnectedAnimation(props.Key, parameter, listAnimProperty.ElementName);
                    }
                    catch (...)
                    {
                        animation = nullptr;
                    }

                    if (animation)
                    {
                        break;
                    }
                }
            }
            else if (!props.IsListAnimation() && props.Element)
            {
                animation = cas.PrepareToAnimate(props.Key, props.Element);
            }
            else
            {
                continue;
            }

            if (animation)
            {
                if (e.NavigationMode() == winrt::NavigationMode::Back)
                {
                    UseDirectConnectedAnimationConfiguration(animation);
                }

                _previousPageConnectedAnimationProps[props.Key] = props;
            }
        }
    }

    void ConnectedAnimationHelper::Frame_Navigated(
        winrt::IInspectable const& sender,
        winrt::NavigationEventArgs const& e)
    {
        const auto frame = sender.as<winrt::Frame>();
        const auto navigatedPage = frame.Content().try_as<winrt::Page>();

        if (!navigatedPage)
        {
            return;
        }

        const auto weakFrame = winrt::make_weak(frame);
        const auto token = std::make_shared<winrt::event_token>();

        *token = navigatedPage.Loaded([this, weakFrame, token, e](winrt::IInspectable const& source, winrt::RoutedEventArgs const&)
        {
            const auto page = source.try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            page.Loaded(*token);

            winrt::IInspectable parameter{ nullptr };

            if (_nextParameter)
            {
                parameter = _nextParameter;
            }
            else if (e.NavigationMode() == winrt::NavigationMode::Back)
            {
                if (auto strongFrame = weakFrame.get())
                {
                    auto forwardStack = strongFrame.ForwardStack();
                    if (forwardStack.Size() > 0)
                    {
                        parameter = forwardStack.GetAt(forwardStack.Size() - 1).Parameter();
                    }
                }
            }
            else
            {
                parameter = e.Parameter();
            }

            const auto cas = winrt::ConnectedAnimationService::GetForCurrentView();

            const auto connectedAnimationsProps = implementation::Connected::GetPageConnectedAnimationProperties(page);
            const auto coordinatedAnimationElements = implementation::Connected::GetPageCoordinatedAnimationElements(page);

            for (const auto& [_, props] : *connectedAnimationsProps)
            {
                const auto connectedAnimation = cas.GetAnimation(props.Key);
                bool animationHandled = false;

                if (connectedAnimation)
                {
                    if (props.IsListAnimation() && parameter)
                    {
                        for (const auto& listAnimProperty : props.ListAnimProperties)
                        {
                            auto itemsSource = listAnimProperty.ListViewBase.ItemsSource().try_as<winrt::IVector<winrt::IInspectable>>();
                            uint32_t index;
                            if (itemsSource && itemsSource.IndexOf(parameter, index))
                            {
                                listAnimProperty.ListViewBase.ScrollIntoView(parameter);

                                // give time to the UI thread to scroll the list
                                const auto dispatcherQueue = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
                                dispatcherQueue.TryEnqueue([=]() -> winrt::fire_and_forget
                                {
                                    try
                                    {
                                        co_await listAnimProperty.ListViewBase.TryStartConnectedAnimationAsync(
                                                connectedAnimation,
                                                parameter,
                                                listAnimProperty.ElementName);
                                    }
                                    catch (...)
                                    {
                                        connectedAnimation.Cancel();
                                    }
                                });

                                animationHandled = true;
                            }
                        }
                    }
                    else if (!props.IsListAnimation())
                    {
                        const auto it = coordinatedAnimationElements->find(props.Element);

                        if (it != coordinatedAnimationElements->end() && !it->second.empty())
                        {
                            auto list = winrt::single_threaded_vector<winrt::UIElement>();
                            list.ReplaceAll(it->second);
                            connectedAnimation.TryStart(props.Element, list);
                        }
                        else
                        {
                            connectedAnimation.TryStart(props.Element);
                        }

                        animationHandled = true;
                    }
                }

                if (animationHandled)
                {
                    _previousPageConnectedAnimationProps.erase(props.Key);
                }
            }

            // if there are animations that were prepared on previous page but no elements on this page have the same key - cancel
            for (const auto& [key, _] : _previousPageConnectedAnimationProps)
            {
                if (const auto animation = cas.GetAnimation(key))
                {
                    animation.Cancel();
                }
            }

            _previousPageConnectedAnimationProps.clear();
            _nextParameter = nullptr;
        });
    }

    void ConnectedAnimationHelper::UseDirectConnectedAnimationConfiguration(winrt::ConnectedAnimation const& animation)
    {
        animation.Configuration(winrt::DirectConnectedAnimationConfiguration{});
    }
}
