#include "pch.h"
#include "winrt_module_imports.h"
#include "OpacityMaskView.h"
#if __has_include("OpacityMaskView.g.cpp")
#include "OpacityMaskView.g.cpp"
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Hosting;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    OpacityMaskView::OpacityMaskView()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    winrt::UIElement OpacityMaskView::OpacityMask() const 
    { 
        return GetValue(OpacityMaskProperty()).try_as<winrt::UIElement>();
    }

    const wil::single_threaded_property<winrt::DependencyProperty> OpacityMaskView::OpacityMaskProperty =
        winrt::DependencyProperty::Register(
            L"OpacityMask",
            winrt::xaml_typename<winrt::UIElement>(),
            winrt::xaml_typename<class_type>(),
            winrt::PropertyMetadata(nullptr, &OpacityMaskView::OnOpacityMaskChanged));

    void OpacityMaskView::OpacityMask(winrt::UIElement const& value) 
    { 
        SetValue(OpacityMaskProperty(), value); 
    }

    void OpacityMaskView::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        winrt::Grid rootGrid = GetTemplateChild(RootGridTemplateName).try_as<winrt::Grid>();
        winrt::ContentPresenter contentPresenter = GetTemplateChild(ContentPresenterTemplateName).try_as<winrt::ContentPresenter>();
        winrt::Border maskContainer = GetTemplateChild(MaskContainerTemplateName).try_as<winrt::Border>();

        if (!rootGrid || !contentPresenter || !maskContainer)
        {
            return;
		}

        _compositor = winrt::CompositionTarget::GetCompositorForCurrentThread();

        _maskBrush = _compositor.CreateMaskBrush();
        _maskBrush.Source(GetVisualBrush(contentPresenter));
        _mask = GetVisualBrush(maskContainer);
        _maskBrush.Mask(OpacityMask() ? _mask : nullptr);

        winrt::SpriteVisual redirectVisual = _compositor.CreateSpriteVisual();
        redirectVisual.RelativeSizeAdjustment(winrt::float2::one());
        redirectVisual.Brush(_maskBrush);
        winrt::ElementCompositionPreview::SetElementChildVisual(rootGrid, redirectVisual);
    }

    winrt::CompositionBrush OpacityMaskView::GetVisualBrush(winrt::UIElement const& element)
    {
        winrt::Visual visual = winrt::ElementCompositionPreview::GetElementVisual(element);

        winrt::Compositor compositor = visual.Compositor();

        winrt::CompositionVisualSurface visualSurface = compositor.CreateVisualSurface();
        visualSurface.SourceVisual(visual);
        winrt::ExpressionAnimation sourceSizeAnimation = compositor.CreateExpressionAnimation(L"visual.Size");
        sourceSizeAnimation.SetReferenceParameter(L"visual", visual);
        visualSurface.StartAnimation(L"SourceSize", sourceSizeAnimation);

        winrt::CompositionSurfaceBrush brush = compositor.CreateSurfaceBrush(visualSurface);

        visual.Opacity(0);

        return brush;
    }

    void OpacityMaskView::OnOpacityMaskChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        auto self = winrt::get_self<OpacityMaskView>(d.as<class_type>())->get_strong();
        if (const auto& maskBrush = self->_maskBrush)
        {
            winrt::UIElement opacityMask = e.NewValue().try_as<winrt::UIElement>();
            maskBrush.Mask(opacityMask ? self->_mask : nullptr);
        }
    }
}
