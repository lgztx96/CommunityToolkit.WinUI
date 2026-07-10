#pragma once

#include "OpacityMaskView.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Composition;
    using namespace Windows::Foundation::Numerics;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct OpacityMaskView : OpacityMaskViewT<OpacityMaskView>
    {
        OpacityMaskView();

        winrt::UIElement OpacityMask() const;

        void OpacityMask(winrt::UIElement const& value);

        static void OnOpacityMaskChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static const wil::single_threaded_property<winrt::DependencyProperty> OpacityMaskProperty;

        void OnApplyTemplate();

    private:
        static constexpr std::wstring_view ContentPresenterTemplateName = L"PART_ContentPresenter";
        static constexpr std::wstring_view MaskContainerTemplateName = L"PART_MaskContainer";
        static constexpr std::wstring_view RootGridTemplateName = L"PART_RootGrid";

        winrt::Compositor _compositor{ nullptr };
        winrt::CompositionBrush _mask{ nullptr };
        winrt::CompositionMaskBrush _maskBrush{ nullptr };

        static winrt::CompositionBrush GetVisualBrush(winrt::UIElement const& element);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct OpacityMaskView : OpacityMaskViewT<OpacityMaskView, implementation::OpacityMaskView>
    {
    };
}
