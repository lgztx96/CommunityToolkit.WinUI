#pragma once

#include "OpacityMaskView.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Composition;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
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

        Compositor _compositor{ nullptr };
        CompositionBrush _mask{ nullptr };
        CompositionMaskBrush _maskBrush{ nullptr };

        static CompositionBrush GetVisualBrush(UIElement const& element);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct OpacityMaskView : OpacityMaskViewT<OpacityMaskView, implementation::OpacityMaskView>
    {
    };
}
