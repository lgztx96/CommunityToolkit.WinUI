#include "pch.h"
#include "OpacityMaskView.h"
#if __has_include("OpacityMaskView.g.cpp")
#include "OpacityMaskView.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Composition;
using namespace Microsoft::UI::Xaml::Hosting;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    OpacityMaskView::OpacityMaskView()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    UIElement OpacityMaskView::OpacityMask() { return winrt::unbox_value<UIElement>(GetValue(OpacityMaskProperty)); }
    void OpacityMaskView::OpacityMask(UIElement const& value) { SetValue(OpacityMaskProperty, value); }

    void OpacityMaskView::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        Grid rootGrid = GetTemplateChild(RootGridTemplateName).try_as<Grid>();
        ContentPresenter contentPresenter = GetTemplateChild(ContentPresenterTemplateName).try_as<ContentPresenter>();
        Border maskContainer = GetTemplateChild(MaskContainerTemplateName).try_as<Border>();

        if (!rootGrid || !contentPresenter || !maskContainer)
        {
            return;
		}

        _compositor = Microsoft::UI::Xaml::Media::CompositionTarget::GetCompositorForCurrentThread();

        _maskBrush = _compositor.CreateMaskBrush();
        _maskBrush.Source(GetVisualBrush(contentPresenter));
        _mask = GetVisualBrush(maskContainer);
        _maskBrush.Mask(OpacityMask() ? _mask : nullptr);

        SpriteVisual redirectVisual = _compositor.CreateSpriteVisual();
        redirectVisual.RelativeSizeAdjustment(winrt::Windows::Foundation::Numerics::float2::one());
        redirectVisual.Brush(_maskBrush);
        ElementCompositionPreview::SetElementChildVisual(rootGrid, redirectVisual);
    }

    CompositionBrush OpacityMaskView::GetVisualBrush(UIElement const& element)
    {
        Visual visual = ElementCompositionPreview::GetElementVisual(element);

        Compositor compositor = visual.Compositor();

        CompositionVisualSurface visualSurface = compositor.CreateVisualSurface();
        visualSurface.SourceVisual(visual);
        ExpressionAnimation sourceSizeAnimation = compositor.CreateExpressionAnimation(L"visual.Size");
        sourceSizeAnimation.SetReferenceParameter(L"visual", visual);
        visualSurface.StartAnimation(L"SourceSize", sourceSizeAnimation);

        CompositionSurfaceBrush brush = compositor.CreateSurfaceBrush(visualSurface);

        visual.Opacity(0);

        return brush;
    }

    void OpacityMaskView::OnOpacityMaskChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        auto self = winrt::get_self<OpacityMaskView>(d.as<class_type>())->get_strong();
        if (const auto& maskBrush = self->_maskBrush)
        {
            UIElement opacityMask = e.NewValue().try_as<UIElement>();
            maskBrush.Mask(opacityMask ? self->_mask : nullptr);
        }
    }
}
