// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "IAddChild.h"

namespace winrt::CommunityToolkit::Labs::WinUI::TextElements 
{
    class MdLineBreak final : public IAddChild
    {
    private:
        LineBreak _lineBreak;

    public:
        Microsoft::UI::Xaml::Documents::TextElement TextElement() const override
        {
            return _lineBreak;
        }
    };
}
