#include "pch.h"
#include "Shimmer.h"
#if __has_include("Shimmer.g.cpp")
#include "Shimmer.g.cpp"
#endif
#include <winrt/CommunityToolkit.WinUI.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    using namespace winrt::Windows::Foundation::Numerics;

    Shimmer::Shimmer() : _animationStarted(false), _initialized(false)
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        Loaded({ this, &Shimmer::OnLoaded });
        Unloaded({ this, &Shimmer::OnUnloaded });
    }

    void Shimmer::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        _shape = GetTemplateChild(PART_Shape).try_as<Border>();
        if (_initialized == false && TryInitializationResource() && IsActive())
        {
            TryStartAnimation();
        }
    }

    void Shimmer::OnLoaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        if (_initialized == false && TryInitializationResource() && IsActive())
        {
            TryStartAnimation();
        }

        _actualThemeChangedRevoker = ActualThemeChanged(winrt::auto_revoke, { this, &Shimmer::OnActualThemeChanged });
    }

    void Shimmer::OnUnloaded([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        _actualThemeChangedRevoker.revoke();
        StopAnimation();

        if (_initialized && _shape != nullptr)
        {
            ElementCompositionPreview::SetElementChildVisual(_shape, nullptr);

            _rectangleGeometry = nullptr;
            _shapeVisual = nullptr;
            _shimmerMaskGradient = nullptr;
            _gradientStop1 = nullptr;
            _gradientStop2 = nullptr;
            _gradientStop3 = nullptr;
            _gradientStop4 = nullptr;

            _initialized = false;
        }
    }

    void Shimmer::OnActualThemeChanged([[maybe_unused]] FrameworkElement const& sender, [[maybe_unused]] IInspectable const& args)
    {
        if (_initialized == false)
        {
            return;
        }

        SetGradientStopColorsByTheme();
    }

    bool Shimmer::TryInitializationResource()
    {
        if (_initialized)
        {
            return true;
        }

        if (_shape == nullptr || IsLoaded() == false)
        {
            return false;
        }

        auto compositor = CommunityToolkit::WinUI::VisualExtensions::GetVisual(_shape).Compositor();

        _rectangleGeometry = compositor.CreateRoundedRectangleGeometry();
        _shapeVisual = compositor.CreateShapeVisual();
        _shimmerMaskGradient = compositor.CreateLinearGradientBrush();
        _gradientStop1 = compositor.CreateColorGradientStop();
        _gradientStop2 = compositor.CreateColorGradientStop();
        _gradientStop3 = compositor.CreateColorGradientStop();
        _gradientStop4 = compositor.CreateColorGradientStop();
        SetGradientAndStops();
        SetGradientStopColorsByTheme();
        _rectangleGeometry.CornerRadius(float2(static_cast<float>(CornerRadius().TopLeft)));
        auto spriteShape = compositor.CreateSpriteShape(_rectangleGeometry);
        spriteShape.FillBrush(_shimmerMaskGradient);
        _shapeVisual.Shapes().Append(spriteShape);
        ElementCompositionPreview::SetElementChildVisual(_shape, _shapeVisual);

        _initialized = true;
        return true;
    }

    void Shimmer::SetGradientAndStops()
    {
        _shimmerMaskGradient.StartPoint(float2(InitialStartPointX, 0.0f));
        _shimmerMaskGradient.EndPoint(float2(0.0f, 1.0f)); //Vector2.One

        _gradientStop1.Offset(0.273f);
        _gradientStop2.Offset(0.436f);
        _gradientStop3.Offset(0.482f);
        _gradientStop4.Offset(0.643f);

        _shimmerMaskGradient.ColorStops().Append(_gradientStop1);
        _shimmerMaskGradient.ColorStops().Append(_gradientStop2);
        _shimmerMaskGradient.ColorStops().Append(_gradientStop3);
        _shimmerMaskGradient.ColorStops().Append(_gradientStop4);
    }

    void Shimmer::SetGradientStopColorsByTheme()
    {
        switch (ActualTheme())
        {
        case ElementTheme::Default:
        case ElementTheme::Dark:
            _gradientStop1.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 6.05 / 100), 255, 255, 255));
            _gradientStop2.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 3.26 / 100), 255, 255, 255));
            _gradientStop3.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 3.26 / 100), 255, 255, 255));
            _gradientStop4.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 6.05 / 100), 255, 255, 255));
            break;
        case ElementTheme::Light:
            _gradientStop1.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 5.37 / 100), 0, 0, 0));
            _gradientStop2.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 2.89 / 100), 0, 0, 0));
            _gradientStop3.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 2.89 / 100), 0, 0, 0));
            _gradientStop4.Color(Microsoft::UI::ColorHelper::FromArgb((uint8_t)(255 * 5.37 / 100), 0, 0, 0));
            break;
        }
    }

    void Shimmer::TryStartAnimation()
    {
        if (_animationStarted || _initialized == false || _shape == nullptr || _shapeVisual == nullptr || _rectangleGeometry == nullptr)
        {
            return;
        }

        auto rootVisual = CommunityToolkit::WinUI::VisualExtensions::GetVisual(_shape);
        _sizeAnimation = rootVisual.Compositor().CreateExpressionAnimation(L"rootVisual.Size");
        _sizeAnimation.SetReferenceParameter(L"rootVisual", rootVisual);

        _shapeVisual.StartAnimation(L"Size", _sizeAnimation);
        _rectangleGeometry.StartAnimation(L"Size", _sizeAnimation);

        _gradientStartPointAnimation = rootVisual.Compositor().CreateVector2KeyFrameAnimation();
        _gradientStartPointAnimation.Duration(Duration());
        _gradientStartPointAnimation.IterationBehavior(AnimationIterationBehavior::Forever);
        _gradientStartPointAnimation.InsertKeyFrame(0.0f, float2(InitialStartPointX, 0.0f));
        _gradientStartPointAnimation.InsertKeyFrame(1.0f, float2::zero());
        _shimmerMaskGradient.StartAnimation(L"StartPoint", _gradientStartPointAnimation);

        _gradientEndPointAnimation = rootVisual.Compositor().CreateVector2KeyFrameAnimation();
        _gradientEndPointAnimation.Duration(Duration());
        _gradientEndPointAnimation.IterationBehavior(AnimationIterationBehavior::Forever);
        _gradientEndPointAnimation.InsertKeyFrame(0.0f, float2(1.0f, 0.0f)); //Vector2.One
        _gradientEndPointAnimation.InsertKeyFrame(1.0f, float2(-InitialStartPointX, 1.0f));
        _shimmerMaskGradient.StartAnimation(L"EndPoint", _gradientEndPointAnimation);

        _animationStarted = true;
    }

    void Shimmer::StopAnimation()
    {
        if (_animationStarted == false)
        {
            return;
        }

        _shapeVisual.StopAnimation(L"Size");
        _rectangleGeometry.StopAnimation(L"Size");
        _shimmerMaskGradient.StopAnimation(L"StartPoint");
        _shimmerMaskGradient.StopAnimation(L"EndPoint");

        _sizeAnimation = nullptr;
        _gradientStartPointAnimation = nullptr;
        _gradientEndPointAnimation = nullptr;
        _animationStarted = false;
    }

    void Shimmer::PropertyChanged(DependencyObject const& s, [[maybe_unused]] DependencyPropertyChangedEventArgs e)
    {
        if (auto control = s.try_as<class_type>())
        {
            auto self = winrt::get_self<Shimmer>(control)->get_strong();
            if (self->IsActive())
            {
                self->StopAnimation();
                self->TryStartAnimation();
            }
            else
            {
                self->StopAnimation();
            }
        }
    }
}
