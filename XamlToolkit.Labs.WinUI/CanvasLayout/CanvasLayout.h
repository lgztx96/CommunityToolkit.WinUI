#pragma once

#include "CanvasLayout.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct CanvasLayout : CanvasLayoutT<CanvasLayout>
	{
		CanvasLayout() = default;

#pragma region Layout
		winrt::Size MeasureOverride(winrt::VirtualizingLayoutContext const& context, winrt::Size availableSize);

		winrt::Size ArrangeOverride(winrt::VirtualizingLayoutContext const& context, winrt::Size finalSize);
#pragma endregion
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct CanvasLayout : CanvasLayoutT<CanvasLayout, implementation::CanvasLayout>
	{
	};
}
