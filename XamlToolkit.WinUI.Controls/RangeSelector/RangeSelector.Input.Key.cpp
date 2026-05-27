#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.System.h>
#include <memory>
#endif
#include "RangeSelector.h"

namespace winrt
{
    using namespace Windows::System;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RangeSelector::Debounce()
    {
        if (keyDebounceTimer.IsRunning())
        {
            keyDebounceTimer.Stop();
        }

        keyDebounceTimer.IsRepeating(false);
        keyDebounceTimer.Interval(TimeToHideToolTipOnKeyUp);

        auto timerTickToken = std::make_shared<winrt::event_token>();
        *timerTickToken = keyDebounceTimer.Tick({ get_weak(), [this, timerTickToken](auto const& sender, auto&)
        {
            if (auto timer = sender.template try_as<Microsoft::UI::Dispatching::DispatcherQueueTimer>())
            {
                timer.Tick(*timerTickToken);
                timer.Stop();
                _toolTip.IsOpen(false);
            }
        }});

        keyDebounceTimer.Start();
    }

    double RangeSelector::GetKeyDelta(winrt::Windows::System::VirtualKey key) const
    {
        const bool isRtl = FlowDirection() == FlowDirection::RightToLeft;
        const double step = StepFrequency();

        switch (key)
        {
        case VirtualKey::Left:
            return isRtl ? +step : -step;

        case VirtualKey::Right:
            return isRtl ? -step : +step;

        case VirtualKey::Down:
            return -step;

        case VirtualKey::Up:
            return +step;

        default:
            return 0;
        }
    }

    void RangeSelector::MinThumb_KeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        double delta = GetKeyDelta(e.Key());
        if (delta == 0)
            return;

        RangeStart(RangeStart() + delta);
        SyncThumbs(true);

        if (_toolTip != nullptr)
        {
            _toolTip.IsOpen(false);
        }

        e.Handled(true);
    }

    void RangeSelector::MaxThumb_KeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        double delta = GetKeyDelta(e.Key());
        if (delta == 0)
            return;

        RangeEnd(RangeEnd() + delta);
        SyncThumbs(true);

        if (_toolTip != nullptr)
        {
            _toolTip.IsOpen(false);
        }

        e.Handled(true);
    }

    void RangeSelector::Thumb_KeyUp([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        switch (e.Key())
        {
        case VirtualKey::Left:
        case VirtualKey::Right:
        case VirtualKey::Up:
        case VirtualKey::Down:
            if (_toolTip != nullptr)
            {
                Debounce();
            }

            e.Handled(true);
            break;
        }
    }
}
