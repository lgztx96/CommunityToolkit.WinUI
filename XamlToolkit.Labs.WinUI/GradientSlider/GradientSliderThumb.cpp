#include "pch.h"
#include "winrt_module_imports.h"
#include "GradientSliderThumb.h"
#if __has_include("GradientSliderThumb.g.cpp")
#include "GradientSliderThumb.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    GradientSliderThumb::GradientSliderThumb()
        : _border(nullptr)
        , _colorPicker(nullptr)
        , _pointerOver(false)
        , _pressed(false)
        , _isDragging(false)
        , _dragStartPosition(0, 0)
        , _lastPosition(0, 0)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void GradientSliderThumb::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        _border = GetTemplateChild(BorderPartName).try_as<winrt::Border>();
        _colorPicker = GetTemplateChild(ColorPickerPartName).try_as<winrt::Microsoft::UI::Xaml::Controls::ColorPicker>();

        if (!_pointerEnteredToken)
        {
            _pointerEnteredToken = PointerEntered({ this, &GradientSliderThumb::GradientSliderThumb_PointerEntered });
            _pointerExitedToken = PointerExited({ this, &GradientSliderThumb::GradientSliderThumb_PointerExited });
            _pointerPressedToken = PointerPressed({ this, &GradientSliderThumb::GradientSliderThumb_PointerPressed });
            _pointerMovedToken = PointerMoved({ this, &GradientSliderThumb::GradientSliderThumb_PointerMoved });
            _pointerReleasedToken = PointerReleased({ this, &GradientSliderThumb::GradientSliderThumb_PointerReleased });
            _pointerCanceledToken = PointerCanceled({ this, &GradientSliderThumb::GradientSliderThumb_PointerCanceled });
            _isEnabledChangedToken = IsEnabledChanged({ this, &GradientSliderThumb::GradientSliderThumb_IsEnabledChanged });
            _tappedToken = Tapped({ this, &GradientSliderThumb::GradientSliderThumb_Tapped });
        }

        if (_colorPicker)
        {
            _colorPicker.Color(GradientStop().Color());
            _colorPicker.ColorChanged({ this, &GradientSliderThumb::ColorPicker_ColorChanged });
        }
    }

    void GradientSliderThumb::ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const&, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
    {
        GradientStop().Color(args.NewColor());
    }

    void GradientSliderThumb::GradientSliderThumb_Tapped(winrt::IInspectable const&, winrt::TappedRoutedEventArgs const&)
    {
        winrt::FlyoutBase::ShowAttachedFlyout(_border);
    }
}
