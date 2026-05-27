#include "pch.h"
#include "winrt_module_imports.h"
#include "RibbonGroup.h"
#if __has_include("RibbonGroup.g.cpp")
#include "RibbonGroup.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    RibbonGroup::RibbonGroup()
    {
        DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
    }

    void RibbonGroup::OnApplyTemplate() { base_type::OnApplyTemplate(); }
}
