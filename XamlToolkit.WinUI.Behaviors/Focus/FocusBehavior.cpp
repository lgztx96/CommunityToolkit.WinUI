#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <chrono>
#endif
#include "FocusBehavior.h"
#include "FocusTarget.h"
#include "FocusTargetList.h"
#include "../Helper.h"
#if __has_include("FocusBehavior.g.cpp")
#include "FocusBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> FocusBehavior::TargetsProperty =
        winrt::DependencyProperty::Register(
            L"Targets",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &FocusBehavior::OnTargetsPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> FocusBehavior::FocusEngagementTimeoutProperty =
        winrt::DependencyProperty::Register(
            L"FocusEngagementTimeout",
            winrt::xaml_typename<winrt::TimeSpan>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::TimeSpan{ std::chrono::milliseconds(100) })));

    FocusBehavior::FocusBehavior()
    {
        _timer = winrt::DispatcherQueue::GetForCurrentThread().CreateTimer();
        _timer.Tick({ this, &FocusBehavior::OnEngagementTimerTick });
        auto list = winrt::make<winrt::XamlToolkit::WinUI::Behaviors::implementation::FocusTargetList>();
        Targets(list);
    }

    winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList FocusBehavior::Targets() const
    {
        return GetValue(TargetsProperty()).try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList>();
    }

    void FocusBehavior::Targets(winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList const& value)
    {
        SetValue(TargetsProperty(), value);
    }

    winrt::TimeSpan FocusBehavior::FocusEngagementTimeout() const
    {
        return winrt::unbox_value<winrt::TimeSpan>(GetValue(FocusEngagementTimeoutProperty()));
    }

    void FocusBehavior::FocusEngagementTimeout(winrt::TimeSpan const& value)
    {
        SetValue(FocusEngagementTimeoutProperty(), winrt::box_value(value));
    }

    void FocusBehavior::OnAssociatedObjectLoaded()
    {
        std::erase_if(_controlChangedTokens, [](auto& pair) 
        {
            if (const auto target = pair.first.get())
            {
                target.ControlChanged(pair.second);
            }
            return true;
        });

        const auto targets = Targets();
        for (uint32_t i = 0; i < targets.Size(); i++)
        {
            if (const auto target = targets.GetAt(i).try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusTarget>())
            {
                const auto token = target.ControlChanged({ this, &FocusBehavior::OnTargetControlChanged });
                _controlChangedTokens.emplace_back(target, token);
            }
        }

        ApplyFocus();
    }

    bool FocusBehavior::Uninitialize()
    {
        std::erase_if(_controlChangedTokens, [](auto& pair)
        {
            if (const auto target = pair.first.get())
            {
                target.ControlChanged(pair.second);
            }
            return true;
        });

        Stop();

        return true;
    }

    void FocusBehavior::OnTargetsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (const auto behavior = d.try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusBehavior>())
        {
            const auto impl = winrt::get_self<FocusBehavior>(behavior);

            if (const auto oldValue = args.OldValue())
            {
                if (const auto oldTargets = oldValue.try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusTargetList>())
                {
                    impl->Stop();
                }
            }

            impl->ApplyFocus();
        }
    }

    void FocusBehavior::ApplyFocus()
    {
        const auto targets = Targets();
        if (targets.Size() == 0)
        {
            return;
        }

        std::erase_if(_controlLoadedTokens, [](auto& pair)
        {
            if (const auto control = pair.first.get())
            {
                control.Loaded(pair.second);
            }
            return true;
        });

        std::erase_if(_containerContentChangingTokens, [](auto& pair)
        {
            if (const auto listView = pair.first.get())
            {
                listView.ContainerContentChanging(pair.second);
            }
            return true;
        });

        int32_t focusedControlIndex = -1;
        bool hasListViewBaseControl = false;
        bool allLoaded = true;

        for (uint32_t i = 0; i < targets.Size(); i++)
        {
            auto target = targets.GetAt(i).try_as<winrt::XamlToolkit::WinUI::Behaviors::FocusTarget>();
            if (!target) continue;

            auto control = target.Control();
            if (!control) { allLoaded = false; continue; }

            if (control.IsLoaded())
            {
                if (control.Focus(winrt::FocusState::Programmatic))
                {
                    focusedControlIndex = static_cast<int32_t>(i);
                    break;
                }

                if (const auto listViewBase = control.try_as<winrt::ListViewBase>())
                {
                    const auto token = listViewBase.ContainerContentChanging({ this, &FocusBehavior::OnContainerContentChanging });
                    _containerContentChangingTokens.emplace_back(listViewBase, token);
                    hasListViewBaseControl = true;
                }
            }
            else
            {
                allLoaded = false;
                const auto token = control.Loaded({ this, &FocusBehavior::OnControlLoaded });
                _controlLoadedTokens.emplace_back(control, token);
            }
        }

        if (focusedControlIndex == 0 || (!hasListViewBaseControl && allLoaded))
        {
            Stop();
        }
        else if (focusedControlIndex > 0)
        {
            // We have been able to set the focus on one control.
            // We start the timer to detect if we can focus another control with an higher priority.
            // This allows us to handle the case where the controls are not loaded in the order we expect.
            _timer.Interval(FocusEngagementTimeout());
            _timer.Start();
        }
    }

    void FocusBehavior::Stop()
    {
        if (_timer.IsRunning())
        {
            _timer.Stop();
        }

        std::erase_if(_controlLoadedTokens, [](auto& pair)
        {
            if (const auto control = pair.first.get())
            {
                control.Loaded(pair.second);
            }
            return true;
        });

        std::erase_if(_containerContentChangingTokens, [](auto& pair) 
        {
            if (const auto listView = pair.first.get())
            {
                listView.ContainerContentChanging(pair.second);
            }
            return true;
        });
    }

    void FocusBehavior::OnControlLoaded(
        [[maybe_unused]] winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        ApplyFocus();
    }

    void FocusBehavior::OnTargetControlChanged(
        [[maybe_unused]] winrt::IInspectable const& sender,
        [[maybe_unused]] winrt::IInspectable const& e)
    {
        ApplyFocus();
    }

    void FocusBehavior::OnContainerContentChanging(winrt::ListViewBase const& sender, 
        [[maybe_unused]] winrt::ContainerContentChangingEventArgs const& args)
    {
        std::erase_if(_containerContentChangingTokens, [&sender](auto& pair)
        {
            if (const auto listView = pair.first.get())
            {
                if (listView == sender)
                {
                    listView.ContainerContentChanging(pair.second);
                    return true;
                }
                return false;
            }
            return true;
        });

        ApplyFocus();
    }

    void FocusBehavior::OnEngagementTimerTick([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& e)
    {
        ApplyFocus();
        Stop();
    }
}
