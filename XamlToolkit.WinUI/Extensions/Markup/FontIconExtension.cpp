#include "pch.h"
#include "winrt_module_imports.h"
#include "FontIconExtension.h"
#if __has_include("FontIconExtension.g.cpp")
#include "FontIconExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
	winrt::IInspectable FontIconExtension::ProvideValue() const
	{
		if (Glyph().empty()) {
			throw hresult_invalid_argument(
				L"Glyph property must be set on FontIconExtension before calling ProvideValue.");
		}

		winrt::FontIcon fontIcon;
		fontIcon.Glyph(Glyph());
		fontIcon.FontFamily(FontFamily() ? FontFamily() : TextIconExtension::SymbolThemeFontFamily());
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

	winrt::IInspectable FontIconExtension::ProvideValue([[maybe_unused]] winrt::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
