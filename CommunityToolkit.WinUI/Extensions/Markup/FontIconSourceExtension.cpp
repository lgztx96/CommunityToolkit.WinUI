#include "pch.h"
#include "FontIconSourceExtension.h"
#if __has_include("FontIconSourceExtension.g.cpp")
#include "FontIconSourceExtension.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	winrt::Windows::Foundation::IInspectable FontIconSourceExtension::ProvideValue() const
	{
		if (Glyph().empty()) {
			throw hresult_invalid_argument(
				L"Glyph property must be set on FontIconSourceExtension before calling ProvideValue.");
		}

		FontIconSource fontIcon;
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

	IInspectable FontIconSourceExtension::ProvideValue([[maybe_unused]] IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
