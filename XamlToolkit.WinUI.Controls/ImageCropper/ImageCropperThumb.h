#pragma once

#include "ImageCropperThumb.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct ImageCropperThumb : ImageCropperThumbT<ImageCropperThumb>
    {
        static constexpr std::wstring_view NormalState = L"Normal";
        static constexpr std::wstring_view PointerOverState = L"PointerOver";
        static constexpr std::wstring_view PressedState = L"Pressed";
        static constexpr std::wstring_view DisabledState = L"Disabled";

        wil::single_threaded_rw_property<ThumbPosition> Position;

        double X() const { return winrt::unbox_value<double>(GetValue(XProperty())); }
        void X(double value) { SetValue(XProperty(), winrt::box_value(value)); }

        double Y() const { return winrt::unbox_value<double>(GetValue(YProperty())); }
        void Y(double value) { SetValue(YProperty(), winrt::box_value(value)); }

        ImageCropperThumb();

        void OnApplyTemplate();

        void UpdatePosition();

        static void OnXChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnYChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<winrt::DependencyProperty> XProperty =
            winrt::DependencyProperty::Register(
                L"X", 
                winrt::xaml_typename<double>(), 
                winrt::xaml_typename<class_type>(), 
                winrt::PropertyMetadata(winrt::box_value(0.0), &ImageCropperThumb::OnXChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> YProperty =
            winrt::DependencyProperty::Register(
                L"Y", 
                winrt::xaml_typename<double>(),
                winrt::xaml_typename<class_type>(), 
                winrt::PropertyMetadata(winrt::box_value(0.0), &ImageCropperThumb::OnYChanged));

        void Control_PointerEntered(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void Control_PointerExited(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void OnPointerPressed(winrt::PointerRoutedEventArgs const& e);

        void OnPointerReleased(winrt::PointerRoutedEventArgs const& e);

    private:
        winrt::TranslateTransform _layoutTransform;

        void ImageCropperThumb_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

        void Control_PointerCaptureLost(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);

        void Control_PointerCanceled(winrt::IInspectable const& sender, winrt::PointerRoutedEventArgs const& e);
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct ImageCropperThumb : ImageCropperThumbT<ImageCropperThumb, implementation::ImageCropperThumb>
    {
    };
}
