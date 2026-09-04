#include "pch.h"
#include "winrt_module_imports.h"
#include "ControlStoryboardAction.h"
#if __has_include("ControlStoryboardAction.g.cpp")
#include "ControlStoryboardAction.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> ControlStoryboardAction::ControlStoryboardOptionProperty =
        winrt::DependencyProperty::Register(
            L"ControlStoryboardOption",
            winrt::xaml_typename<enum ControlStoryboardOption>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(winrt::box_value(ControlStoryboardOption::Play)));

    const wil::single_threaded_property<winrt::DependencyProperty> ControlStoryboardAction::StoryboardProperty =
        winrt::DependencyProperty::Register(
            L"Storyboard",
            winrt::xaml_typename<winrt::Storyboard>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr));

    const wil::single_threaded_property<winrt::DependencyProperty> ControlStoryboardAction::IsPausedProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"IsPaused",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<winrt::Storyboard>(),
            winrt::PropertyMetadata(winrt::box_value(false)));

    ControlStoryboardOption ControlStoryboardAction::ControlStoryboardOption() const
    {
        auto value = GetValue(ControlStoryboardOptionProperty());
        return winrt::unbox_value_or<enum ControlStoryboardOption>(value, ControlStoryboardOption::Play);
    }

    void ControlStoryboardAction::ControlStoryboardOption(enum ControlStoryboardOption const& value)
    {
        SetValue(ControlStoryboardOptionProperty(), winrt::box_value(value));
    }

    winrt::Storyboard ControlStoryboardAction::Storyboard() const
    {
        return GetValue(StoryboardProperty()).try_as<winrt::Storyboard>();
    }

    void ControlStoryboardAction::Storyboard(winrt::Storyboard const& value)
    {
        SetValue(StoryboardProperty(), value);
    }

	void ControlStoryboardAction::SetIsPaused(winrt::Storyboard const& obj, bool value)
	{
        obj.SetValue(IsPausedProperty(), winrt::box_value(value));
	}

	bool ControlStoryboardAction::GetIsPaused(winrt::Storyboard const& obj)
	{
        return winrt::unbox_value<bool>(obj.GetValue(IsPausedProperty()));
	}

    winrt::IInspectable ControlStoryboardAction::Execute([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::IInspectable const& parameter) const
    {
        const auto storyboard = Storyboard();
        if (!storyboard)
        {
            return winrt::box_value(false);
        }

        switch (ControlStoryboardOption())
        {
        case ControlStoryboardOption::Play:
            storyboard.Begin();
            break;

        case ControlStoryboardOption::Stop:
            storyboard.Stop();
            break;

        case ControlStoryboardOption::TogglePlayPause:
        {
            const auto currentState = storyboard.GetCurrentState();
            if (currentState == winrt::ClockState::Stopped)
            {
                SetIsPaused(Storyboard(), false);
                storyboard.Begin();
            }
            else if (GetIsPaused(Storyboard()))
            {
                SetIsPaused(Storyboard(), false);
                storyboard.Resume();
            }
            else
            {
                SetIsPaused(Storyboard(), true);
                storyboard.Pause();
            }
        }
        break;

        case ControlStoryboardOption::Pause:
            storyboard.Pause();
            break;

        case ControlStoryboardOption::Resume:
            storyboard.Resume();
            break;

        case ControlStoryboardOption::SkipToFill:
            storyboard.SkipToFill();
            break;

        default:
            return winrt::box_value(false);
        }

        return winrt::box_value(true);
    }
}
