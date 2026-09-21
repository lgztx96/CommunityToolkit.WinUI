// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements 
{
    class MdHeading final : public IAddChild
    {
    public:
        winrt::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdHeading(MarkdownTextBlock const& control)
        {
            auto level = 1;
            _paragraph.FontSize([](int level, const auto& control)
            {
                switch (level)
                {
                case 1:
                    return control.H1FontSize();
                case 2:
                    return control.H2FontSize();
                case 3:
                    return control.H3FontSize();
                case 4:
                    return control.H4FontSize();
                case 5:
                    return control.H5FontSize();
                default:
                    return control.H6FontSize();
                }  
            }(level, control));

            _paragraph.Foreground([](int level, const auto& control)
            {
                switch (level)
                {
                case 1:
                    return control.H1Foreground();
                case 2:
                    return control.H2Foreground();
                case 3:
                    return control.H3Foreground();
                case 4:
                    return control.H4Foreground();
                case 5:
                    return control.H5Foreground();
                default:
                    return control.H6Foreground();
                }
            }(level, control));

            _paragraph.FontWeight([](int level, const auto& control)
            {
                switch (level)
                {
                case 1:
                    return control.H1FontWeight();
                case 2:
                    return control.H2FontWeight();
                case 3:
                    return control.H3FontWeight();
                case 4:
                    return control.H4FontWeight();
                case 5:
                    return control.H5FontWeight();
                default:
                    return control.H6FontWeight();
                }
            }(level, control));
        }

        MdHeading(int level, MarkdownTextBlock const& control)
        {
            std::string align{ "left" }; // _htmlNode.GetAttributeValue("align", "left");

            _paragraph.TextAlignment([&]
            {
                if (align == "left")
                    return winrt::TextAlignment::Left;
                else if (align == "right")
                    return winrt::TextAlignment::Right;
                else if (align == "center")
                    return winrt::TextAlignment::Center;
                else if (align == "justify")
                    return winrt::TextAlignment::Justify;
                else
                    return winrt::TextAlignment::Left;
            }());

            //auto level = level;// int.Parse(htmlNode.Name.Substring(1));
            _paragraph.FontSize([](int level, const auto& control)
            {
                switch (level)
                {
                case 1:
                    return control.H1FontSize();
                case 2:
                    return control.H2FontSize();
                case 3:
                    return control.H3FontSize();
                case 4:
                    return control.H4FontSize();
                case 5:
                    return control.H5FontSize();
                default:
                    return control.H6FontSize();
                }
            }(level, control));

            _paragraph.Foreground([](int level, const auto& control)
            {
                switch (level)
                {
                case 1:
                    return control.H1Foreground();
                case 2:
                    return control.H2Foreground();
                case 3:
                    return control.H3Foreground();
                case 4:
                    return control.H4Foreground();
                case 5:
                    return control.H5Foreground();
                default:
                    return control.H6Foreground();
                }
            }(level, control));

            _paragraph.FontWeight([](int level, const auto& control)
                {
                switch (level)
                {
                case 1:
                    return control.H1FontWeight();
                case 2:
                    return control.H2FontWeight();
                case 3:
                    return control.H3FontWeight();
                case 4:
                    return control.H4FontWeight();
                case 5:
                    return control.H5FontWeight();
                default:
                    return control.H6FontWeight();
                }
            }(level, control));

            _paragraph.Margin([](int level, const auto& control) 
            {
                switch (level)
                {
                case 1:
                    return control.H1Margin();
                case 2:
                    return control.H2Margin();
                case 3:
                    return control.H3Margin();
                case 4:
                    return control.H4Margin();
                    break;
                case 5:
                    return control.H5Margin();
                default:
                    return control.H6Margin();
                }
            }(level, control));
        }

        void AddChild(IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<winrt::Inline>())
            {
                _paragraph.Inlines().Append(inlineChild);
            }
        }

    private:
        winrt::Paragraph _paragraph;
    };
}
