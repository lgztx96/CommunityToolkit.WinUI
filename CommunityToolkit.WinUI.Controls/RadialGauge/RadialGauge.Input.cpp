#include "pch.h"
#include "RadialGauge.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    void RadialGauge::RadialGauge_ManipulationDelta([[maybe_unused]] IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e)
    {
        SetGaugeValueFromPoint(e.Position());
    }

    void RadialGauge::RadialGauge_Tapped([[maybe_unused]] IInspectable const& sender, TappedRoutedEventArgs const& e)
    {
        SetGaugeValueFromPoint(e.GetPosition(*this));
    }

    void RadialGauge::RadialGauge_PointerReleased([[maybe_unused]] IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        if (IsInteractive())
        {
            e.Handled(true);
        }
    }

    void RadialGauge::SetKeyboardAccelerators()
    {
        // Small step
        AddKeyboardAccelerator(VirtualKeyModifiers::None, VirtualKey::Left, [](auto&, auto& kaea) 
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::None, VirtualKey::Up, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::None, VirtualKey::Right, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::None, VirtualKey::Down, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        // Large step
        AddKeyboardAccelerator(VirtualKeyModifiers::Control, VirtualKey::Left, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::Control, VirtualKey::Up, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::Control, VirtualKey::Right, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(VirtualKeyModifiers::Control, VirtualKey::Down, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::CommunityToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });
    }

    void RadialGauge::AddKeyboardAccelerator(
        VirtualKeyModifiers keyModifiers,
        VirtualKey key,
        TypedEventHandler<KeyboardAccelerator, KeyboardAcceleratorInvokedEventArgs> const& handler)
    {
        KeyboardAccelerator accelerator;
        accelerator.Modifiers(keyModifiers);
        accelerator.Key(key);

        accelerator.Invoked(handler);
        KeyboardAccelerators().Append(accelerator);
    }
}