#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <vector>
#endif
#include "TokenizingTextBoxAutomationPeer.h"
#if __has_include("TokenizingTextBoxAutomationPeer.g.cpp")
#include "TokenizingTextBoxAutomationPeer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox TokenizingTextBoxAutomationPeer::OwningTokenizingTextBox() const
	{
		return Owner().try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox>();
	}

	void TokenizingTextBoxAutomationPeer::SetValue(winrt::hstring const& value)
	{
		if (IsReadOnly())
		{
			throw winrt::hresult_access_denied(winrt::format(L"Could not set the value of the {} ", L"TokenizingTextBox"));
		}

		OwningTokenizingTextBox().Text(value);
	}

	winrt::hstring TokenizingTextBoxAutomationPeer::GetClassNameCore()
	{
		return winrt::get_class_name(Owner());
	}

	winrt::hstring TokenizingTextBoxAutomationPeer::GetNameCore()
	{
		winrt::hstring name = OwningTokenizingTextBox().Name();
		if (!name.empty())
		{
			return name;
		}

		name = winrt::AutomationProperties::GetName(OwningTokenizingTextBox());
		return !name.empty() ? name : base_type::GetNameCore();
	}

	winrt::IInspectable TokenizingTextBoxAutomationPeer::GetPatternCore(winrt::PatternInterface patternInterface)
	{
		if (patternInterface == winrt::PatternInterface::Value)
		{
			return *this;
		}

		return base_type::GetPatternCore(patternInterface);
	}

	winrt::IVector<winrt::AutomationPeer> TokenizingTextBoxAutomationPeer::GetChildrenCore()
	{
		winrt::XamlToolkit::WinUI::Controls::TokenizingTextBox owner = OwningTokenizingTextBox();

		winrt::ItemCollection items = owner.Items();
		if (items.Size() == 0)
		{
			return { nullptr };
		}

		std::vector<winrt::AutomationPeer> peers;
		peers.reserve(items.Size());
		for (uint32_t i = 0; i < items.Size(); i++)
		{
			if (auto element = owner.ContainerFromIndex(i).try_as<winrt::XamlToolkit::WinUI::Controls::TokenizingTextBoxItem>())
			{
				auto automationPeer = winrt::FrameworkElementAutomationPeer::FromElement(element);
				peers.emplace_back(automationPeer ? automationPeer : winrt::FrameworkElementAutomationPeer::CreatePeerForElement(element));
			}
		}

		return winrt::single_threaded_vector(std::move(peers));
	}
}
