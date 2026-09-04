#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.System.h>
#include <memory>
#endif
#include "RangeSelector.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RangeSelector::Debounce()
    {
        if (_keyDebounceTimer.IsRunning())
        {
            _keyDebounceTimer.Stop();
        }

        _keyDebounceTimer.IsRepeating(false);
        _keyDebounceTimer.Interval(TimeToHideToolTipOnKeyUp);

        auto timerTickToken = std::make_shared<winrt::event_token>();
        *timerTickToken = _keyDebounceTimer.Tick({ get_weak(), [this, timerTickToken](const auto& sender, auto&)
        {
            if (auto timer = sender.template try_as<winrt::DispatcherQueueTimer>())
            {
                timer.Tick(*timerTickToken);
                timer.Stop();
                _toolTip.IsOpen(false);
            }
        }});

        _keyDebounceTimer.Start();
    }

    double RangeSelector::GetKeyDelta(winrt::Windows::System::VirtualKey key) const
    {
        using enum winrt::Windows::System::VirtualKey;

        const bool isRtl = FlowDirection() == winrt::FlowDirection::RightToLeft;
        const double step = StepFrequency();

        switch (key)
        {
        case Left:
            return isRtl ? +step : -step;
        case Right:
            return isRtl ? -step : +step;
        case Down:
            return -step;
        case Up:
            return +step;
        default:
            return 0;
        }
    }

    void RangeSelector::MinThumb_KeyDown([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
    {
        double delta = GetKeyDelta(e.Key());
        if (delta == 0)
            return;

        RangeStart(RangeStart() + delta);
        SyncThumbs(true);

        if (_toolTip)
        {
            _toolTip.IsOpen(false);
        }

        e.Handled(true);
    }

    void RangeSelector::MaxThumb_KeyDown([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
    {
        double delta = GetKeyDelta(e.Key());
        if (delta == 0)
            return;

        RangeEnd(RangeEnd() + delta);
        SyncThumbs(true);

        if (_toolTip)
        {
            _toolTip.IsOpen(false);
        }

        e.Handled(true);
    }

    void RangeSelector::Thumb_KeyUp([[maybe_unused]] winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
    {
        using enum winrt::Windows::System::VirtualKey;

        switch (e.Key())
        {
            case Left:
            case Right:
            case Up:
            case Down:
            if (_toolTip)
            {
                Debounce();
            }

            e.Handled(true);
            break;
        }
    }
}
