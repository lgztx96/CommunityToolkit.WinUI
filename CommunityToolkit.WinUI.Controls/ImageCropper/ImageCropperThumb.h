#pragma once

#include "ImageCropperThumb.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct ImageCropperThumb : ImageCropperThumbT<ImageCropperThumb>
    {
        static constexpr std::wstring_view NormalState = L"Normal";
        static constexpr std::wstring_view PointerOverState = L"PointerOver";
        static constexpr std::wstring_view PressedState = L"Pressed";
        static constexpr std::wstring_view DisabledState = L"Disabled";

        wil::single_threaded_rw_property<ThumbPosition> Position;

        double X() { return winrt::unbox_value<double>(GetValue(XProperty)); }
        void X(double value) { SetValue(XProperty, winrt::box_value(value)); }

        double Y() { return winrt::unbox_value<double>(GetValue(YProperty)); }
        void Y(double value) { SetValue(YProperty, winrt::box_value(value)); }

        ImageCropperThumb();

        void OnApplyTemplate();

        void UpdatePosition();

        static void OnXChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnYChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> XProperty =
            DependencyProperty::Register(L"X", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0), OnXChanged));

        static inline const wil::single_threaded_property<DependencyProperty> YProperty =
            DependencyProperty::Register(L"Y", winrt::xaml_typename<double>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(0.0), OnYChanged));

        void Control_PointerEntered(winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e);

        void Control_PointerExited(winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e);

        void OnPointerPressed(PointerRoutedEventArgs const& e);

        void OnPointerReleased(PointerRoutedEventArgs const& e);

    private:
        TranslateTransform _layoutTransform;

        void ImageCropperThumb_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, SizeChangedEventArgs const& e);

        void Control_PointerCaptureLost(winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e);

        void Control_PointerCanceled(winrt::Windows::Foundation::IInspectable const& sender, PointerRoutedEventArgs const& e);
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct ImageCropperThumb : ImageCropperThumbT<ImageCropperThumb, implementation::ImageCropperThumb>
    {
    };
}
