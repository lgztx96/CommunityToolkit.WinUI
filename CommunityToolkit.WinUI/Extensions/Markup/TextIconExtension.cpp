#include "pch.h"
#include "TextIconExtension.h"
#if __has_include("TextIconExtension.g.cpp")
#include "TextIconExtension.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace winrt::Windows::Foundation;

	FontFamily TextIconExtension::SymbolThemeFontFamily()
	{
		if (!symbolThemeFontFamily)
		{
			symbolThemeFontFamily = FontFamily(L"Segoe Fluent Icons,Segoe MDL2 Assets");
		}

		return symbolThemeFontFamily;
	}

	IInspectable TextIconExtension::ProvideValue() { return nullptr; }

	IInspectable TextIconExtension::ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider
	) {
		return ProvideValue();
	}
}
