#include "pch.h"
#include "winrt_module_imports.h"
#include "VisualExtensions.h"
#if __has_include("VisualExtensions.g.cpp")
#include "VisualExtensions.g.cpp"
#endif
#include "../Text/StringExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
    Visual VisualExtensions::GetVisual(winrt::UIElement const& element)
    {
        return winrt::ElementCompositionPreview::GetElementVisual(element);
    }

    winrt::hstring VisualExtensions::GetAnchorPoint(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetAnchorPointForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(AnchorPointProperty));
    }

    void VisualExtensions::SetAnchorPoint(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetAnchorPointForElement(value, element);
        }

        obj.SetValue(AnchorPointProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetCenterPoint(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetCenterPointForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(CenterPointProperty));
    }

    void VisualExtensions::SetCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetCenterPointForElement(value, element);
        }

        obj.SetValue(CenterPointProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetOffset(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetOffsetForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(OffsetProperty));
    }

    void VisualExtensions::SetOffset(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetOffsetForElement(value, element);
        }

        obj.SetValue(OffsetProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetTranslation(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetTranslationForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(TranslationProperty));
    }

    void VisualExtensions::SetTranslation(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetTranslationForElement(value, element);
        }

        obj.SetValue(TranslationProperty, winrt::box_value(value));
    }

    double VisualExtensions::GetOpacity(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetOpacityForElement(element);
        }

        return winrt::unbox_value<double>(obj.GetValue(OpacityProperty));
    }

    void VisualExtensions::SetOpacity(winrt::DependencyObject const& obj, double value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetOpacityForElement(value, element);
        }

        obj.SetValue(OpacityProperty, winrt::box_value(value));
    }

    double VisualExtensions::GetRotationAngle(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetRotationAngleForElement(element);
        }

        return winrt::unbox_value<double>(obj.GetValue(RotationAngleProperty));
    }

    void VisualExtensions::SetRotationAngle(winrt::DependencyObject const& obj, double value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetRotationAngleForElement(value, element);
        }

        obj.SetValue(RotationAngleProperty, winrt::box_value(value));
    }

    double VisualExtensions::GetRotationAngleInDegrees(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetRotationAngleInDegreesForElement(element);
        }

        return winrt::unbox_value<double>(obj.GetValue(RotationAngleInDegreesProperty));
    }

    void VisualExtensions::SetRotationAngleInDegrees(winrt::DependencyObject const& obj, double value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetRotationAngleInDegreesForElement(value, element);
        }

        obj.SetValue(RotationAngleInDegreesProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetRotationAxis(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetRotationAxisForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(RotationAxisProperty));
    }

    void VisualExtensions::SetRotationAxis(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetRotationAxisForElement(value, element);
        }

        obj.SetValue(RotationAxisProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetScale(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetScaleForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(ScaleProperty));
    }

    void VisualExtensions::SetScale(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetScaleForElement(value, element);
        }

        obj.SetValue(ScaleProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetSize(winrt::DependencyObject const& obj)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            return GetSizeForElement(element);
        }

        return winrt::unbox_value<winrt::hstring>(obj.GetValue(SizeProperty));
    }

    void VisualExtensions::SetSize(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        if (auto element = obj.try_as<winrt::UIElement>())
        {
            SetSizeForElement(value, element);
        }

        obj.SetValue(SizeProperty, winrt::box_value(value));
    }

    winrt::hstring VisualExtensions::GetNormalizedCenterPoint(winrt::DependencyObject const& obj)
    {
        return winrt::unbox_value<winrt::hstring>(obj.GetValue(NormalizedCenterPointProperty));
    }

    void VisualExtensions::SetNormalizedCenterPoint(winrt::DependencyObject const& obj, winrt::hstring const& value)
    {
        obj.SetValue(NormalizedCenterPointProperty, winrt::box_value(value));
    }

    void VisualExtensions::OnAnchorPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetAnchorPoint(d, *str);
        }
    }

    void VisualExtensions::OnCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetCenterPoint(d, *str);
        }
    }

    void VisualExtensions::OnOffsetChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetOffset(d, *str);
        }
    }

    void VisualExtensions::OnTranslationChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetTranslation(d, *str);
        }
    }

    void VisualExtensions::OnOpacityChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto dbl = e.NewValue().try_as<double>())
        {
            SetOpacity(d, *dbl);
        }
    }

    void VisualExtensions::OnRotationAngleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto dbl = e.NewValue().try_as<double>())
        {
            SetRotationAngle(d, *dbl);
        }
    }

    void VisualExtensions::OnRotationAngleInDegreesChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto dbl = e.NewValue().try_as<double>())
        {
            SetRotationAngleInDegrees(d, *dbl);
        }
    }

    void VisualExtensions::OnRotationAxisChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetRotationAxis(d, *str);
        }
    }

    void VisualExtensions::OnScaleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetScale(d, *str);
        }
    }

    void VisualExtensions::OnSizeChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        if (auto str = e.NewValue().try_as<winrt::hstring>())
        {
            SetSize(d, *str);
        }
    }

    void VisualExtensions::OnNormalizedCenterPointChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto element = d.try_as<winrt::FrameworkElement>();
        auto newValue = e.NewValue().try_as<winrt::hstring>();
        if (element && newValue)
        {
            winrt::float2 center = ToVector2(*newValue);
            winrt::Visual visual = GetVisual(element);
            const winrt::hstring expression = L"Vector2(this.Target.Size.X * X, this.Target.Size.Y * Y)";
            winrt::ExpressionAnimation animation = visual.Compositor().CreateExpressionAnimation(expression);

            animation.SetScalarParameter(L"X", center.x);
            animation.SetScalarParameter(L"Y", center.y);

            visual.StopAnimation(L"CenterPoint.XY");
            visual.StartAnimation(L"CenterPoint.XY", animation);
        }
    }

    winrt::hstring VisualExtensions::GetAnchorPointForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.AnchorPoint());
    }

    void VisualExtensions::SetAnchorPointForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.AnchorPoint(ToVector2(value));
    }

    winrt::hstring VisualExtensions::GetCenterPointForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.CenterPoint());
    }

    void VisualExtensions::SetCenterPointForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.CenterPoint(ToVector3(value));
    }

    winrt::hstring VisualExtensions::GetOffsetForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.Offset());
    }

    void VisualExtensions::SetOffsetForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.Offset(ToVector3(value));
    }

    winrt::hstring VisualExtensions::GetTranslationForElement(winrt::UIElement const& element)
    {
        float3 translation;
        CompositionGetValueStatus result = GetVisual(element).Properties().TryGetVector3(L"Translation", translation);

        if (result == CompositionGetValueStatus::Succeeded) 
        {
            // The ("G", CultureInfo.InvariantCulture) combination produces a string with the default numeric
           // formatting style, and using ',' as component separator, so that the resulting text can safely
           // be parsed back if needed with the StringExtensions.ToVector3(string) extension, which uses
           // the invariant culture mode by default so that the syntax will always match that from XAML.
            return ToString(translation);
        }

        return L"<0, 0, 0>";
    }

    void VisualExtensions::SetTranslationForElement(winrt::hstring const& value, UIElement const& element)
    {
        winrt::ElementCompositionPreview::SetIsTranslationEnabled(element, true);

        // The "Translation" attached property refers to the "hidden" property that is enabled
        // through "ElementCompositionPreview.SetIsTranslationEnabled". The value for this property
        // is not available directly on the Visual class and can only be accessed through its property
        // set. Note that this "Translation" value is not the same as Visual.TransformMatrix.Translation.
        // In fact, the latter doesn't require to be explicitly enabled and is actually combined with
        // this at runtime (ie. the whole transform matrix is combined with the additional translation
        // from the "Translation" property, if any), and the two can be set and animated independently.
        // In this case we're just interested in the "Translation" property, which is more commonly used
        // as it can also be animated directly with a Vector3 animation instead of a Matrix4x4 one.
        GetVisual(element).Properties().InsertVector3(L"Translation", ToVector3(value));
    }

    double VisualExtensions::GetOpacityForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return visual.Opacity();
    }

    void VisualExtensions::SetOpacityForElement(double value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.Opacity((float)value);
    }

    double VisualExtensions::GetRotationAngleForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return visual.RotationAngle();
    }

    void VisualExtensions::SetRotationAngleForElement(double value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.RotationAngle((float)value);
    }

    double VisualExtensions::GetRotationAngleInDegreesForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return visual.RotationAngleInDegrees();
    }

    void VisualExtensions::SetRotationAngleInDegreesForElement(double value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.RotationAngleInDegrees((float)value);
    }

    winrt::hstring VisualExtensions::GetRotationAxisForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.RotationAxis());
    }

    void VisualExtensions::SetRotationAxisForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.RotationAxis(ToVector3(value));
    }

    winrt::hstring VisualExtensions::GetScaleForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.Scale());
    }

    void VisualExtensions::SetScaleForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.Scale(ToVector3(value));
    }

    winrt::hstring VisualExtensions::GetSizeForElement(winrt::UIElement const& element)
    {
        auto visual = GetVisual(element);
        return ToString(visual.Size());
    }

    void VisualExtensions::SetSizeForElement(winrt::hstring const& value, UIElement const& element)
    {
        auto visual = GetVisual(element);
        visual.Size(ToVector2(value));
    }
}
