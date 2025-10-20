// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include <MarkdownTextBlock/MarkdownConfig.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    using namespace winrt::Microsoft::UI::Xaml;

    class MdHeading final : public IAddChild
    {
    private:
        Paragraph _paragraph;
        MarkdownConfig _config;

    public:
        //bool IsHtml() const { return _htmlNode != nullptr; }

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdHeading(MarkdownConfig const& config) : _config(config)
        {
            auto level = 1;
            _paragraph.FontSize([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1FontSize();
                case 2:
                    return config.Themes().H2FontSize();
                case 3:
                    return config.Themes().H3FontSize();
                case 4:
                    return config.Themes().H4FontSize();
                case 5:
                    return config.Themes().H5FontSize();
                default:
                    return config.Themes().H6FontSize();
                }  
            }(level, _config));
            _paragraph.Foreground([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1Foreground();
                case 2:
                    return config.Themes().H2Foreground();
                case 3:
                    return config.Themes().H3Foreground();
                case 4:
                    return config.Themes().H4Foreground();
                case 5:
                    return config.Themes().H5Foreground();
                default:
                    return config.Themes().H6Foreground();
                }
                }(level, _config));
            _paragraph.FontWeight([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1FontWeight();
                case 2:
                    return config.Themes().H2FontWeight();
                case 3:
                    return config.Themes().H3FontWeight();
                case 4:
                    return config.Themes().H4FontWeight();
                case 5:
                    return config.Themes().H5FontWeight();
                default:
                    return config.Themes().H6FontWeight();
                }
            }(level, _config));
        }

        MdHeading(int level, MarkdownConfig const& config) : _config(config)
        {
            std::string align = ""; //_htmlNode.GetAttributeValue("align", "left");

            _paragraph.TextAlignment([&]
                {
                    if (align == "left")
                        return TextAlignment::Left;
                    else if (align == "right")
                        return TextAlignment::Right;
                    else if (align == "center")
                        return TextAlignment::Center;
                    else if (align == "justify")
                        return TextAlignment::Justify;
                    else
                        return TextAlignment::Left;
                }());

            //auto level = level;// int.Parse(htmlNode.Name.Substring(1));
            _paragraph.FontSize([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1FontSize();
                case 2:
                    return config.Themes().H2FontSize();
                case 3:
                    return config.Themes().H3FontSize();
                case 4:
                    return config.Themes().H4FontSize();
                case 5:
                    return config.Themes().H5FontSize();
                default:
                    return config.Themes().H6FontSize();
                }
                }(level, _config));
            _paragraph.Foreground([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1Foreground();
                case 2:
                    return config.Themes().H2Foreground();
                case 3:
                    return config.Themes().H3Foreground();
                case 4:
                    return config.Themes().H4Foreground();
                case 5:
                    return config.Themes().H5Foreground();
                default:
                    return config.Themes().H6Foreground();
                }
                }(level, _config));
            _paragraph.FontWeight([](int level, const auto& config) {
                switch (level)
                {
                case 1:
                    return config.Themes().H1FontWeight();
                    break;
                case 2:
                    return config.Themes().H2FontWeight();
                    break;
                case 3:
                    return config.Themes().H3FontWeight();
                    break;
                case 4:
                    return config.Themes().H4FontWeight();
                    break;
                case 5:
                    return config.Themes().H5FontWeight();
                    break;
                default:
                    return config.Themes().H6FontWeight();
                    break;
                }
                }(level, _config));
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            if (auto inlineChild = child->TextElement().try_as<Inline>())
            {
                _paragraph.Inlines().Append(inlineChild);
            }
        }
    };
}

