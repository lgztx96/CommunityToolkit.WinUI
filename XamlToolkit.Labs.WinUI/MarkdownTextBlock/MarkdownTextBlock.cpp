#include "pch.h"
#include "winrt_module_imports.h"
#include "MarkdownTextBlock.h"
#if __has_include("MarkdownTextBlock.g.cpp")
#include "MarkdownTextBlock.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	// Registers a theme dependency property. Every theme DP shares OnThemePropertyChanged, so a batch
	// of updates — most importantly the Setter pass when a Style is applied — collapses into a single
	// queued re-render instead of one render per property.
#define MTB_THEME_DP(name, type, ...)                                                                  \
	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::name##Property = \
		winrt::DependencyProperty::Register(                                                           \
			L"" #name,                                                                                 \
			winrt::xaml_typename<type>(),                                                              \
			winrt::xaml_typename<class_type>(),                                                        \
			winrt::PropertyMetadata(__VA_ARGS__, &MarkdownTextBlock::OnThemePropertyChanged));

	// ── Headings ────────────────────────────────────────────────────────────

	MTB_THEME_DP(H1FontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(H2FontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(H3FontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(H4FontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(H5FontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(H6FontSize, double, winrt::box_value(0.0))

	MTB_THEME_DP(H1Foreground, winrt::Brush, nullptr)
	MTB_THEME_DP(H2Foreground, winrt::Brush, nullptr)
	MTB_THEME_DP(H3Foreground, winrt::Brush, nullptr)
	MTB_THEME_DP(H4Foreground, winrt::Brush, nullptr)
	MTB_THEME_DP(H5Foreground, winrt::Brush, nullptr)
	MTB_THEME_DP(H6Foreground, winrt::Brush, nullptr)

	MTB_THEME_DP(H1FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))
	MTB_THEME_DP(H2FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))
	MTB_THEME_DP(H3FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))
	MTB_THEME_DP(H4FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))
	MTB_THEME_DP(H5FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))
	MTB_THEME_DP(H6FontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))

	MTB_THEME_DP(H1Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(H2Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(H3Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(H4Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(H5Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(H6Margin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))

	// ── Inline code ─────────────────────────────────────────────────────────

	MTB_THEME_DP(InlineCodeBackground, winrt::Brush, nullptr)
	MTB_THEME_DP(InlineCodeForeground, winrt::Brush, nullptr)
	MTB_THEME_DP(InlineCodeBorderBrush, winrt::Brush, nullptr)
	MTB_THEME_DP(InlineCodeBorderThickness, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(InlineCodeCornerRadius, winrt::CornerRadius, winrt::box_value(winrt::CornerRadius{ 0, 0, 0, 0 }))
	MTB_THEME_DP(InlineCodePadding, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(InlineCodeFontSize, double, winrt::box_value(0.0))
	MTB_THEME_DP(InlineCodeFontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))

	MTB_THEME_DP(BoldFontWeight, winrt::FontWeight, winrt::box_value(winrt::FontWeight{ 400 }))

	// ── Code blocks ─────────────────────────────────────────────────────────

	MTB_THEME_DP(CodeBlockBackground, winrt::Brush, nullptr)
	MTB_THEME_DP(CodeBlockBorderBrush, winrt::Brush, nullptr)
	MTB_THEME_DP(CodeBlockBorderThickness, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(CodeBlockPadding, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(CodeBlockMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(CodeBlockCornerRadius, winrt::CornerRadius, winrt::box_value(winrt::CornerRadius{ 0, 0, 0, 0 }))
	MTB_THEME_DP(CodeBlockFontFamily, winrt::FontFamily, nullptr)
	MTB_THEME_DP(CodeBlockForeground, winrt::Brush, nullptr)

	// ── Horizontal rule ─────────────────────────────────────────────────────

	MTB_THEME_DP(HorizontalRuleBrush, winrt::Brush, nullptr)
	MTB_THEME_DP(HorizontalRuleThickness, double, winrt::box_value(0.0))
	MTB_THEME_DP(HorizontalRuleMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))

	// ── Links ───────────────────────────────────────────────────────────────

	MTB_THEME_DP(LinkForeground, winrt::Brush, nullptr)

	// ── Paragraphs and lists ────────────────────────────────────────────────

	MTB_THEME_DP(ParagraphMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(ParagraphLineHeight, double, winrt::box_value(0.0))
	MTB_THEME_DP(ListBulletSpacing, double, winrt::box_value(0.0))
	MTB_THEME_DP(ListGutterWidth, double, winrt::box_value(0.0))

	// ── Block quotes ────────────────────────────────────────────────────────

	MTB_THEME_DP(QuoteBackground, winrt::Brush, nullptr)
	MTB_THEME_DP(QuoteBorderBrush, winrt::Brush, nullptr)
	MTB_THEME_DP(QuoteBorderThickness, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(QuoteForeground, winrt::Brush, nullptr)
	MTB_THEME_DP(QuoteMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(QuotePadding, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(QuoteBarMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(QuoteCornerRadius, winrt::CornerRadius, winrt::box_value(winrt::CornerRadius{ 0, 0, 0, 0 }))

	// ── Images ──────────────────────────────────────────────────────────────

	MTB_THEME_DP(ImageMaxWidth, double, winrt::box_value(0.0))
	MTB_THEME_DP(ImageMaxHeight, double, winrt::box_value(0.0))
	MTB_THEME_DP(ImageStretch, winrt::Stretch, winrt::box_value(winrt::Stretch::Uniform))

	// ── Tables ──────────────────────────────────────────────────────────────

	MTB_THEME_DP(TableHeadingBackground, winrt::Brush, nullptr)
	MTB_THEME_DP(TableBorderBrush, winrt::Brush, nullptr)
	MTB_THEME_DP(TableBorderThickness, double, winrt::box_value(0.0))
	MTB_THEME_DP(TableCellPadding, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(TableMargin, winrt::Thickness, winrt::box_value(winrt::Thickness{ 0, 0, 0, 0 }))
	MTB_THEME_DP(TableCornerRadius, winrt::CornerRadius, winrt::box_value(winrt::CornerRadius{ 0, 0, 0, 0 }))

#undef MTB_THEME_DP

	// Not theme state: these describe where content comes from, and the source registers them without
	// a change callback, so changing them does not by itself re-render.
	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::BaseUrlProperty =
		winrt::DependencyProperty::Register(
			L"BaseUrl",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::ImageProviderProperty =
		winrt::DependencyProperty::Register(
			L"ImageProvider",
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::IImageProvider>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::SVGRendererProperty =
		winrt::DependencyProperty::Register(
			L"SVGRenderer",
			winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::ISVGRenderer>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr));

	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::TextProperty =
		winrt::DependencyProperty::Register(
			L"Text",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &MarkdownTextBlock::OnTextChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> MarkdownTextBlock::IsTextSelectionEnabledProperty =
		winrt::DependencyProperty::Register(
			L"IsTextSelectionEnabled",
			winrt::xaml_typename<bool>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(true), &MarkdownTextBlock::OnIsTextSelectionEnabledChanged));

	MarkdownTextBlock::MarkdownTextBlock() : _document(std::make_shared<MdFlowDocument>())
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		Loaded(&MarkdownTextBlock::OnLoaded);
		Unloaded(&MarkdownTextBlock::OnUnloaded);
	}

	void MarkdownTextBlock::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		_container = GetTemplateChild(MarkdownContainerName).as<winrt::Grid>();
		auto children = _container.Children();
		children.Clear();
		children.Append(_document->RichTextBlock());
		Build();
	}

	void MarkdownTextBlock::OnLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const&)
	{
		const auto impl = winrt::get_self<MarkdownTextBlock>(sender.try_as<class_type>());
		if (!impl->_actualThemeChangedToken)
		{
			impl->_actualThemeChangedToken = impl->ActualThemeChanged(&MarkdownTextBlock::OnActualThemeChanged);
		}
	}

	void MarkdownTextBlock::OnUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const&)
	{
		const auto impl = winrt::get_self<MarkdownTextBlock>(sender.try_as<class_type>());
		if (impl->_actualThemeChangedToken)
		{
			impl->ActualThemeChanged(impl->_actualThemeChangedToken);
			impl->_actualThemeChangedToken = {};
		}
	}

	// A theme property change and an app theme switch mean the same thing here — the theme values
	// moved — so both queue their re-render through this. One render per batch, not one per
	// property: applying a Style sets every theme DP in a row, and the first change queues a single
	// re-render that the rest fold into.
	void MarkdownTextBlock::QueueThemeRerender(winrt::DependencyObject const& d)
	{
		const auto control = d.try_as<class_type>();
		if (!control)
		{
			return;
		}

		const auto impl = winrt::get_self<MarkdownTextBlock>(control);
		if (impl->_themePropertyChangeQueued)
		{
			return;
		}

		impl->_themePropertyChangeQueued = true;
		impl->DispatcherQueue().TryEnqueue([weak = winrt::make_weak(control)]()
		{
			if (const auto queued = weak.get())
			{
				const auto self = winrt::get_self<MarkdownTextBlock>(queued);
				self->_themePropertyChangeQueued = false;
				self->ApplyText(true);
			}
		});
	}

	void MarkdownTextBlock::OnActualThemeChanged(winrt::FrameworkElement const& sender, winrt::IInspectable const&)
	{
		// The app theme changed, which usually means the ThemeResource-backed brushes the Style
		// installed have new values.
		QueueThemeRerender(sender);
	}

	void MarkdownTextBlock::OnThemePropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const&)
	{
		QueueThemeRerender(d);
	}

	void MarkdownTextBlock::OnTextChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto control = d.try_as<class_type>(); control && e.NewValue())
		{
			const auto self = winrt::get_self<MarkdownTextBlock>(control);
			self->ApplyText(true);
		}
	}

	void MarkdownTextBlock::OnIsTextSelectionEnabledChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (const auto mtb = d.try_as<class_type>())
		{
			const auto self = winrt::get_self<MarkdownTextBlock>(mtb);
			if (self->_document)
			{
				self->_document->RichTextBlock().IsTextSelectionEnabled(winrt::unbox_value<bool>(e.NewValue()));
			}
		}
	}

	void MarkdownTextBlock::ApplyText(bool rerender) const
	{
		if (_renderer)
		{
			if (rerender)
			{
				_renderer->ReloadDocument();
			}

			if (const auto text = Text(); !text.empty())
			{
				_renderer->Render(text);
			}
		}
	}

	void MarkdownTextBlock::Build(bool rerender)
	{
		if (!_renderer)
		{
			_renderer = std::make_unique<WinUIRenderer>(_document, *this);
		}

		ApplyText(rerender);
	}

	bool MarkdownTextBlock::RaiseLinkClickedEvent(winrt::Uri const& uri)
	{
		const auto args = winrt::make_self<LinkClickedEventArgs>(uri);
		OnLinkClicked.invoke(*this, *args);
		return args->Handled();
	}
}
