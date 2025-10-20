#include "pch.h"
#include "ImageCropperThumb.h"
#if __has_include("ImageCropperThumb.g.cpp")
#include "ImageCropperThumb.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    ImageCropperThumb::ImageCropperThumb()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        RenderTransform(_layoutTransform);
        ManipulationMode(ManipulationModes::TranslateX | ManipulationModes::TranslateY);
        SizeChanged({ get_weak(), &ImageCropperThumb::ImageCropperThumb_SizeChanged });
    }

    void ImageCropperThumb::OnApplyTemplate()
    {
        PointerEntered({ get_weak(), &ImageCropperThumb::Control_PointerEntered });
        PointerExited({ get_weak(), &ImageCropperThumb::Control_PointerExited });
        PointerCaptureLost({ get_weak(), &ImageCropperThumb::Control_PointerCaptureLost });
        PointerCanceled({ get_weak(), &ImageCropperThumb::Control_PointerCanceled });
    }

    void ImageCropperThumb::UpdatePosition()
    {
        if (_layoutTransform != nullptr)
        {
            _layoutTransform.X(X() - (ActualWidth() / 2));
            _layoutTransform.Y(Y() - (ActualHeight() / 2));
        }
    }

    void ImageCropperThumb::OnXChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        auto target = winrt::get_self<ImageCropperThumb>(d.as<class_type>())->get_strong();
        target->UpdatePosition();
    }

    void ImageCropperThumb::OnYChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
    {
        auto target = winrt::get_self<ImageCropperThumb>(d.as<class_type>())->get_strong();
        target->UpdatePosition();
    }

    void ImageCropperThumb::Control_PointerExited([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerExited(e);
        VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::OnPointerPressed(PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerPressed(e);
        VisualStateManager::GoToState(*this, PressedState, true);
    }

    void ImageCropperThumb::OnPointerReleased(PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerReleased(e);
        VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::Control_PointerEntered([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerEntered(e);
        VisualStateManager::GoToState(*this, PointerOverState, true);
    }

    void ImageCropperThumb::ImageCropperThumb_SizeChanged([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
    {
        UpdatePosition();
    }

    void ImageCropperThumb::Control_PointerCaptureLost([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerCaptureLost(e);
        VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::Control_PointerCanceled([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerCanceled(e);
        VisualStateManager::GoToState(*this, NormalState, true);
    }
}
