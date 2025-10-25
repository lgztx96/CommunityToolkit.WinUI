#include "pch.h"
#include "RadialGaugeAutomationPeer.h"
#if __has_include("RadialGaugeAutomationPeer.g.cpp")
#include "RadialGaugeAutomationPeer.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void RadialGaugeAutomationPeer::SetValue(double value)
	{
		Owner().as<owner_type>().Value(value);
	}

	IVector<AutomationPeer> RadialGaugeAutomationPeer::GetChildrenCore()
	{
		return nullptr;
	}

	winrt::hstring RadialGaugeAutomationPeer::GetNameCore()
	{
		auto gauge = Owner().as<owner_type>();
		auto unit = gauge.Unit();
		std::wstring result = L"radial gauge. ";
		if (unit.empty() || std::all_of(unit.begin(), unit.end(), iswspace))
		{
			result += L"no unit specified, ";
		}
		else
		{
			result += L"unit " + std::wstring(unit) + L", ";
		}
		result += std::to_wstring(Value());
		return winrt::hstring(result);
	}

	IInspectable RadialGaugeAutomationPeer::GetPatternCore(PatternInterface patternInterface)
	{
		if (patternInterface == PatternInterface::RangeValue)
		{
			// Expose RangeValue properties.
			return *this;
		}

		return base_type::GetPatternCore(patternInterface);
	}

	AutomationControlType RadialGaugeAutomationPeer::GetAutomationControlTypeCore()
	{
		return AutomationControlType::Custom;
	}

	void RadialGaugeAutomationPeer::RaiseValueChangedEvent(double oldValue, double newValue)
	{
		RaisePropertyChangedEvent(RangeValuePatternIdentifiers::ValueProperty(), PropertyValue::CreateDouble(oldValue), PropertyValue::CreateDouble(newValue));
	}
}
