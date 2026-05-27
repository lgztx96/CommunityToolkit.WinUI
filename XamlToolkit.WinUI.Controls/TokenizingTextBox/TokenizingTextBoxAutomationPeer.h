#pragma once

#include "TokenizingTextBoxAutomationPeer.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Automation;
	using namespace Microsoft::UI::Xaml::Automation::Peers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct TokenizingTextBoxAutomationPeer : TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer>
	{
		TokenizingTextBoxAutomationPeer(winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox const& owner)
			: TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer>(owner)
		{
		}

	public:
		bool IsReadOnly() { return !OwningTokenizingTextBox().IsEnabled(); }

		hstring Value() { return OwningTokenizingTextBox().Text(); }

	private:
		winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox OwningTokenizingTextBox();

	public:
		void SetValue(winrt::hstring const& value);
		hstring GetClassNameCore();
		hstring GetNameCore();
		IInspectable GetPatternCore(PatternInterface patternInterface);
		IVector<AutomationPeer> GetChildrenCore();
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBoxAutomationPeer : TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer, implementation::TokenizingTextBoxAutomationPeer>
	{
	};
}
