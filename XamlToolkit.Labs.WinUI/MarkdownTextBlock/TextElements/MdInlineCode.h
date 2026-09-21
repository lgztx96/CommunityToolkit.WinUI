// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Microsoft.UI.Xaml.Media.Media3D.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media::Media3D;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdInlineCode final : public IAddChild
    {
        winrt::InlineUIContainer _inlineContainer;
        winrt::TextBlock _textBlock;
        MarkdownTextBlock _control;

    public:
        winrt::TextElement TextElement() const override
        {
            return _inlineContainer;
        }

		MdInlineCode(XamlToolkit::Labs::WinUI::MarkdownTextBlock const& control) : _control(control) { }

        void Enter() override
        {
            winrt::Border border;
            border.VerticalAlignment(winrt::VerticalAlignment::Bottom);
            border.Background(_control.InlineCodeBackground());
            border.BorderBrush(_control.InlineCodeBorderBrush());
            border.BorderThickness(_control.InlineCodeBorderThickness());
            border.CornerRadius(_control.InlineCodeCornerRadius());
            border.Padding(_control.InlineCodePadding());
            winrt::CompositeTransform3D transform;
            transform.TranslateY(4.0);
            border.Transform3D(transform);

            _textBlock.FontSize(_control.InlineCodeFontSize());
            _textBlock.Foreground(_control.InlineCodeForeground());
            _textBlock.FontWeight(_control.InlineCodeFontWeight());
            _textBlock.IsTextSelectionEnabled(_control.IsTextSelectionEnabled());
            border.Child(_textBlock);
            _inlineContainer.Child(border);
		}

        void AddChild(IAddChild* child) override
        {
            if (auto inlineElement = child->TextElement().try_as<winrt::Inline>())
            {
				_textBlock.Inlines().Append(inlineElement);
            }
        }
    };
}
