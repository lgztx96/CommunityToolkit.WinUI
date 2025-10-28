#pragma once

#include "TokenizingTextBoxAutomationPeer.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct TokenizingTextBoxAutomationPeer : TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer>
	{
		TokenizingTextBoxAutomationPeer(winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox const& owner)
			: TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer>(owner)
		{
		}

	public:
		bool IsReadOnly() { return !OwningTokenizingTextBox().IsEnabled(); }

		hstring Value() { return OwningTokenizingTextBox().Text(); }

	private:
		winrt::CommunityToolkit::WinUI::Controls::TokenizingTextBox OwningTokenizingTextBox();

	public:
		void SetValue(winrt::hstring const& value);
		hstring GetClassNameCore();
		hstring GetNameCore();
		IInspectable GetPatternCore(PatternInterface patternInterface);
		IVector<AutomationPeer> GetChildrenCore();
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct TokenizingTextBoxAutomationPeer : TokenizingTextBoxAutomationPeerT<TokenizingTextBoxAutomationPeer, implementation::TokenizingTextBoxAutomationPeer>
	{
	};
}
