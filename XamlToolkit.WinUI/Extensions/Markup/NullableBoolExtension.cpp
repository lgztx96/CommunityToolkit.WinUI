#include "pch.h"
#include "winrt_module_imports.h"
#include "NullableBoolExtension.h"
#if __has_include("NullableBoolExtension.g.cpp")
#include "NullableBoolExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::IInspectable NullableBoolExtension::ProvideValue() const
	{
		if (IsNull())
		{
			return nullptr;
		}

		return winrt::box_value(Value());
	}

	winrt::IInspectable NullableBoolExtension::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider) const
	{
		return ProvideValue();
	}
}
