#pragma once

#include "OpacityMaskView.g.h"
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Xaml;

    struct OpacityMaskView : OpacityMaskViewT<OpacityMaskView>
    {
        OpacityMaskView();

        UIElement OpacityMask();

        void OpacityMask(UIElement const& value);

        static void OnOpacityMaskChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> OpacityMaskProperty = DependencyProperty::Register(
            L"OpacityMask",
            winrt::xaml_typename<UIElement>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr, &OpacityMaskView::OnOpacityMaskChanged));

        void OnApplyTemplate();

    private:

        static constexpr std::wstring_view ContentPresenterTemplateName = L"PART_ContentPresenter";
        static constexpr std::wstring_view MaskContainerTemplateName = L"PART_MaskContainer";
        static constexpr std::wstring_view RootGridTemplateName = L"PART_RootGrid";

        winrt::Microsoft::UI::Composition::Compositor _compositor{ nullptr };
        winrt::Microsoft::UI::Composition::CompositionBrush _mask{ nullptr };
        winrt::Microsoft::UI::Composition::CompositionMaskBrush _maskBrush{ nullptr };

        static winrt::Microsoft::UI::Composition::CompositionBrush GetVisualBrush(UIElement const& element);
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct OpacityMaskView : OpacityMaskViewT<OpacityMaskView, implementation::OpacityMaskView>
    {
    };
}
