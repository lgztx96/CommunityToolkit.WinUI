#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "RadialGauge.h"

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    void RadialGauge::RadialGauge_ManipulationDelta([[maybe_unused]] winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        SetGaugeValueFromPoint(e.Position());
    }

    void RadialGauge::RadialGauge_Tapped([[maybe_unused]] winrt::IInspectable const& sender, winrt::TappedRoutedEventArgs const& e)
    {
        SetGaugeValueFromPoint(e.GetPosition(*this));
    }

    void RadialGauge::RadialGauge_PointerReleased([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        if (IsInteractive())
        {
            e.Handled(true);
        }
    }

    void RadialGauge::SetKeyboardAccelerators()
    {
        // Small step
        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::None, winrt::VirtualKey::Left, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::None, winrt::VirtualKey::Up, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::None, winrt::VirtualKey::Right, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::None, winrt::VirtualKey::Down, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.SmallChange())));
                kaea.Handled(true);
            }
        });

        // Large step
        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::Control, winrt::VirtualKey::Left, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::Control, winrt::VirtualKey::Up, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::Control, winrt::VirtualKey::Right, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::min(gauge.Maximum(), gauge.Value() + std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });

        AddKeyboardAccelerator(winrt::VirtualKeyModifiers::Control, winrt::VirtualKey::Down, [](auto&, auto& kaea)
        {
            if (auto gauge = kaea.Element().try_as<winrt::XamlToolkit::WinUI::Controls::RadialGauge>())
            {
                gauge.Value(std::max(gauge.Minimum(), gauge.Value() - std::max(gauge.StepSize(), gauge.LargeChange())));
                kaea.Handled(true);
            }
        });
    }

    void RadialGauge::AddKeyboardAccelerator(
        winrt::VirtualKeyModifiers keyModifiers,
        winrt::VirtualKey key,
        winrt::TypedEventHandler<winrt::KeyboardAccelerator, winrt::KeyboardAcceleratorInvokedEventArgs> const& handler)
    {
        winrt::KeyboardAccelerator accelerator;
        accelerator.Modifiers(keyModifiers);
        accelerator.Key(key);

        accelerator.Invoked(handler);
        KeyboardAccelerators().Append(accelerator);
    }
}
