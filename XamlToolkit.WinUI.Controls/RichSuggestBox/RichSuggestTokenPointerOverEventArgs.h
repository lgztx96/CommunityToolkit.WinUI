#pragma once

#include "RichSuggestTokenPointerOverEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Text.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RichSuggestTokenPointerOverEventArgs : RichSuggestTokenPointerOverEventArgsT<RichSuggestTokenPointerOverEventArgs>
	{
		RichSuggestTokenPointerOverEventArgs() = default;

		RichSuggestTokenPointerOverEventArgs(
			winrt::XamlToolkit::WinUI::Controls::RichSuggestToken const& token,
			Microsoft::UI::Text::ITextRange const& range,
			Microsoft::UI::Input::PointerPoint const& point)
			: Token(token), Range(range), CurrentPoint(point) {
		}

		wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Controls::RichSuggestToken> Token{ nullptr };

		wil::single_threaded_rw_property<Microsoft::UI::Text::ITextRange> Range{ nullptr };

		wil::single_threaded_rw_property<Microsoft::UI::Input::PointerPoint> CurrentPoint{ nullptr };
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct RichSuggestTokenPointerOverEventArgs : RichSuggestTokenPointerOverEventArgsT<RichSuggestTokenPointerOverEventArgs, implementation::RichSuggestTokenPointerOverEventArgs>
	{
	};
}
