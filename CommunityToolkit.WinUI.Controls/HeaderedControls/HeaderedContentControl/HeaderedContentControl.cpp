#include "pch.h"
#include "HeaderedContentControl.h"
#if __has_include("HeaderedContentControl.g.cpp")
#include "HeaderedContentControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
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

    void HeaderedContentControl::OnHeaderChanged([[maybe_unused]] IInspectable const& oldValue, [[maybe_unused]] IInspectable const& newValue) { }

    void HeaderedContentControl::OnOrientationChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        auto control = winrt::get_self<HeaderedContentControl>(d.as<class_type>())->get_strong();
        control->SetOrientation();
    }

    void HeaderedContentControl::OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        auto control = winrt::get_self<HeaderedContentControl>(d.as<class_type>())->get_strong();
        control->SetHeaderVisibility();
        control->OnHeaderChanged(e.OldValue(), e.NewValue());
    }

    void HeaderedContentControl::SetHeaderVisibility()
    {
        if (auto headerPresenter = GetTemplateChild(PartHeaderPresenter).try_as<FrameworkElement>())
        {
            if (auto headerText = Header().try_as<winrt::hstring>())
            {
				headerPresenter.Visibility(headerText->empty()
                    ? Visibility::Collapsed
                    : Visibility::Visible);
            }
            else
            {
                headerPresenter.Visibility(Header() ? Visibility::Visible : Visibility::Collapsed);
            }
        }
    }

    void HeaderedContentControl::SetOrientation()
    {
        auto orientation = Orientation() == Orientation::Vertical ? L"Vertical" : L"Horizontal";

        VisualStateManager::GoToState(*this, orientation, true);
    }
}
