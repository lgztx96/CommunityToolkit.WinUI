#pragma once

#include "LinkClickedEventArgs.h"
#include "MarkdownTextBlock.g.h"
#include "Renderer\WinUIRenderer.h"
#include "TextElements/MdFlowDocument.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <memory>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Windows::UI::Text;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Documents;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace XamlToolkit::Labs::WinUI::TextElements;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock>
    {
        static constexpr auto MarkdownContainerName = L"MarkdownContainer";

        MarkdownTextBlock();

        wil::untyped_event<winrt::XamlToolkit::Labs::WinUI::LinkClickedEventArgs> OnLinkClicked;

        bool RaiseLinkClickedEvent(winrt::Uri const& uri);

        static const wil::single_threaded_property<winrt::DependencyProperty> TextProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> IsTextSelectionEnabledProperty;

        winrt::hstring Text() const { return winrt::unbox_value_or<winrt::hstring>(GetValue(TextProperty()), L""); }
        void Text(winrt::hstring const& value) { SetValue(TextProperty(), winrt::box_value(value)); }

        bool IsTextSelectionEnabled() const { return winrt::unbox_value<bool>(GetValue(IsTextSelectionEnabledProperty())); }
        void IsTextSelectionEnabled(bool value) { SetValue(IsTextSelectionEnabledProperty(), winrt::box_value(value)); }

        // ── Configuration ───────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> BaseUrlProperty;

        winrt::hstring BaseUrl() const { return winrt::unbox_value_or<winrt::hstring>(GetValue(BaseUrlProperty()), L""); }
        void BaseUrl(winrt::hstring const& value) { SetValue(BaseUrlProperty(), winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> ImageProviderProperty;

        winrt::XamlToolkit::Labs::WinUI::IImageProvider ImageProvider() const { return ReadValue<winrt::XamlToolkit::Labs::WinUI::IImageProvider>(GetValue(ImageProviderProperty())); }
        void ImageProvider(winrt::XamlToolkit::Labs::WinUI::IImageProvider const& value) { SetValue(ImageProviderProperty(), value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> SVGRendererProperty;

        winrt::XamlToolkit::Labs::WinUI::ISVGRenderer SVGRenderer() const { return ReadValue<winrt::XamlToolkit::Labs::WinUI::ISVGRenderer>(GetValue(SVGRendererProperty())); }
        void SVGRenderer(winrt::XamlToolkit::Labs::WinUI::ISVGRenderer const& value) { SetValue(SVGRendererProperty(), value); }

        // ── Headings ────────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> H1FontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H2FontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H3FontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H4FontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H5FontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H6FontSizeProperty;

        double H1FontSize() const { return winrt::unbox_value<double>(GetValue(H1FontSizeProperty())); }
        void H1FontSize(double value) { SetValue(H1FontSizeProperty(), winrt::box_value(value)); }
        double H2FontSize() const { return winrt::unbox_value<double>(GetValue(H2FontSizeProperty())); }
        void H2FontSize(double value) { SetValue(H2FontSizeProperty(), winrt::box_value(value)); }
        double H3FontSize() const { return winrt::unbox_value<double>(GetValue(H3FontSizeProperty())); }
        void H3FontSize(double value) { SetValue(H3FontSizeProperty(), winrt::box_value(value)); }
        double H4FontSize() const { return winrt::unbox_value<double>(GetValue(H4FontSizeProperty())); }
        void H4FontSize(double value) { SetValue(H4FontSizeProperty(), winrt::box_value(value)); }
        double H5FontSize() const { return winrt::unbox_value<double>(GetValue(H5FontSizeProperty())); }
        void H5FontSize(double value) { SetValue(H5FontSizeProperty(), winrt::box_value(value)); }
        double H6FontSize() const { return winrt::unbox_value<double>(GetValue(H6FontSizeProperty())); }
        void H6FontSize(double value) { SetValue(H6FontSizeProperty(), winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> H1ForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H2ForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H3ForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H4ForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H5ForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H6ForegroundProperty;

        winrt::Brush H1Foreground() const { return ReadValue<winrt::Brush>(GetValue(H1ForegroundProperty())); }
        void H1Foreground(winrt::Brush const& value) { SetValue(H1ForegroundProperty(), value); }
        winrt::Brush H2Foreground() const { return ReadValue<winrt::Brush>(GetValue(H2ForegroundProperty())); }
        void H2Foreground(winrt::Brush const& value) { SetValue(H2ForegroundProperty(), value); }
        winrt::Brush H3Foreground() const { return ReadValue<winrt::Brush>(GetValue(H3ForegroundProperty())); }
        void H3Foreground(winrt::Brush const& value) { SetValue(H3ForegroundProperty(), value); }
        winrt::Brush H4Foreground() const { return ReadValue<winrt::Brush>(GetValue(H4ForegroundProperty())); }
        void H4Foreground(winrt::Brush const& value) { SetValue(H4ForegroundProperty(), value); }
        winrt::Brush H5Foreground() const { return ReadValue<winrt::Brush>(GetValue(H5ForegroundProperty())); }
        void H5Foreground(winrt::Brush const& value) { SetValue(H5ForegroundProperty(), value); }
        winrt::Brush H6Foreground() const { return ReadValue<winrt::Brush>(GetValue(H6ForegroundProperty())); }
        void H6Foreground(winrt::Brush const& value) { SetValue(H6ForegroundProperty(), value); }

        static const wil::single_threaded_property<winrt::DependencyProperty> H1FontWeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H2FontWeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H3FontWeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H4FontWeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H5FontWeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H6FontWeightProperty;

        winrt::FontWeight H1FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H1FontWeightProperty())); }
        void H1FontWeight(winrt::FontWeight const& value) { SetValue(H1FontWeightProperty(), winrt::box_value(value)); }
        winrt::FontWeight H2FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H2FontWeightProperty())); }
        void H2FontWeight(winrt::FontWeight const& value) { SetValue(H2FontWeightProperty(), winrt::box_value(value)); }
        winrt::FontWeight H3FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H3FontWeightProperty())); }
        void H3FontWeight(winrt::FontWeight const& value) { SetValue(H3FontWeightProperty(), winrt::box_value(value)); }
        winrt::FontWeight H4FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H4FontWeightProperty())); }
        void H4FontWeight(winrt::FontWeight const& value) { SetValue(H4FontWeightProperty(), winrt::box_value(value)); }
        winrt::FontWeight H5FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H5FontWeightProperty())); }
        void H5FontWeight(winrt::FontWeight const& value) { SetValue(H5FontWeightProperty(), winrt::box_value(value)); }
        winrt::FontWeight H6FontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(H6FontWeightProperty())); }
        void H6FontWeight(winrt::FontWeight const& value) { SetValue(H6FontWeightProperty(), winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> H1MarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H2MarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H3MarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H4MarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H5MarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> H6MarginProperty;

        winrt::Thickness H1Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H1MarginProperty())); }
        void H1Margin(winrt::Thickness const& value) { SetValue(H1MarginProperty(), winrt::box_value(value)); }
        winrt::Thickness H2Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H2MarginProperty())); }
        void H2Margin(winrt::Thickness const& value) { SetValue(H2MarginProperty(), winrt::box_value(value)); }
        winrt::Thickness H3Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H3MarginProperty())); }
        void H3Margin(winrt::Thickness const& value) { SetValue(H3MarginProperty(), winrt::box_value(value)); }
        winrt::Thickness H4Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H4MarginProperty())); }
        void H4Margin(winrt::Thickness const& value) { SetValue(H4MarginProperty(), winrt::box_value(value)); }
        winrt::Thickness H5Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H5MarginProperty())); }
        void H5Margin(winrt::Thickness const& value) { SetValue(H5MarginProperty(), winrt::box_value(value)); }
        winrt::Thickness H6Margin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(H6MarginProperty())); }
        void H6Margin(winrt::Thickness const& value) { SetValue(H6MarginProperty(), winrt::box_value(value)); }

        // ── Inline code ─────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeBackgroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeBorderBrushProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeBorderThicknessProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeCornerRadiusProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodePaddingProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeFontSizeProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> InlineCodeFontWeightProperty;

        winrt::Brush InlineCodeBackground() const { return ReadValue<winrt::Brush>(GetValue(InlineCodeBackgroundProperty())); }
        void InlineCodeBackground(winrt::Brush const& value) { SetValue(InlineCodeBackgroundProperty(), value); }
        winrt::Brush InlineCodeForeground() const { return ReadValue<winrt::Brush>(GetValue(InlineCodeForegroundProperty())); }
        void InlineCodeForeground(winrt::Brush const& value) { SetValue(InlineCodeForegroundProperty(), value); }
        winrt::Brush InlineCodeBorderBrush() const { return ReadValue<winrt::Brush>(GetValue(InlineCodeBorderBrushProperty())); }
        void InlineCodeBorderBrush(winrt::Brush const& value) { SetValue(InlineCodeBorderBrushProperty(), value); }
        winrt::Thickness InlineCodeBorderThickness() const { return winrt::unbox_value<winrt::Thickness>(GetValue(InlineCodeBorderThicknessProperty())); }
        void InlineCodeBorderThickness(winrt::Thickness const& value) { SetValue(InlineCodeBorderThicknessProperty(), winrt::box_value(value)); }
        winrt::CornerRadius InlineCodeCornerRadius() const { return winrt::unbox_value<winrt::CornerRadius>(GetValue(InlineCodeCornerRadiusProperty())); }
        void InlineCodeCornerRadius(winrt::CornerRadius const& value) { SetValue(InlineCodeCornerRadiusProperty(), winrt::box_value(value)); }
        winrt::Thickness InlineCodePadding() const { return winrt::unbox_value<winrt::Thickness>(GetValue(InlineCodePaddingProperty())); }
        void InlineCodePadding(winrt::Thickness const& value) { SetValue(InlineCodePaddingProperty(), winrt::box_value(value)); }
        double InlineCodeFontSize() const { return winrt::unbox_value<double>(GetValue(InlineCodeFontSizeProperty())); }
        void InlineCodeFontSize(double value) { SetValue(InlineCodeFontSizeProperty(), winrt::box_value(value)); }
        winrt::FontWeight InlineCodeFontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(InlineCodeFontWeightProperty())); }
        void InlineCodeFontWeight(winrt::FontWeight const& value) { SetValue(InlineCodeFontWeightProperty(), winrt::box_value(value)); }

        static const wil::single_threaded_property<winrt::DependencyProperty> BoldFontWeightProperty;

        winrt::FontWeight BoldFontWeight() const { return winrt::unbox_value<winrt::FontWeight>(GetValue(BoldFontWeightProperty())); }
        void BoldFontWeight(winrt::FontWeight const& value) { SetValue(BoldFontWeightProperty(), winrt::box_value(value)); }

        // ── Code blocks ─────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockBackgroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockBorderBrushProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockBorderThicknessProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockPaddingProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockMarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockCornerRadiusProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockFontFamilyProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> CodeBlockForegroundProperty;

        winrt::Brush CodeBlockBackground() const { return ReadValue<winrt::Brush>(GetValue(CodeBlockBackgroundProperty())); }
        void CodeBlockBackground(winrt::Brush const& value) { SetValue(CodeBlockBackgroundProperty(), value); }
        winrt::Brush CodeBlockBorderBrush() const { return ReadValue<winrt::Brush>(GetValue(CodeBlockBorderBrushProperty())); }
        void CodeBlockBorderBrush(winrt::Brush const& value) { SetValue(CodeBlockBorderBrushProperty(), value); }
        winrt::Thickness CodeBlockBorderThickness() const { return winrt::unbox_value<winrt::Thickness>(GetValue(CodeBlockBorderThicknessProperty())); }
        void CodeBlockBorderThickness(winrt::Thickness const& value) { SetValue(CodeBlockBorderThicknessProperty(), winrt::box_value(value)); }
        winrt::Thickness CodeBlockPadding() const { return winrt::unbox_value<winrt::Thickness>(GetValue(CodeBlockPaddingProperty())); }
        void CodeBlockPadding(winrt::Thickness const& value) { SetValue(CodeBlockPaddingProperty(), winrt::box_value(value)); }
        winrt::Thickness CodeBlockMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(CodeBlockMarginProperty())); }
        void CodeBlockMargin(winrt::Thickness const& value) { SetValue(CodeBlockMarginProperty(), winrt::box_value(value)); }
        winrt::CornerRadius CodeBlockCornerRadius() const { return winrt::unbox_value<winrt::CornerRadius>(GetValue(CodeBlockCornerRadiusProperty())); }
        void CodeBlockCornerRadius(winrt::CornerRadius const& value) { SetValue(CodeBlockCornerRadiusProperty(), winrt::box_value(value)); }
        winrt::FontFamily CodeBlockFontFamily() const { return ReadValue<winrt::FontFamily>(GetValue(CodeBlockFontFamilyProperty())); }
        void CodeBlockFontFamily(winrt::FontFamily const& value) { SetValue(CodeBlockFontFamilyProperty(), value); }
        winrt::Brush CodeBlockForeground() const { return ReadValue<winrt::Brush>(GetValue(CodeBlockForegroundProperty())); }
        void CodeBlockForeground(winrt::Brush const& value) { SetValue(CodeBlockForegroundProperty(), value); }

        // ── Horizontal rule ─────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalRuleBrushProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalRuleThicknessProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> HorizontalRuleMarginProperty;

        winrt::Brush HorizontalRuleBrush() const { return ReadValue<winrt::Brush>(GetValue(HorizontalRuleBrushProperty())); }
        void HorizontalRuleBrush(winrt::Brush const& value) { SetValue(HorizontalRuleBrushProperty(), value); }
        double HorizontalRuleThickness() const { return winrt::unbox_value<double>(GetValue(HorizontalRuleThicknessProperty())); }
        void HorizontalRuleThickness(double value) { SetValue(HorizontalRuleThicknessProperty(), winrt::box_value(value)); }
        winrt::Thickness HorizontalRuleMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(HorizontalRuleMarginProperty())); }
        void HorizontalRuleMargin(winrt::Thickness const& value) { SetValue(HorizontalRuleMarginProperty(), winrt::box_value(value)); }

        // ── Links ───────────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> LinkForegroundProperty;

        winrt::Brush LinkForeground() const { return ReadValue<winrt::Brush>(GetValue(LinkForegroundProperty())); }
        void LinkForeground(winrt::Brush const& value) { SetValue(LinkForegroundProperty(), value); }

        // ── Paragraphs and lists ────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> ParagraphMarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ParagraphLineHeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ListBulletSpacingProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ListGutterWidthProperty;

        winrt::Thickness ParagraphMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(ParagraphMarginProperty())); }
        void ParagraphMargin(winrt::Thickness const& value) { SetValue(ParagraphMarginProperty(), winrt::box_value(value)); }
        double ParagraphLineHeight() const { return winrt::unbox_value<double>(GetValue(ParagraphLineHeightProperty())); }
        void ParagraphLineHeight(double value) { SetValue(ParagraphLineHeightProperty(), winrt::box_value(value)); }
        double ListBulletSpacing() const { return winrt::unbox_value<double>(GetValue(ListBulletSpacingProperty())); }
        void ListBulletSpacing(double value) { SetValue(ListBulletSpacingProperty(), winrt::box_value(value)); }
        double ListGutterWidth() const { return winrt::unbox_value<double>(GetValue(ListGutterWidthProperty())); }
        void ListGutterWidth(double value) { SetValue(ListGutterWidthProperty(), winrt::box_value(value)); }

        // ── Block quotes ────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteBackgroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteBorderBrushProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteBorderThicknessProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteForegroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteMarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuotePaddingProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteBarMarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> QuoteCornerRadiusProperty;

        winrt::Brush QuoteBackground() const { return ReadValue<winrt::Brush>(GetValue(QuoteBackgroundProperty())); }
        void QuoteBackground(winrt::Brush const& value) { SetValue(QuoteBackgroundProperty(), value); }
        winrt::Brush QuoteBorderBrush() const { return ReadValue<winrt::Brush>(GetValue(QuoteBorderBrushProperty())); }
        void QuoteBorderBrush(winrt::Brush const& value) { SetValue(QuoteBorderBrushProperty(), value); }
        winrt::Thickness QuoteBorderThickness() const { return winrt::unbox_value<winrt::Thickness>(GetValue(QuoteBorderThicknessProperty())); }
        void QuoteBorderThickness(winrt::Thickness const& value) { SetValue(QuoteBorderThicknessProperty(), winrt::box_value(value)); }
        winrt::Brush QuoteForeground() const { return ReadValue<winrt::Brush>(GetValue(QuoteForegroundProperty())); }
        void QuoteForeground(winrt::Brush const& value) { SetValue(QuoteForegroundProperty(), value); }
        winrt::Thickness QuoteMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(QuoteMarginProperty())); }
        void QuoteMargin(winrt::Thickness const& value) { SetValue(QuoteMarginProperty(), winrt::box_value(value)); }
        winrt::Thickness QuotePadding() const { return winrt::unbox_value<winrt::Thickness>(GetValue(QuotePaddingProperty())); }
        void QuotePadding(winrt::Thickness const& value) { SetValue(QuotePaddingProperty(), winrt::box_value(value)); }
        winrt::Thickness QuoteBarMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(QuoteBarMarginProperty())); }
        void QuoteBarMargin(winrt::Thickness const& value) { SetValue(QuoteBarMarginProperty(), winrt::box_value(value)); }
        winrt::CornerRadius QuoteCornerRadius() const { return winrt::unbox_value<winrt::CornerRadius>(GetValue(QuoteCornerRadiusProperty())); }
        void QuoteCornerRadius(winrt::CornerRadius const& value) { SetValue(QuoteCornerRadiusProperty(), winrt::box_value(value)); }

        // ── Images ──────────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> ImageMaxWidthProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ImageMaxHeightProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> ImageStretchProperty;

        double ImageMaxWidth() const { return winrt::unbox_value<double>(GetValue(ImageMaxWidthProperty())); }
        void ImageMaxWidth(double value) { SetValue(ImageMaxWidthProperty(), winrt::box_value(value)); }
        double ImageMaxHeight() const { return winrt::unbox_value<double>(GetValue(ImageMaxHeightProperty())); }
        void ImageMaxHeight(double value) { SetValue(ImageMaxHeightProperty(), winrt::box_value(value)); }
        winrt::Stretch ImageStretch() const { return winrt::unbox_value<winrt::Stretch>(GetValue(ImageStretchProperty())); }
        void ImageStretch(winrt::Stretch const& value) { SetValue(ImageStretchProperty(), winrt::box_value(value)); }

        // ── Tables ──────────────────────────────────────────────────────────

        static const wil::single_threaded_property<winrt::DependencyProperty> TableHeadingBackgroundProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TableBorderBrushProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TableBorderThicknessProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TableCellPaddingProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TableMarginProperty;
        static const wil::single_threaded_property<winrt::DependencyProperty> TableCornerRadiusProperty;

        winrt::Brush TableHeadingBackground() const { return ReadValue<winrt::Brush>(GetValue(TableHeadingBackgroundProperty())); }
        void TableHeadingBackground(winrt::Brush const& value) { SetValue(TableHeadingBackgroundProperty(), value); }
        winrt::Brush TableBorderBrush() const { return ReadValue<winrt::Brush>(GetValue(TableBorderBrushProperty())); }
        void TableBorderBrush(winrt::Brush const& value) { SetValue(TableBorderBrushProperty(), value); }
        double TableBorderThickness() const { return winrt::unbox_value<double>(GetValue(TableBorderThicknessProperty())); }
        void TableBorderThickness(double value) { SetValue(TableBorderThicknessProperty(), winrt::box_value(value)); }
        winrt::Thickness TableCellPadding() const { return winrt::unbox_value<winrt::Thickness>(GetValue(TableCellPaddingProperty())); }
        void TableCellPadding(winrt::Thickness const& value) { SetValue(TableCellPaddingProperty(), winrt::box_value(value)); }
        winrt::Thickness TableMargin() const { return winrt::unbox_value<winrt::Thickness>(GetValue(TableMarginProperty())); }
        void TableMargin(winrt::Thickness const& value) { SetValue(TableMarginProperty(), winrt::box_value(value)); }
        winrt::CornerRadius TableCornerRadius() const { return winrt::unbox_value<winrt::CornerRadius>(GetValue(TableCornerRadiusProperty())); }
        void TableCornerRadius(winrt::CornerRadius const& value) { SetValue(TableCornerRadiusProperty(), winrt::box_value(value)); }

        void OnApplyTemplate();

    private:
        // Reference-typed theme DPs default to null (the brush/family values all come from the
        // control's Style), so a plain try_as would be called on an empty IInspectable whenever a
        // Style omits a setter. Unset values read back as null instead of faulting.
        template <typename T>
        static T ReadValue(winrt::IInspectable const& value) { return value ? value.try_as<T>() : nullptr; }

        static void OnTextChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnIsTextSelectionEnabledChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void QueueThemeRerender(winrt::DependencyObject const& d);

        static void OnThemePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

        static void OnActualThemeChanged(winrt::FrameworkElement const& sender, winrt::IInspectable const& args);

        static void OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        static void OnUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        void ApplyText(bool rerender) const;

        void Build(bool rerender = true);

        winrt::Grid _container{ nullptr };
        std::shared_ptr<MdFlowDocument> _document;
        std::unique_ptr<WinUIRenderer> _renderer;
        bool _themePropertyChangeQueued{ false };
        winrt::event_token _actualThemeChangedToken;
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock, implementation::MarkdownTextBlock>
    {
    };
}
