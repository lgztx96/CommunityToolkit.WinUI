#include "pch.h"
#include "SymbolIconExtension.h"
#if __has_include("SymbolIconExtension.g.cpp")
#include "SymbolIconExtension.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
    IInspectable SymbolIconExtension::ProvideValue() const
    {
        FontIcon fontIcon;
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

	IInspectable SymbolIconExtension::ProvideValue([[maybe_unused]] IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
