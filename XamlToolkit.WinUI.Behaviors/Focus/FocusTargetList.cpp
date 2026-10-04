#include "pch.h"
#include "winrt_module_imports.h"
#include "FocusTargetList.h"
#if __has_include("FocusTargetList.g.cpp")
#include "FocusTargetList.g.cpp"
#endif
#include "../Diagnostics/ToolkitProfilerTracing.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
    FocusTargetList::FocusTargetList()
    {
        XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
    }

    FocusTargetList::~FocusTargetList()
    {
        XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
    }
}
