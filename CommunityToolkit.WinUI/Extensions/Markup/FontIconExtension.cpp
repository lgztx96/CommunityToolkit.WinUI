#include "pch.h"
#include "FontIconExtension.h"
#if __has_include("FontIconExtension.g.cpp")
#include "FontIconExtension.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
	winrt::Windows::Foundation::IInspectable FontIconExtension::ProvideValue()
	{
		if (Glyph().empty()) {
			throw hresult_invalid_argument(
				L"Glyph property must be set on FontIconExtension before calling ProvideValue.");
		}

		FontIcon fontIcon;
		fontIcon.Glyph(Glyph());
		if (!FontFamily())
		{
			FontFamily(TextIconExtension::SymbolThemeFontFamily());
		}
		fontIcon.FontFamily(FontFamily());
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

	winrt::Windows::Foundation::IInspectable FontIconExtension::ProvideValue([[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}
