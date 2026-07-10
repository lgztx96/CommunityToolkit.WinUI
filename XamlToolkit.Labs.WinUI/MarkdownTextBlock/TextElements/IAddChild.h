// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <string_view>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml::Documents;
}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
    class IAddChild
    {
    public:
        using Block = winrt::Microsoft::UI::Xaml::Documents::Block;
        using Inline = winrt::Microsoft::UI::Xaml::Documents::Inline;

        virtual winrt::TextElement TextElement() const = 0;
        virtual void Enter() {};
        virtual void AddInlineText([[maybe_unused]] std::wstring_view text) {};
        virtual void Leave() {};
        virtual void AddChild([[maybe_unused]] IAddChild* child) {};
        virtual ~IAddChild() {};
    };
}

