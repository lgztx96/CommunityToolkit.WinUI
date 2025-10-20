#pragma once

#include "MarkdownThemes.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Text.h>
#include "Extension.h"

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    using namespace winrt;
    using namespace Windows::UI::Text;
    using namespace Microsoft::UI;
    using namespace Microsoft::UI::Text;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;

    using FontWeights = Microsoft::UI::Text::FontWeights;

    struct MarkdownThemes : MarkdownThemesT<MarkdownThemes>
    {
        MarkdownThemes() = default;

        static CommunityToolkit::Labs::WinUI::MarkdownThemes Default();

        wil::single_threaded_rw_property<Thickness> Padding = Thickness(8, 8, 8, 8);

        wil::single_threaded_rw_property<Thickness> InternalMargin = Thickness(4, 4, 4, 4);

        wil::single_threaded_rw_property<CornerRadius> CornerRadius = Microsoft::UI::Xaml::CornerRadius(4, 4, 4, 4);

        wil::single_threaded_rw_property<double> H1FontSize = 22;

        wil::single_threaded_rw_property<double> H2FontSize = 20;

        wil::single_threaded_rw_property<double> H3FontSize = 18;

        wil::single_threaded_rw_property<double> H4FontSize = 16;

        wil::single_threaded_rw_property<double> H5FontSize = 14;

        wil::single_threaded_rw_property<double> H6FontSize = 12;

        wil::single_threaded_rw_property<Brush> H1Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Brush> H2Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Brush> H3Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Brush> H4Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Brush> H5Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Brush> H6Foreground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();

        wil::single_threaded_rw_property<FontWeight> H1FontWeight = FontWeights::Bold();

        wil::single_threaded_rw_property<FontWeight> H2FontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<FontWeight> H3FontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<FontWeight> H4FontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<FontWeight> H5FontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<FontWeight> H6FontWeight = FontWeights::Normal();

        wil::single_threaded_rw_property<Thickness> H1Margin = Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<Thickness> H2Margin = Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<Thickness> H3Margin = Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<Thickness> H4Margin = Thickness(0, 16, 0, 0);
        wil::single_threaded_rw_property<Thickness> H5Margin = Thickness(0, 8, 0, 0);
        wil::single_threaded_rw_property<Thickness> H6Margin = Thickness(0, 8, 0, 0);

        wil::single_threaded_rw_property<Brush> BorderBrush = SolidColorBrush(Colors::Gray());

        wil::single_threaded_rw_property<Brush> TableHeadingBackground = Extensions:: GetAccentColorBrush(Windows::UI::ViewManagement::UIColorType::AccentLight3);

        wil::single_threaded_rw_property<Brush> InlineCodeBackground = Application::Current().Resources().Lookup(winrt::box_value(L"ExpanderHeaderBackground")).as<Brush>();

        wil::single_threaded_rw_property<Brush> InlineCodeForeground = Application::Current().Resources().Lookup(winrt::box_value(L"TextFillColorPrimaryBrush")).as<Brush>();

        wil::single_threaded_rw_property<Brush> InlineCodeBorderBrush = SolidColorBrush(Colors::Gray());

        wil::single_threaded_rw_property<Thickness> InlineCodeBorderThickness = Thickness(1, 1, 1, 1);

        wil::single_threaded_rw_property<Microsoft::UI::Xaml::CornerRadius> InlineCodeCornerRadius = Microsoft::UI::Xaml::CornerRadius(2, 2, 2, 2);

        wil::single_threaded_rw_property<Thickness> InlineCodePadding = Thickness(0, 0, 0, 0);

        wil::single_threaded_rw_property<double> InlineCodeFontSize = 10;

        wil::single_threaded_rw_property<FontWeight> InlineCodeFontWeight = FontWeights::Normal();

        // Code block styling
        wil::single_threaded_rw_property<Brush> CodeBlockBackground = Application::Current().Resources().Lookup(box_value(L"ExpanderHeaderBackground")).as<Brush>();
        wil::single_threaded_rw_property<Brush> CodeBlockBorderBrush = SolidColorBrush(Colors::Gray());
        wil::single_threaded_rw_property<Thickness> CodeBlockBorderThickness = ThicknessHelper::FromUniformLength(1);
        wil::single_threaded_rw_property<Thickness> CodeBlockPadding = ThicknessHelper::FromUniformLength(8);
        wil::single_threaded_rw_property<Thickness> CodeBlockMargin = Thickness(0, 8, 0, 8);
        wil::single_threaded_rw_property<FontFamily> CodeBlockFontFamily = FontFamily(L"Consolas");
        wil::single_threaded_rw_property<Brush> CodeBlockForeground =
            Application::Current().Resources().Lookup(box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Microsoft::UI::Xaml::CornerRadius> CodeBlockCornerRadius = CornerRadiusHelper::FromUniformRadius(4);

        // Horizontal rule
        wil::single_threaded_rw_property<Brush> HorizontalRuleBrush = SolidColorBrush(Colors::Gray());
        wil::single_threaded_rw_property<double> HorizontalRuleThickness = 1.0;
        wil::single_threaded_rw_property<Thickness> HorizontalRuleMargin = Thickness(0, 12, 0, 12);

        // Link styling
        wil::single_threaded_rw_property<Brush> LinkForeground =
            Application::Current().Resources().TryLookup(box_value(L"AccentTextFillColorPrimaryBrush")).try_as<Brush>()
                ? Application::Current().Resources().Lookup(box_value(L"AccentTextFillColorPrimaryBrush")).as<Brush>()
                : SolidColorBrush(Colors::DodgerBlue());

        // Paragraph / list
        wil::single_threaded_rw_property<Thickness> ParagraphMargin = Thickness(0, 8, 0, 8);
        wil::single_threaded_rw_property<double> ParagraphLineHeight = 0.0; // 0 = default
        wil::single_threaded_rw_property<double> ListBulletSpacing = 4.0;
        wil::single_threaded_rw_property<double> ListGutterWidth = 30.0;
        wil::single_threaded_rw_property<Thickness> ListMargin = Thickness(0, 4, 0, 4);

        // Quote styling
        wil::single_threaded_rw_property<Brush> QuoteBackground = SolidColorBrush(Colors::Transparent());
        wil::single_threaded_rw_property<Brush> QuoteBorderBrush = SolidColorBrush(Colors::Gray());
        wil::single_threaded_rw_property<Thickness> QuoteBorderThickness = Thickness(4, 0, 0, 0);
        wil::single_threaded_rw_property<Brush> QuoteForeground =
            Application::Current().Resources().Lookup(box_value(L"TextFillColorPrimaryBrush")).as<Brush>();
        wil::single_threaded_rw_property<Thickness> QuoteMargin = Thickness(0, 4, 0, 4);
        wil::single_threaded_rw_property<Thickness> QuotePadding = ThicknessHelper::FromUniformLength(4);
        wil::single_threaded_rw_property<Microsoft::UI::Xaml::CornerRadius> QuoteCornerRadius = CornerRadiusHelper::FromUniformRadius(4);

        // Image styling
        wil::single_threaded_rw_property<double> ImageMaxWidth = 0.0;
        wil::single_threaded_rw_property<double> ImageMaxHeight = 0.0;
        wil::single_threaded_rw_property<Stretch> ImageStretch = Stretch::Uniform;

        // Table styling
        wil::single_threaded_rw_property<Brush> TableBorderBrush = SolidColorBrush(Colors::Gray());
        wil::single_threaded_rw_property<float> TableBorderThickness = 1.0f;
        wil::single_threaded_rw_property<Thickness> TableCellPadding = ThicknessHelper::FromUniformLength(4);
        wil::single_threaded_rw_property<Thickness> TableMargin = Thickness(0, 10, 0, 10);

        // YAML styling
        wil::single_threaded_rw_property<Brush> YamlBorderBrush = SolidColorBrush(Colors::Gray());
        wil::single_threaded_rw_property<Thickness> YamlBorderThickness = ThicknessHelper::FromUniformLength(1);
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownThemes : MarkdownThemesT<MarkdownThemes, implementation::MarkdownThemes>
    {
    };
}
