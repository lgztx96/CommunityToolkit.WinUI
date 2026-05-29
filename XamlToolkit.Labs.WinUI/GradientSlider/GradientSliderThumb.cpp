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

        _border = GetTemplateChild(BorderPartName).try_as<Border>();
        _colorPicker = GetTemplateChild(ColorPickerPartName).try_as<winrt::Microsoft::UI::Xaml::Controls::ColorPicker>();

        _pointerEnteredRevoker = PointerEntered(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerEntered });
        _pointerExitedRevoker = PointerExited(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerExited });
        _pointerPressedRevoker = PointerPressed(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerPressed });
        _pointerMovedRevoker = PointerMoved(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerMoved });
        _pointerReleasedRevoker = PointerReleased(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerReleased });
        _pointerCanceledRevoker = PointerCanceled(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_PointerCanceled });
        _isEnabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_IsEnabledChanged });
        _tappedRevoker = Tapped(winrt::auto_revoke, { this, &GradientSliderThumb::GradientSliderThumb_Tapped });

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

    void GradientSliderThumb::GradientSliderThumb_Tapped(IInspectable const&, TappedRoutedEventArgs const&)
    {
        FlyoutBase::ShowAttachedFlyout(_border);
    }
}
