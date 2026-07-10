#pragma once

#include "MarkdownThemes.g.h"
#include "Extension.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Windows::UI::Text;
    using namespace Microsoft::UI;
    using namespace Microsoft::UI::Text;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    using FontWeights = winrt::Microsoft::UI::Text::FontWeights;

    struct MarkdownThemes : MarkdownThemesT<MarkdownThemes>
    {
        MarkdownThemes() = default;

        static XamlToolkit::Labs::WinUI::MarkdownThemes Default();

        wil::single_threaded_rw_property<winrt::Thickness> Padding = winrt::Thickness(8, 8, 8, 8);

        wil::single_threaded_rw_property<winrt::Thickness> InternalMargin = winrt::Thickness(4, 4, 4, 4);

        wil::single_threaded_rw_property<winrt::CornerRadius> CornerRadius = winrt::CornerRadius(4, 4, 4, 4);

        wil::single_threaded_rw_property<double> H1FontSize = 22;

        wil::single_threaded_rw_property<double> H2FontSize = 20;

        wil::single_threaded_rw_property<double> H3FontSize = 18;

        wil::single_threaded_rw_property<double> H4FontSize = 16;

        wil::single_threaded_rw_property<double> H5FontSize = 14;

        wil::single_threaded_rw_property<double> H6FontSize = 12;

        wil::single_threaded_rw_property<winrt::Brush> H1Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> H2Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> H3Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> H4Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> H5Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> H6Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();

        wil::single_threaded_rw_property<winrt::FontWeight> H1FontWeight = FontWeights::Bold();

        wil::single_threaded_rw_property<winrt::FontWeight> H2FontWeight = FontWeights::SemiBold();

        wil::single_threaded_rw_property<winrt::FontWeight> H3FontWeight = FontWeights::SemiBold();

        wil::single_threaded_rw_property<winrt::FontWeight> H4FontWeight = FontWeights::SemiBold();

        wil::single_threaded_rw_property<winrt::FontWeight> H5FontWeight = FontWeights::SemiBold();

        wil::single_threaded_rw_property<winrt::FontWeight> H6FontWeight = FontWeights::SemiBold();

        wil::single_threaded_rw_property<winrt::Thickness> H1Margin = winrt::Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<winrt::Thickness> H2Margin = winrt::Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<winrt::Thickness> H3Margin = winrt::Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<winrt::Thickness> H4Margin = winrt::Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<winrt::Thickness> H5Margin = winrt::Thickness(0, 8, 0, 0);
        wil::single_threaded_rw_property<winrt::Thickness> H6Margin = winrt::Thickness(0, 8, 0, 0);

        wil::single_threaded_rw_property<winrt::Brush> BorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<Brush>();

        wil::single_threaded_rw_property<winrt::Brush> TableHeadingBackground = Application::Current().Resources().Lookup(winrt::box_value(L"CardBackgroundFillColorDefaultBrush")).as<Brush>();

        wil::single_threaded_rw_property<winrt::Brush> InlineCodeBackground = Application::Current().Resources().Lookup(winrt::box_value(L"CardBackgroundFillColorSecondaryBrush")).as<Brush>();

        wil::single_threaded_rw_property<winrt::Brush> InlineCodeForeground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();

        wil::single_threaded_rw_property<winrt::Brush> InlineCodeBorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<Brush>();

        wil::single_threaded_rw_property<winrt::Thickness> InlineCodeBorderThickness = winrt::Thickness(1, 1, 1, 1);

        wil::single_threaded_rw_property<winrt::CornerRadius> InlineCodeCornerRadius = winrt::CornerRadius(2, 2, 2, 2);

        wil::single_threaded_rw_property<winrt::Thickness> InlineCodePadding = winrt::Thickness(0, 0, 0, 0);

        wil::single_threaded_rw_property<double> InlineCodeFontSize = 10;

        wil::single_threaded_rw_property<winrt::FontWeight> InlineCodeFontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<winrt::FontWeight> BoldFontWeight = FontWeights::SemiBold();

        // Code block styling
        wil::single_threaded_rw_property<winrt::Brush> CodeBlockBackground = Application::Current().Resources().Lookup(winrt::box_value(L"CardBackgroundFillColorSecondaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Brush> CodeBlockBorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Thickness> CodeBlockBorderThickness = winrt::ThicknessHelper::FromUniformLength(1);
        wil::single_threaded_rw_property<winrt::Thickness> CodeBlockPadding = winrt::ThicknessHelper::FromUniformLength(8);
        wil::single_threaded_rw_property<winrt::Thickness> CodeBlockMargin = winrt::Thickness(0, 8, 0, 8);
        wil::single_threaded_rw_property<winrt::FontFamily> CodeBlockFontFamily = winrt::FontFamily(L"Consolas");
        wil::single_threaded_rw_property<winrt::Brush> CodeBlockForeground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::CornerRadius> CodeBlockCornerRadius = winrt::CornerRadiusHelper::FromUniformRadius(4);

        // Horizontal rule
        wil::single_threaded_rw_property<winrt::Brush> HorizontalRuleBrush = Application::Current().Resources().Lookup(winrt::box_value(L"DividerStrokeColorDefaultBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<double> HorizontalRuleThickness = 1.0;
        wil::single_threaded_rw_property<winrt::Thickness> HorizontalRuleMargin = winrt::Thickness(0, 12, 0, 12);

        // Link styling
        wil::single_threaded_rw_property<winrt::Brush> LinkForeground = Application::Current().Resources().Lookup(winrt::box_value(L"AccentTextFillColorPrimaryBrush")).as<winrt::Brush>();

        // Paragraph / list
        wil::single_threaded_rw_property<winrt::Thickness> ParagraphMargin = winrt::Thickness(0, 8, 0, 8);
        wil::single_threaded_rw_property<double> ParagraphLineHeight = 0.0; // 0 = default
        wil::single_threaded_rw_property<double> ListBulletSpacing = 4.0;
        wil::single_threaded_rw_property<double> ListGutterWidth = 8.0;
        wil::single_threaded_rw_property<winrt::Thickness> ListMargin = winrt::Thickness(0, 4, 0, 4);

        // Quote styling
        wil::single_threaded_rw_property<winrt::Brush> QuoteBackground = winrt::SolidColorBrush(winrt::Microsoft::UI::Colors::Transparent());
        wil::single_threaded_rw_property<winrt::Brush> QuoteBorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Thickness> QuoteBorderThickness = winrt::Thickness(4, 0, 0, 0);
        wil::single_threaded_rw_property<winrt::Brush> QuoteForeground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Thickness> QuoteMargin = winrt::Thickness(0, 4, 0, 4);
        wil::single_threaded_rw_property<winrt::Thickness> QuotePadding = winrt::ThicknessHelper::FromUniformLength(4);
        wil::single_threaded_rw_property<winrt::CornerRadius> QuoteCornerRadius = winrt::CornerRadiusHelper::FromUniformRadius(4);

        // Image styling
        wil::single_threaded_rw_property<double> ImageMaxWidth = 0.0;
        wil::single_threaded_rw_property<double> ImageMaxHeight = 0.0;
        wil::single_threaded_rw_property<winrt::Stretch> ImageStretch = winrt::Stretch::Uniform;

        // Table styling
        wil::single_threaded_rw_property<winrt::Brush> TableBorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<float> TableBorderThickness = 1.0f;
        wil::single_threaded_rw_property<winrt::Thickness> TableCellPadding = winrt::ThicknessHelper::FromUniformLength(4);
        wil::single_threaded_rw_property<winrt::Thickness> TableMargin = winrt::Thickness(0, 10, 0, 10);

        // YAML styling
        wil::single_threaded_rw_property<winrt::Brush> YamlBorderBrush = Application::Current().Resources().Lookup(winrt::box_value(L"CardStrokeColorDefaultBrush")).as<winrt::Brush>();
        wil::single_threaded_rw_property<winrt::Thickness> YamlBorderThickness = winrt::ThicknessHelper::FromUniformLength(1);
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownThemes : MarkdownThemesT<MarkdownThemes, implementation::MarkdownThemes>
    {
    };
}
