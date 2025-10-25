#include "pch.h"
#include "MarkdownTextBlock.h"
#if __has_include("MarkdownTextBlock.g.cpp")
#include "MarkdownTextBlock.g.cpp"
#endif

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	void MarkdownTextBlock::OnApplyTemplate()
	{
		base_type::OnApplyTemplate();
		_container = GetTemplateChild(MarkdownContainerName).as<Grid>();
		_container.Children().Clear();
		_container.Children().Append(_document->RichTextBlock());
		Config(CommunityToolkit::Labs::WinUI::MarkdownConfig::Default());
		Build();
	}

	void MarkdownTextBlock::ApplyConfig(CommunityToolkit::Labs::WinUI::MarkdownConfig const& config) const
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
		if (Config())
		{
			if (_renderer == nullptr)
			{
				_renderer = std::make_unique<WinUIRenderer>(_document, Config(), *this);
			}

			ApplyText(Text(), false);
		}
	}

	bool MarkdownTextBlock::RaiseLinkClickedEvent(winrt::Windows::Foundation::Uri const& uri)
	{
		auto args = winrt::make<LinkClickedEventArgs>(uri);
		OnLinkClicked.invoke(*this, args);
		return args.Handled();
	}
}
