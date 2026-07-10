#pragma once

#include "TextIconExtension.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Windows.UI.Text.h>
#endif

namespace winrt
{
	using namespace Windows::UI::Text;
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Text;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::implementation
{
	struct TextIconExtension : TextIconExtensionT<TextIconExtension>
	{
	private:
		static inline thread_local winrt::FontFamily symbolThemeFontFamily{ nullptr };

	public:
		TextIconExtension() = default;

		wil::single_threaded_rw_property<double> FontSize;

		static winrt::FontFamily SymbolThemeFontFamily();

		wil::single_threaded_rw_property<winrt::FontWeight> FontWeight = Microsoft::UI::Text::FontWeights::Normal();

		wil::single_threaded_rw_property<winrt::FontStyle> FontStyle = Windows::UI::Text::FontStyle::Normal;

		wil::single_threaded_rw_property<winrt::Brush> Foreground{ nullptr };

		wil::single_threaded_rw_property<bool> IsTextScaleFactorEnabled;

		wil::single_threaded_rw_property<bool> MirroredWhenRightToLeft;

		virtual winrt::IInspectable ProvideValue();

		virtual winrt::IInspectable ProvideValue(winrt::IXamlServiceProvider const& provider);
	};
}

namespace winrt::XamlToolkit::WinUI::factory_implementation
{
	struct TextIconExtension : TextIconExtensionT<TextIconExtension, implementation::TextIconExtension>
	{
	};
}
