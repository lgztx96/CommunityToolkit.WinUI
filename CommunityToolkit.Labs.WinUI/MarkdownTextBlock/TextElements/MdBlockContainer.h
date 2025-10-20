// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"
#include "MdFlowDocument.h"
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    class MdBlockContainer final : public IAddChild
    {
    private:
        InlineUIContainer _inlineUIContainer;
        MdFlowDocument _flowDocument;
        Paragraph _paragraph;

    public:

        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _paragraph;
        }

        MdBlockContainer()
        {
            _inlineUIContainer.Child(_flowDocument.RichTextBlock());
            _paragraph.Inlines().Append(_inlineUIContainer);
        }

        void AddChild(TextElements::IAddChild* child) override
        {
            _flowDocument.AddChild(child);
        }
    };
}

