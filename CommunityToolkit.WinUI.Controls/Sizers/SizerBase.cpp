#include "pch.h"
#include "SizerBase.h"
#if __has_include("SizerBase.g.cpp")
#include "SizerBase.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    SizerBase::SizerBase()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void SizerBase::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        // Unregister Events
        if (_loadedToken) Loaded(_loadedToken);
        if (_enteredToken) PointerEntered(_enteredToken);
        if (_exitedToken) PointerExited(_exitedToken);
        if (_pressedToken) PointerPressed(_pressedToken);
        if (_releasedToken) PointerReleased(_releasedToken);
        if (_startedToken) ManipulationStarted(_startedToken);
        if (_completedToken) ManipulationCompleted(_completedToken);
        if (_enabledToken) IsEnabledChanged(_enabledToken);

        // Register Events
        _loadedToken = Loaded({ this, &SizerBase::SizerBase_Loaded });
        _enteredToken = PointerEntered({ this, &SizerBase::SizerBase_PointerEntered });
        _exitedToken = PointerExited({ this, &SizerBase::SizerBase_PointerExited });
        _pressedToken = PointerPressed({ this, &SizerBase::SizerBase_PointerPressed });
        _releasedToken = PointerReleased({ this, &SizerBase::SizerBase_PointerReleased });
        _startedToken = ManipulationStarted({ this, &SizerBase::SizerBase_ManipulationStarted });
        _completedToken = ManipulationCompleted({ this, &SizerBase::SizerBase_ManipulationCompleted });
        _enabledToken = IsEnabledChanged({ this, &SizerBase::SizerBase_IsEnabledChanged });

        // Trigger initial state transition based on if we're Enabled or not currently.
        SizerBase_IsEnabledChanged(*this, nullptr);

        // On WinAppSDK, we'll trigger this to setup the initial ProtectedCursor value.
        _appliedTemplate = true;

        // Ensure we have the proper cursor value setup, as we can only set now for WinUI 3
        OnOrientationPropertyChanged(*this, nullptr);

        // Ensure we set the Thumb visiblity
        OnIsThumbVisiblePropertyChanged(*this, nullptr);
    }

    void SizerBase::SizerBase_Loaded([[maybe_unused]] IInspectable const& sender, RoutedEventArgs const& e)
    {
        Loaded(_loadedToken);

        overridable().OnLoaded(e);
    }

    void SizerBase::OnOrientationPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        if (auto sizerBase = d.try_as<class_type>())
        {
            auto gripper = winrt::get_self<SizerBase>(sizerBase)->get_strong();
            VisualStateManager::GoToState(sizerBase, gripper->Orientation() == Orientation::Vertical ? VerticalState : HorizontalState, true);

            CursorEnum cursorByOrientation = gripper->Orientation() == Orientation::Vertical ? CursorEnum::SizeWestEast : CursorEnum::SizeNorthSouth;

            // See if there's been a cursor override, otherwise we'll pick
            auto cursor = gripper->ReadLocalValue(CursorProperty);
            if (cursor == DependencyProperty::UnsetValue() || cursor == nullptr)
            {
                cursor = winrt::box_value(cursorByOrientation);
            }

            // Need to wait until we're at least applying template step of loading before setting Cursor
            // See https://github.com/microsoft/microsoft-ui-xaml/issues/7062
            auto cursorValue = cursor.try_as<CursorEnum>();
            auto current = gripper->ProtectedCursor().try_as<Microsoft::UI::Input::InputSystemCursor>();
            if (gripper->_appliedTemplate && cursorValue &&
                (!current || current.CursorShape() != cursorValue))
            {
                gripper->ProtectedCursor(Microsoft::UI::Input::InputSystemCursor::Create(*cursorValue));
            }
        }
    }

    void SizerBase::OnIsThumbVisiblePropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        if (auto gripper = d.try_as<class_type>())
        {
            VisualStateManager::GoToState(gripper, gripper.IsThumbVisible() ? VisibleState : CollapsedState, true);
        }
    }

    void SizerBase::OnKeyDown(KeyRoutedEventArgs const& e)
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

        if (Orientation() == Orientation::Vertical)
        {
            auto horizontalChange = KeyboardIncrement();

            if (FlowDirection() == FlowDirection::RightToLeft)
            {
                horizontalChange *= -1;
            }

            if (e.Key() == Windows::System::VirtualKey::Left)
            {
                OnDragHorizontal(-horizontalChange);
            }
            else if (e.Key() == Windows::System::VirtualKey::Right)
            {
                OnDragHorizontal(horizontalChange);
            }
        }
        else
        {
            if (e.Key() == Windows::System::VirtualKey::Up)
            {
                overridable().OnDragVertical(-KeyboardIncrement());
            }
            else if (e.Key() == Windows::System::VirtualKey::Down)
            {
                overridable().OnDragVertical(KeyboardIncrement());
            }
        }
    }

    void SizerBase::OnManipulationStarting(ManipulationStartingRoutedEventArgs const& e)
    {
        base_type::OnManipulationStarting(e);

        overridable().OnDragStarting();
    }

    void SizerBase::OnManipulationDelta(ManipulationDeltaRoutedEventArgs const& e)
    {
        // We use Truncate here to provide 'snapping' points with the DragIncrement property
        // It works for both our negative and positive values, as otherwise we'd need to use
        // Ceiling when negative and Floor when positive to maintain the correct behavior.
        auto horizontalChange =
            std::trunc(e.Cumulative().Translation.X / DragIncrement()) * DragIncrement();
        auto verticalChange =
            std::trunc(e.Cumulative().Translation.Y / DragIncrement()) * DragIncrement();

        // Important: adjust for RTL language flow settings and invert horizontal axis
        if (FlowDirection() == FlowDirection::RightToLeft)
        {
            horizontalChange *= -1;
        }

        if (Orientation() == Orientation::Vertical)
        {
            if (!overridable().OnDragHorizontal(horizontalChange))
            {
                return;
            }
        }
        else if (Orientation() == Orientation::Horizontal)
        {
            if (!overridable().OnDragVertical(verticalChange))
            {
                return;
            }
        }

        base_type::OnManipulationDelta(e);
    }

    void SizerBase::SizerBase_PointerReleased([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
    {
        _pressed = false;

        if (IsEnabled())
        {
            VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
        }
    }

    void SizerBase::SizerBase_PointerPressed([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
    {
        _pressed = true;

        if (IsEnabled())
        {
            VisualStateManager::GoToState(*this, PointerOverState, true);
        }
    }

    void SizerBase::SizerBase_PointerExited([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
    {
        _pointerEntered = false;

        if (!_pressed && !_dragging && IsEnabled())
        {
            VisualStateManager::GoToState(*this, NormalState, true);
        }
    }

    void SizerBase::SizerBase_PointerEntered([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] PointerRoutedEventArgs const& e)
    {
        _pointerEntered = true;

        if (!_pressed && !_dragging && IsEnabled())
        {
            VisualStateManager::GoToState(*this, PointerOverState, true);
        }
    }

    void SizerBase::SizerBase_ManipulationCompleted([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] ManipulationCompletedRoutedEventArgs const& e)
    {
        _dragging = false;
        _pressed = false;
        VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
    }

    void SizerBase::SizerBase_ManipulationStarted([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] ManipulationStartedRoutedEventArgs const& e)
    {
        _dragging = true;
        VisualStateManager::GoToState(*this, PressedState, true);
    }

    void SizerBase::SizerBase_IsEnabledChanged([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        if (!IsEnabled())
        {
            VisualStateManager::GoToState(*this, DisabledState, true);
        }
        else
        {
            VisualStateManager::GoToState(*this, _pointerEntered ? PointerOverState : NormalState, true);
        }
    }

    bool SizerBase::IsValidHeight(FrameworkElement const& target, double newHeight, double parentActualHeight)
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

    bool SizerBase::IsValidWidth(FrameworkElement const& target, double newWidth, double parentActualWidth)
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
