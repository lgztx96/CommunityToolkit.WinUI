#include "pch.h"
#include "winrt_module_imports.h"
#include "TextBoxExtensions.h"
#include "SurfaceDialOptions.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    // Static member definitions
    winrt::RadialController TextBoxExtensions::_controller = nullptr;
    winrt::RadialControllerMenuItem TextBoxExtensions::_stepTextMenuItem = nullptr;
    winrt::weak_ref<winrt::TextBox> TextBoxExtensions::_textBox = nullptr;
    winrt::event_token TextBoxExtensions::_gotFocusToken = {};
    winrt::event_token TextBoxExtensions::_lostFocusToken = {};
    winrt::event_token TextBoxExtensions::_rotationToken = {};
    winrt::event_token TextBoxExtensions::_buttonToken = {};

    void TextBoxExtensions::OnSurfaceDialOptionsPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (!IsSurfaceDialOptionsSupported())
        {
            return;
        }

        auto textBox = d.try_as<winrt::TextBox>();
        if (!textBox)
        {
            return;
        }

        // Initialize our RadialController once
        if (!_controller)
        {
            _controller = winrt::RadialController::CreateForCurrentView();
        }

        // Detach old event handlers using stored tokens
        textBox.GotFocus(_gotFocusToken);
        textBox.LostFocus(_lostFocusToken);

        if (e.NewValue())
        {
            _gotFocusToken = textBox.GotFocus(&TextBoxExtensions::TextBox_GotFocus_SurfaceDial);
            _lostFocusToken = textBox.LostFocus(&TextBoxExtensions::TextBox_LostFocus_SurfaceDial);
        }
    }

    void TextBoxExtensions::TextBox_GotFocus_SurfaceDial(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        _textBox = sender.try_as<winrt::TextBox>();

        auto textBox = _textBox.get();
        if (!textBox || !_controller)
        {
            return;
        }

        if (!IsSurfaceDialOptionsSupported())
        {
            return;
        }

        // Detach previous controller handlers using stored tokens
        _controller.RotationChanged(_rotationToken);
        _controller.ButtonClicked(_buttonToken);

        auto options = GetSurfaceDialOptions(textBox);
        if (!options)
        {
            options = SurfaceDialOptions::Default();
        }

        if (!_stepTextMenuItem)
        {
            _stepTextMenuItem = winrt::RadialControllerMenuItem::CreateFromKnownIcon(L"Step Text Box", options.Icon());
        }

        _controller.Menu().Items().Append(_stepTextMenuItem);
        _controller.Menu().SelectMenuItem(_stepTextMenuItem);

        _controller.UseAutomaticHapticFeedback(options.EnableHapticFeedback());
        _controller.RotationResolutionInDegrees(options.RotationResolutionInDegrees());
        _rotationToken = _controller.RotationChanged(&TextBoxExtensions::Controller_RotationChanged);

        if (options.EnableTapToNextControl())
        {
            _buttonToken = _controller.ButtonClicked(&TextBoxExtensions::Controller_ButtonClicked);
        }
    }

    void TextBoxExtensions::TextBox_LostFocus_SurfaceDial([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        auto textBox = _textBox.get();
        if (!textBox || !_controller)
        {
            return;
        }

        auto options = GetSurfaceDialOptions(textBox);
        if (!options)
        {
            options = SurfaceDialOptions::Default();
        }

        if (_stepTextMenuItem)
        {
            uint32_t index;
            if (auto items = _controller.Menu().Items(); items.IndexOf(_stepTextMenuItem, index))
            {
                items.RemoveAt(index);
            }
        }

        _controller.RotationChanged(_rotationToken);

        if (options.EnableTapToNextControl())
        {
            _controller.ButtonClicked(_buttonToken);
        }

        _textBox = nullptr;
    }

    void TextBoxExtensions::Controller_RotationChanged([[maybe_unused]] winrt::RadialController const& sender, winrt::RadialControllerRotationChangedEventArgs const& args)
    {
        auto textBox = _textBox.get();
        if (!textBox)
        {
            return;
        }

        std::wstring text{ textBox.Text() };
        auto options = GetSurfaceDialOptions(textBox);
        if (!options)
        {
            options = SurfaceDialOptions::Default();
        }

        double number;
        try
        {
            number = std::stod(text);
            // We only care about the sign of RotationDeltaInDegrees to determine if we're going up/down
            // The value is controlled by the StepValue independent of when we should call the rotation changed event.
            number += (args.RotationDeltaInDegrees() > 0 ? 1.0 : -1.0) * options.StepValue();

            if (options.EnableMinMaxValue())
            {
                number = std::clamp(number, options.MinValue(), options.MaxValue());
            }
        }
        catch (...)
        {
            number = 0.0;
        }

        textBox.Text(winrt::format(L"{:.2f}", number));
    }

    void TextBoxExtensions::Controller_ButtonClicked([[maybe_unused]] winrt::RadialController const& sender, [[maybe_unused]] winrt::RadialControllerButtonClickedEventArgs const& args)
    {
        winrt::FocusManager::TryMoveFocus(winrt::FocusNavigationDirection::Next);
    }
}
