// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <string_view>
#endif

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class MdInlineText final : public IAddChild
    {
    private:
        Run _run;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _run;
        }

        MdInlineText(std::wstring_view text)
        {
            _run.Text(winrt::hstring(text));
        }
    };
}
