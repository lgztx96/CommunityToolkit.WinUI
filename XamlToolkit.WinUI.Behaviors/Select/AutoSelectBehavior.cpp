#include "pch.h"
#include "winrt_module_imports.h"
#include "AutoSelectBehavior.h"
#if __has_include("AutoSelectBehavior.g.cpp")
#include "AutoSelectBehavior.g.cpp"
#endif
#include "../Diagnostics/ToolkitProfilerTracing.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
	AutoSelectBehavior::AutoSelectBehavior()
	{
		XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
	}

	AutoSelectBehavior::~AutoSelectBehavior()
	{
		XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
	}

    void AutoSelectBehavior::OnAssociatedObjectLoaded()
    {
		const auto associatedObject = BehaviorBase::AssociatedObject();
        associatedObject.Focus(winrt::FocusState::Programmatic);
        associatedObject.SelectAll();
    }
}
