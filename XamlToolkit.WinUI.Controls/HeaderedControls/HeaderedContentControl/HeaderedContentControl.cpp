#include "pch.h"
#include "winrt_module_imports.h"
#include "HeaderedContentControl.h"
#if __has_include("HeaderedContentControl.g.cpp")
#include "HeaderedContentControl.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    HeaderedContentControl::HeaderedContentControl()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void HeaderedContentControl::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        SetHeaderVisibility();
        SetOrientation();
    }

    void HeaderedContentControl::OnHeaderChanged([[maybe_unused]] winrt::IInspectable const& oldValue, [[maybe_unused]] winrt::IInspectable const& newValue) { }

    void HeaderedContentControl::OnOrientationChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto control = winrt::get_self<HeaderedContentControl>(d.as<class_type>())->get_strong();
        control->SetOrientation();
    }

    void HeaderedContentControl::OnHeaderChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto control = winrt::get_self<HeaderedContentControl>(d.as<class_type>())->get_strong();
        control->SetHeaderVisibility();
        control->OnHeaderChanged(e.OldValue(), e.NewValue());
    }

    void HeaderedContentControl::SetHeaderVisibility()
    {
        if (auto headerPresenter = GetTemplateChild(PartHeaderPresenter).try_as<winrt::FrameworkElement>())
        {
            if (auto headerText = Header().try_as<winrt::hstring>())
            {
				headerPresenter.Visibility(headerText->empty()
                    ? winrt::Visibility::Collapsed
                    : winrt::Visibility::Visible);
            }
            else
            {
                headerPresenter.Visibility(Header() ? winrt::Visibility::Visible : winrt::Visibility::Collapsed);
            }
        }
    }

    void HeaderedContentControl::SetOrientation()
    {
        auto orientation = Orientation() == winrt::Orientation::Vertical ? L"Vertical" : L"Horizontal";

        VisualStateManager::GoToState(*this, orientation, true);
    }
}
