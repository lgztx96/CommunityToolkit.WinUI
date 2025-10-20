// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include <winrt/Microsoft.UI.Xaml.Documents.h>

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements
{
    using namespace winrt;
    using namespace Microsoft::UI::Xaml::Documents;

    class IAddChild
    {
    public:

        using Block = Microsoft::UI::Xaml::Documents::Block;
        using Inline = Microsoft::UI::Xaml::Documents::Inline;

        virtual TextElement TextElement() const = 0;
        virtual void Enter() {};
        virtual void AddInlineText([[maybe_unused]] std::wstring_view text) {};
        virtual void Leave() {};
        virtual void AddChild([[maybe_unused]] TextElements::IAddChild* child) {};
        virtual ~IAddChild() {};
    };
}

