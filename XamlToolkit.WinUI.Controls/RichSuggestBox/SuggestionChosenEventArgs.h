#pragma once

#include "SuggestionChosenEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Text.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Text;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct SuggestionChosenEventArgs : SuggestionChosenEventArgsT<SuggestionChosenEventArgs>, winrt::deferrable_event_args<SuggestionChosenEventArgs>
	{
		SuggestionChosenEventArgs() = default;

		wil::single_threaded_rw_property<winrt::hstring> QueryText;

		wil::single_threaded_rw_property<winrt::hstring> Prefix;

		wil::single_threaded_rw_property<winrt::hstring> DisplayText;

		wil::single_threaded_rw_property<IInspectable> SelectedItem{ nullptr };

		wil::single_threaded_rw_property<winrt::guid> Id;

		wil::single_threaded_rw_property<ITextCharacterFormat> Format;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct SuggestionChosenEventArgs : SuggestionChosenEventArgsT<SuggestionChosenEventArgs, implementation::SuggestionChosenEventArgs>
	{
	};
}
