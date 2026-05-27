#include "pch.h"
#include "winrt_module_imports.h"
#include "TextIconExtension.h"
#if __has_include("TextIconExtension.g.cpp")
#include "TextIconExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	FontFamily TextIconExtension::SymbolThemeFontFamily()
	{
		if (!symbolThemeFontFamily)
		{
			symbolThemeFontFamily = FontFamily(L"Segoe Fluent Icons,Segoe MDL2 Assets");
		}

		return symbolThemeFontFamily;
	}

	IInspectable TextIconExtension::ProvideValue() { return nullptr; }

	IInspectable TextIconExtension::ProvideValue([[maybe_unused]] IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
