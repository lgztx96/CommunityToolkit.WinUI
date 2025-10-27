#include "pch.h"
#include "RangeSelector.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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
                _toolTip.Visibility(Visibility::Collapsed);
            }
        }});

        keyDebounceTimer.Start();
    }

    void RangeSelector::MinThumb_KeyDown([[maybe_unused]] [[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        switch (e.Key())
        {
        case VirtualKey::Left:
            RangeStart(RangeStart() - StepFrequency());
            SyncThumbs(true);
            if (_toolTip != nullptr)
            {
                _toolTip.Visibility(Visibility::Visible);
            }

            e.Handled(true);
            break;
        case VirtualKey::Right:
            RangeStart(RangeStart() + StepFrequency());
            SyncThumbs(true);
            if (_toolTip != nullptr)
            {
                _toolTip.Visibility(Visibility::Visible);
            }

            e.Handled(true);
            break;
        }
    }

    void RangeSelector::MaxThumb_KeyDown([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        switch (e.Key())
        {
        case VirtualKey::Left:
            RangeEnd(RangeEnd() - StepFrequency());
            SyncThumbs(true);
            if (_toolTip != nullptr)
            {
                _toolTip.Visibility(Visibility::Visible);
            }

            e.Handled(true);
            break;
        case VirtualKey::Right:
            RangeEnd(RangeEnd() + StepFrequency());
            SyncThumbs(true);
            if (_toolTip != nullptr)
            {
                _toolTip.Visibility(Visibility::Visible);
            }

            e.Handled(true);
            break;
        }
    }

    void RangeSelector::Thumb_KeyUp([[maybe_unused]] IInspectable const& sender, KeyRoutedEventArgs const& e)
    {
        switch (e.Key())
        {
        case VirtualKey::Left:
        case VirtualKey::Right:
            if (_toolTip != nullptr)
            {
                Debounce();
            }

            e.Handled(true);
            break;
        }
    }
}