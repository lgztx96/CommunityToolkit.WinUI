#pragma once

#include "RichSuggestToken.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Text.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct RichSuggestToken : RichSuggestTokenT<RichSuggestToken>, wil::notify_property_changed_base<RichSuggestToken>
	{
		RichSuggestToken() = default;

	private:
		winrt::Microsoft::UI::Text::ITextRange _range{ nullptr };

	public:
		wil::single_threaded_property<winrt::guid> Id;

		wil::single_threaded_property<winrt::hstring> DisplayText;

		wil::single_threaded_rw_property<winrt::Windows::Foundation::IInspectable> Item{ nullptr };

		wil::single_threaded_property<int> RangeStart;

		wil::single_threaded_property<int> RangeEnd;

		int Position() const;

		wil::single_threaded_rw_property<bool> Active;

		RichSuggestToken(winrt::guid const& id, winrt::hstring const& displayText) : Id(id), DisplayText(displayText) {}

		void UpdateTextRange(winrt::Microsoft::UI::Text::ITextRange const& range);

		winrt::hstring ToString() const;
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct RichSuggestToken : RichSuggestTokenT<RichSuggestToken, implementation::RichSuggestToken>
	{
	};
}
