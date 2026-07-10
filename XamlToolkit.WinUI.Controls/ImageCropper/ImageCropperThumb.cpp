#include "pch.h"
#include "winrt_module_imports.h"
#include "ImageCropperThumb.h"
#if __has_include("ImageCropperThumb.g.cpp")
#include "ImageCropperThumb.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    ImageCropperThumb::ImageCropperThumb()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        RenderTransform(_layoutTransform);
        ManipulationMode(winrt::ManipulationModes::TranslateX | winrt::ManipulationModes::TranslateY);
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
        if (_layoutTransform)
        {
            _layoutTransform.X(X() - (ActualWidth() / 2));
            _layoutTransform.Y(Y() - (ActualHeight() / 2));
        }
    }

    void ImageCropperThumb::OnXChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto target = winrt::get_self<ImageCropperThumb>(d.as<class_type>())->get_strong();
        target->UpdatePosition();
    }

    void ImageCropperThumb::OnYChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto target = winrt::get_self<ImageCropperThumb>(d.as<class_type>())->get_strong();
        target->UpdatePosition();
    }

    void ImageCropperThumb::Control_PointerExited([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerExited(e);
        winrt::VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::OnPointerPressed(winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerPressed(e);
        winrt::VisualStateManager::GoToState(*this, PressedState, true);
    }

    void ImageCropperThumb::OnPointerReleased(winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerReleased(e);
        winrt::VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::Control_PointerEntered([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerEntered(e);
        winrt::VisualStateManager::GoToState(*this, PointerOverState, true);
    }

    void ImageCropperThumb::ImageCropperThumb_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
    {
        UpdatePosition();
    }

    void ImageCropperThumb::Control_PointerCaptureLost([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerCaptureLost(e);
        winrt::VisualStateManager::GoToState(*this, NormalState, true);
    }

    void ImageCropperThumb::Control_PointerCanceled([[maybe_unused]] winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e)
    {
        base_type::OnPointerCanceled(e);
        winrt::VisualStateManager::GoToState(*this, NormalState, true);
    }
}
