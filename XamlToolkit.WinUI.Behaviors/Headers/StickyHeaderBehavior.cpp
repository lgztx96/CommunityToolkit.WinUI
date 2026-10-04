#include "pch.h"
#include "winrt_module_imports.h"
#include "StickyHeaderBehavior.h"
#include "../Helper.h"
#if __has_include("StickyHeaderBehavior.g.cpp")
#include "StickyHeaderBehavior.g.cpp"
#endif
#include "../Diagnostics/ToolkitProfilerTracing.h"

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
	StickyHeaderBehavior::StickyHeaderBehavior()
	{
		XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
	}

	StickyHeaderBehavior::~StickyHeaderBehavior()
	{
		XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
	}

	void StickyHeaderBehavior::Show()
	{
		if (_headerVisual && _scrollViewer && _animationProperties)
		{
			_animationProperties.InsertScalar(L"OffsetY", 0.0f);
		}
	}

	bool StickyHeaderBehavior::AssignAnimation()
	{
		if (HeaderBehaviorBase::AssignAnimation())
		{
			_animationProperties.InsertScalar(L"OffsetY", 0.0f);

			auto compositor = _animationProperties.Compositor();
			auto expressionAnimation = compositor.CreateExpressionAnimation(L"max(animationProps.OffsetY - scrollProps.Translation.Y, 0)");
			expressionAnimation.SetReferenceParameter(L"animationProps", _animationProperties);
			expressionAnimation.SetReferenceParameter(L"scrollProps", _scrollProperties);

			_headerVisual.StartAnimation(L"Offset.Y", expressionAnimation);

			return true;
		}

		return false;
	}

	void StickyHeaderBehavior::StopAnimation()
	{
		if (_animationProperties)
		{
			_animationProperties.InsertScalar(L"OffsetY", 0.0f);
		}

		if (_headerVisual)
		{
			_headerVisual.StopAnimation(L"Offset.Y");

			auto offset = _headerVisual.Offset();
			offset.y = 0.0f;
			_headerVisual.Offset(offset);
		}
	}
}
