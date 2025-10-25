#pragma once

#include "TextIconExtension.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Text.h>

namespace winrt::CommunityToolkit::WinUI::implementation
{
	using namespace winrt::Windows::UI::Text;
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Microsoft::UI::Text;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Media;

	struct TextIconExtension : TextIconExtensionT<TextIconExtension>
	{
	private:
		static inline thread_local FontFamily symbolThemeFontFamily{ nullptr };

	public:
		TextIconExtension() = default;

		wil::single_threaded_rw_property<double> FontSize;

		static FontFamily SymbolThemeFontFamily();

		wil::single_threaded_rw_property<FontWeight> FontWeight = Microsoft::UI::Text::FontWeights::Normal();

		wil::single_threaded_rw_property<FontStyle> FontStyle = Windows::UI::Text::FontStyle::Normal;

		wil::single_threaded_rw_property<Brush> Foreground{ nullptr };

		wil::single_threaded_rw_property<bool> IsTextScaleFactorEnabled;

		wil::single_threaded_rw_property<bool> MirroredWhenRightToLeft;

		virtual IInspectable ProvideValue();

		virtual IInspectable ProvideValue(IXamlServiceProvider const& provider);
	};
}

namespace winrt::CommunityToolkit::WinUI::factory_implementation
{
	struct TextIconExtension : TextIconExtensionT<TextIconExtension, implementation::TextIconExtension>
	{
	};
}
