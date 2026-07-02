#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <cmath>
#endif
#include "SizerBase.h"
#if __has_include("SizerBase.g.cpp")
#include "SizerBase.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> SizerBase::CursorProperty =
        winrt::DependencyProperty::Register(
            L"Cursor",
            winrt::xaml_typename<CursorEnum>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &SizerBase::OnOrientationPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> SizerBase::DragIncrementProperty =
        winrt::DependencyProperty::Register(
            L"DragIncrement",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(1.0)));

    const wil::single_threaded_property<winrt::DependencyProperty> SizerBase::KeyboardIncrementProperty =
        winrt::DependencyProperty::Register(
            L"KeyboardIncrement",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(8.0)));

    const wil::single_threaded_property<winrt::DependencyProperty> SizerBase::OrientationProperty =
        winrt::DependencyProperty::Register(
            L"Orientation",
            winrt::xaml_typename<winrt::Orientation>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(winrt::Orientation::Vertical), &SizerBase::OnOrientationPropertyChanged));

    const wil::single_threaded_property<winrt::DependencyProperty> SizerBase::IsThumbVisibleProperty =
        winrt::DependencyProperty::Register(
            L"IsThumbVisible",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(true), &SizerBase::OnIsThumbVisiblePropertyChanged));

    SizerBase::SizerBase()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void SizerBase::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        // Register Events
        _loadedRevoker = Loaded(winrt::auto_revoke, { this, &SizerBase::SizerBase_Loaded });
        _pointerEnteredRevoker = PointerEntered(winrt::auto_revoke, { this, &SizerBase::SizerBase_PointerEntered });
        _pointerExitedRevoker = PointerExited(winrt::auto_revoke, { this, &SizerBase::SizerBase_PointerExited });
        _pointerPressedRevoker = PointerPressed(winrt::auto_revoke, { this, &SizerBase::SizerBase_PointerPressed });
        _pointerReleasedRevoker = PointerReleased(winrt::auto_revoke, { this, &SizerBase::SizerBase_PointerReleased });
        _manipulationStartedRevoker = ManipulationStarted(winrt::auto_revoke, { this, &SizerBase::SizerBase_ManipulationStarted });
        _manipulationCompletedRevoker = ManipulationCompleted(winrt::auto_revoke, { this, &SizerBase::SizerBase_ManipulationCompleted });
        _isEnabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &SizerBase::SizerBase_IsEnabledChanged });

        // Trigger initial state transition based on if we're Enabled or not currently.
        SizerBase_IsEnabledChanged(*this, nullptr);

        // On WinAppSDK, we'll trigger this to setup the initial ProtectedCursor value.
        _appliedTemplate = true;

        // Ensure we have the proper cursor value setup, as we can only set now for WinUI 3
        OnOrientationPropertyChanged(*this, nullptr);

        // Ensure we set the Thumb visibility
        OnIsThumbVisiblePropertyChanged(*this, nullptr);
    }

    void SizerBase::SizerBase_Loaded([[maybe_unused]] winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e)
    {
        _loadedRevoker.revoke();

        overridable().OnLoaded(e);
    }

    void SizerBase::OnOrientationPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto sizerBase = d.try_as<class_type>())
        {
            auto gripper = winrt::get_self<SizerBase>(sizerBase)->get_strong();
            winrt::VisualStateManager::GoToState(sizerBase, gripper->Orientation() == winrt::Orientation::Vertical ? VerticalState : HorizontalState, true);

            auto cursorByOrientation = gripper->Orientation() == winrt::Orientation::Vertical ? CursorEnum::SizeWestEast : CursorEnum::SizeNorthSouth;

            // See if there's been a cursor override, otherwise we'll pick
            auto cursor = gripper->ReadLocalValue(CursorProperty);
            if (cursor == winrt::DependencyProperty::UnsetValue() || cursor == nullptr)
            {
                cursor = winrt::box_value(cursorByOrientation);
            }

            // Need to wait until we're at least applying template step of loading before setting Cursor
            // See https://github.com/microsoft/microsoft-ui-xaml/issues/7062
            const auto cursorValue = cursor.try_as<CursorEnum>();
            const auto current = gripper->ProtectedCursor().try_as<winrt::Microsoft::UI::Input::InputSystemCursor>();
            if (gripper->_appliedTemplate && cursorValue &&
                (!current || current.CursorShape() != cursorValue))
            {
                gripper->ProtectedCursor(winrt::Microsoft::UI::Input::InputSystemCursor::Create(*cursorValue));
            }
        }
    }

    void SizerBase::OnIsThumbVisiblePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto gripper = d.try_as<class_type>())
        {
            winrt::VisualStateManager::GoToState(gripper, gripper.IsThumbVisible() ? VisibleState : CollapsedState, true);
        }
    }

    void SizerBase::OnKeyDown(winrt::KeyRoutedEventArgs const& e)
    {
        // If we're manipulating with mouse/touch, we ignore keyboard inputs.
        if (_dragging)
        {
            return;
        }

        //// TODO: Do we want Ctrl/Shift to be a small increment (kind of inverse to old GridSplitter logic)?
        //// var ctrl = Window.Current.CoreWindow.GetKeyState(VirtualKey.Control);
        //// if (ctrl.HasFlag(CoreVirtualKeyStates.Down))
        //// Note: WPF doesn't do anything here.
        //// I think if we did anything, we'd create a SmallKeyboardIncrement property?

        // Initialize a drag event for this keyboard interaction.

        overridable().OnDragStarting();

        if (Orientation() == winrt::Orientation::Vertical)
        {
            auto horizontalChange = KeyboardIncrement();

            if (FlowDirection() == winrt::FlowDirection::RightToLeft)
            {
                horizontalChange *= -1;
            }

            if (e.Key() == winrt::Windows::System::VirtualKey::Left)
            {
                OnDragHorizontal(-horizontalChange);
            }
            else if (e.Key() == winrt::Windows::System::VirtualKey::Right)
            {
                OnDragHorizontal(horizontalChange);
            }
        }
        else
        {
            if (e.Key() == winrt::Windows::System::VirtualKey::Up)
            {
                overridable().OnDragVertical(-KeyboardIncrement());
            }
            else if (e.Key() == winrt::Windows::System::VirtualKey::Down)
            {
                overridable().OnDragVertical(KeyboardIncrement());
            }
        }
    }

    void SizerBase::OnManipulationStarting(winrt::ManipulationStartingRoutedEventArgs const& e)
    {
        base_type::OnManipulationStarting(e);

        overridable().OnDragStarting();
    }

    void SizerBase::OnManipulationDelta(winrt::ManipulationDeltaRoutedEventArgs const& e)
    {
        // We use Truncate here to provide 'snapping' points with the DragIncrement property
        // It works for both our negative and positive values, as otherwise we'd need to use
        // Ceiling when negative and Floor when positive to maintain the correct behavior.
		const auto dragIncrement = DragIncrement();
		const auto cumulativeTranslation = e.Cumulative().Translation;

        auto horizontalChange = std::trunc(cumulativeTranslation.X / dragIncrement) * dragIncrement;
        auto verticalChange = std::trunc(cumulativeTranslation.Y / dragIncrement) * dragIncrement;

        // Important: adjust for RTL language flow settings and invert horizontal axis
        if (FlowDirection() == winrt::FlowDirection::RightToLeft)
        {
            horizontalChange *= -1;
        }

        if (Orientation() == winrt::Orientation::Vertical)
        {
            if (!overridable().OnDragHorizontal(horizontalChange))
            {
                return;
            }
        }
        else if (Orientation() == winrt::Orientation::Horizontal)
        {
            if (!overridable().OnDragVertical(verticalChange))
            {
                return;
            }
        }

        base_type::OnManipulationDelta(e);
    }

    void SizerBase::SizerBase_PointerReleased([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        _pressed = false;

        if (IsEnabled())
        {
            winrt::VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
        }
    }

    void SizerBase::SizerBase_PointerPressed([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        _pressed = true;

        if (IsEnabled())
        {
            winrt::VisualStateManager::GoToState(*this, PointerOverState, true);
        }
    }

    void SizerBase::SizerBase_PointerExited([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        _pointerEntered = false;

        if (!_pressed && !_dragging && IsEnabled())
        {
            winrt::VisualStateManager::GoToState(*this, NormalState, true);
        }
    }

    void SizerBase::SizerBase_PointerEntered([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::PointerRoutedEventArgs const& e)
    {
        _pointerEntered = true;

        if (!_pressed && !_dragging && IsEnabled())
        {
            winrt::VisualStateManager::GoToState(*this, PointerOverState, true);
        }
    }

    void SizerBase::SizerBase_ManipulationCompleted([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationCompletedRoutedEventArgs const& e)
    {
        _dragging = false;
        _pressed = false;
        winrt::VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
    }

    void SizerBase::SizerBase_ManipulationStarted([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationStartedRoutedEventArgs const& e)
    {
        _dragging = true;
        winrt::VisualStateManager::GoToState(*this, PressedState, true);
    }

    void SizerBase::SizerBase_IsEnabledChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (!IsEnabled())
        {
            winrt::VisualStateManager::GoToState(*this, DisabledState, true);
        }
        else
        {
            winrt::VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
        }
    }

    bool SizerBase::IsValidHeight(winrt::FrameworkElement const& target, double newHeight, double parentActualHeight)
    {
        auto minHeight = target.MinHeight();
        if (newHeight < 0 || (!std::isnan(minHeight) && newHeight < minHeight))
        {
            return false;
        }

        auto maxHeight = target.MaxHeight();
        if (!std::isnan(maxHeight) && newHeight > maxHeight)
        {
            return false;
        }

        if (newHeight <= parentActualHeight)
        {
            return false;
        }

        return true;
    }

    bool SizerBase::IsValidWidth(winrt::FrameworkElement const& target, double newWidth, double parentActualWidth)
    {
        auto minWidth = target.MinWidth();
        if (newWidth < 0 || (!std::isnan(minWidth) && newWidth < minWidth))
        {
            return false;
        }

        auto maxWidth = target.MaxWidth();
        if (!std::isnan(maxWidth) && newWidth > maxWidth)
        {
            return false;
        }

        if (newWidth <= parentActualWidth)
        {
            return false;
        }

        return true;
    }
}
