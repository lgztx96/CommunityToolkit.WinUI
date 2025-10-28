#include "pch.h"
#include "SymbolIconSourceExtension.h"
#if __has_include("SymbolIconSourceExtension.g.cpp")
#include "SymbolIconSourceExtension.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	IInspectable SymbolIconSourceExtension::ProvideValue() const
	{
		FontIconSource fontIcon;
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

	IInspectable SymbolIconSourceExtension::ProvideValue([[maybe_unused]] IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
