#include "pch.h"
#include "winrt_module_imports.h"
#include "AdornerDecorator.h"
#if __has_include("AdornerDecorator.g.cpp")
#include "AdornerDecorator.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    AdornerDecorator::AdornerDecorator()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void AdornerDecorator::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        AdornerLayer = GetTemplateChild(PartAdornerLayer).try_as<winrt::XamlToolkit::Labs::WinUI::AdornerLayer>();
    }
}
