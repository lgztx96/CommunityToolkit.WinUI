#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <memory>
#endif
#include "ConnectedAnimationHelper.h"
#include "Connected.h"

namespace winrt::XamlToolkit::WinUI::Animations
{
  
    namespace
    {
        bool IsNullOrEmptyStringParameter(winrt::IInspectable const& value)
        {
            if (!value)
            {
                return true;
            }

            auto text = winrt::unbox_value_or<winrt::hstring>(value, L"");
            return text.empty();
        }

        uintptr_t GetObjectKey(winrt::IInspectable const& value)
        {
            return reinterpret_cast<uintptr_t>(winrt::get_abi(value));
        }
    }

    ConnectedAnimationHelper::ConnectedAnimationHelper(winrt::Frame const& frame)
    {
        if (!frame)
        {
            throw winrt::hresult_invalid_argument(L"frame cannot be null");
        }

        this->frame = frame;
        navigatingToken = this->frame.Navigating({ this, &ConnectedAnimationHelper::Frame_Navigating });
        navigatedToken = this->frame.Navigated({ this, &ConnectedAnimationHelper::Frame_Navigated });
    }

    void ConnectedAnimationHelper::SetParameterForNextFrameNavigation(winrt::IInspectable const& parameter)
    {
        nextParameter = parameter;
    }

    void ConnectedAnimationHelper::Frame_Navigating(
        [[maybe_unused]] winrt::IInspectable const& sender,
        winrt::NavigatingCancelEventArgs const& e)
    {
        winrt::IInspectable parameter;

        if (nextParameter)
        {
            parameter = nextParameter;
        }
        else if (!IsNullOrEmptyStringParameter(e.Parameter()))
        {
            parameter = e.Parameter();
        }

        auto cas = winrt::ConnectedAnimationService::GetForCurrentView();
        auto page = frame.Content().try_as<winrt::Page>();

        if (!page)
        {
            return;
        }

        auto& connectedProps = implementation::Connected::GetPageConnectedAnimationProperties(page);

        for (const auto& [key, props] : connectedProps)
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

                previousPageConnectedAnimationProps[props.Key] = props;
            }
        }
    }

    void ConnectedAnimationHelper::Frame_Navigated(
        [[maybe_unused]] winrt::IInspectable const& sender,
        winrt::NavigationEventArgs const& e)
    {
        auto navigatedPage = frame.Content().try_as<winrt::Page>();

        if (!navigatedPage)
        {
            return;
        }

        auto weakFrame = winrt::make_weak(frame);
        auto token = std::make_shared<winrt::event_token>();

        *token = navigatedPage.Loaded([this, weakFrame, token, e](winrt::IInspectable const& source, winrt::RoutedEventArgs const&)
        {
            auto page = source.try_as<winrt::Page>();

            if (!page)
            {
                return;
            }

            page.Loaded(*token);

            winrt::IInspectable parameter{ nullptr };

            if (nextParameter)
            {
                parameter = nextParameter;
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

            auto cas = winrt::ConnectedAnimationService::GetForCurrentView();
            auto& connectedProps = implementation::Connected::GetPageConnectedAnimationProperties(page);
            auto& coordinated = implementation::Connected::GetPageCoordinatedAnimationElements(page);

            for (const auto& [_, props] : connectedProps)
            {
                auto connectedAnimation = cas.GetAnimation(props.Key);
                bool animationHandled = false;

                if (connectedAnimation)
                {
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
                                listAnimProperty.ListViewBase.ScrollIntoView(parameter);
                                [[maybe_unused]] auto operation =
                                    listAnimProperty.ListViewBase.TryStartConnectedAnimationAsync(
                                        connectedAnimation,
                                        parameter,
                                        listAnimProperty.ElementName);

                                animationHandled = true;
                            }
                            catch (...)
                            {
                                connectedAnimation.Cancel();
                            }
                        }
                    }
                    else if (!props.IsListAnimation() && props.Element)
                    {
                        auto it = coordinated.find(GetObjectKey(props.Element));

                        if (it != coordinated.end() && !it->second.empty())
                        {
                            auto list = winrt::single_threaded_vector<winrt::UIElement>();
                            for (const auto& value : it->second)
                            {
                                list.Append(value);
                            }

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
                    previousPageConnectedAnimationProps.erase(props.Key);
                }
            }

            for (const auto& [key, _] : previousPageConnectedAnimationProps)
            {
                if (auto animation = cas.GetAnimation(key))
                {
                    animation.Cancel();
                }
            }

            previousPageConnectedAnimationProps.clear();
            nextParameter = nullptr;
        });
    }

    void ConnectedAnimationHelper::UseDirectConnectedAnimationConfiguration(winrt::ConnectedAnimation const& animation)
    {
        animation.Configuration(winrt::DirectConnectedAnimationConfiguration{});
    }
}
