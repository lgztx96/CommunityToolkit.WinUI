// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdLineBreak final : public IAddChild
    {
    private:
        winrt::LineBreak _lineBreak;

    public:
        winrt::TextElement TextElement() const override
        {
            return _lineBreak;
        }
    };
}
