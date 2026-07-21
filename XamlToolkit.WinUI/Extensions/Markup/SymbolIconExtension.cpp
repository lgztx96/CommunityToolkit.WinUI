#include "pch.h"
#include "winrt_module_imports.h"
#include "SymbolIconExtension.h"
#if __has_include("SymbolIconExtension.g.cpp")
#include "SymbolIconExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::IInspectable SymbolIconExtension::ProvideValue() const
    {
		winrt::FontIcon fontIcon;
		fontIcon.Glyph(winrt::hstring{ static_cast<wchar_t>(Symbol()) });
		fontIcon.FontFamily(TextIconExtension::SymbolThemeFontFamily());
		fontIcon.FontWeight(FontWeight());
		fontIcon.FontStyle(FontStyle());
		fontIcon.IsTextScaleFactorEnabled(IsTextScaleFactorEnabled());
		fontIcon.MirroredWhenRightToLeft(MirroredWhenRightToLeft());

		if (FontSize() > 0)
		{
			fontIcon.FontSize(FontSize());
		}

		if (Foreground())
		{
			fontIcon.Foreground(Foreground());
		}

        return fontIcon;
    }

	winrt::IInspectable SymbolIconExtension::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
