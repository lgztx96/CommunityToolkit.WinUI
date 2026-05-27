#include "pch.h"
#include "winrt_module_imports.h"
#include "MarkdownTextBlock.h"
#if __has_include("MarkdownTextBlock.g.cpp")
#include "MarkdownTextBlock.g.cpp"
#endif

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	void MarkdownTextBlock::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		_container = GetTemplateChild(MarkdownContainerName).as<Grid>();
		auto children = _container.Children();
		children.Clear();
		children.Append(_document->RichTextBlock());
		Build();
	}

	void MarkdownTextBlock::ApplyConfig(XamlToolkit::Labs::WinUI::MarkdownConfig const& config) const
	{
		if (_renderer) _renderer->Config(config);
	}

	void MarkdownTextBlock::ApplyText(std::wstring_view text, bool rerender) const
	{
		if (_renderer)
		{
			if (rerender)
			{
				_renderer->ReloadDocument();
			}
			_renderer->Render(text);
		}
	}

	void MarkdownTextBlock::Build()
	{
		if (const auto config = Config())
		{
			if (_renderer == nullptr)
			{
				_renderer = std::make_unique<WinUIRenderer>(_document, config, *this);
				ApplyText(Text(), false);
			}
		}
	}

	bool MarkdownTextBlock::RaiseLinkClickedEvent(winrt::Windows::Foundation::Uri const& uri)
	{
		auto args = winrt::make_self<LinkClickedEventArgs>(uri);
		OnLinkClicked.invoke(*this, *args);
		return args->Handled();
	}
}
