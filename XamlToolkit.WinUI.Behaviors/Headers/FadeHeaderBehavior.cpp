#include "pch.h"
#include "winrt_module_imports.h"
#include "FadeHeaderBehavior.h"
#include "../Helper.h"
#if __has_include("FadeHeaderBehavior.g.cpp")
#include "FadeHeaderBehavior.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Behaviors::implementation
{
	bool FadeHeaderBehavior::AssignAnimation()
	{
		if (HeaderBehaviorBase<FadeHeaderBehavior>::AssignAnimation())
		{
			// Get the ScrollViewer's ManipulationPropertySet
			auto compositor = _scrollProperties.Compositor();
			float headerHeight = AssociatedObject().RenderSize().Height;

			// Use the ScrollViewer's Y offset and the header's height to calculate the opacity percentage. Clamp it between 0% and 100%
			auto opacityExpression = compositor.CreateExpressionAnimation(L"clamp(1 - (-scrollProps.Translation.Y / headerHeight), 0, 1)");
			opacityExpression.SetReferenceParameter(L"scrollProps", _scrollProperties);
			opacityExpression.SetScalarParameter(L"headerHeight", headerHeight);

			// Begin animating
			_headerVisual.StartAnimation(L"Opacity", opacityExpression);

			return true;
		}

		return false;
	}

	void FadeHeaderBehavior::StopAnimation()
	{
		if (_headerVisual)
		{
			_headerVisual.StopAnimation(L"Opacity");
			_headerVisual.Opacity(1.0f);
		}
	}
}
