// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <MarkdownTextBlock/MarkdownConfig.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.Media3D.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Media::Media3D;

    class MdInlineCode final : public IAddChild
    {
        InlineUIContainer _inlineContainer;
        TextBlock _textBlock;
        MarkdownConfig _config;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _inlineContainer;
        }

		MdInlineCode(CommunityToolkit::Labs::WinUI::MarkdownConfig const& config) : _config(config) { }

        void Enter() override 
        {
            Border border;
            border.VerticalAlignment(VerticalAlignment::Bottom);
            border.Background(_config.Themes().InlineCodeBackground());
            border.BorderBrush(_config.Themes().InlineCodeBorderBrush());
            border.BorderThickness(_config.Themes().InlineCodeBorderThickness());
            border.CornerRadius(_config.Themes().InlineCodeCornerRadius());
            border.Padding(_config.Themes().InlineCodePadding());
            CompositeTransform3D transform;
            transform.TranslateY(4.0);
            border.Transform3D(transform);

            _textBlock.FontSize(_config.Themes().InlineCodeFontSize());
            _textBlock.Foreground(_config.Themes().InlineCodeForeground());
            _textBlock.FontWeight(_config.Themes().InlineCodeFontWeight());
            _textBlock.IsTextSelectionEnabled(true);
            border.Child(_textBlock);
            _inlineContainer.Child(border);
		}

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto inlineElement = child->TextElement().try_as<Inline>())
            {
				_textBlock.Inlines().Append(inlineElement);
            }
        }
    };
}

