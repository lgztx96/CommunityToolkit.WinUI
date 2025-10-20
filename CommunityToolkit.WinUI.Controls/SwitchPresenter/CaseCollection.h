#pragma once

#include "CaseCollection.g.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct CaseCollection : CaseCollectionT<CaseCollection>
    {
        CaseCollection() = default;
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct CaseCollection : CaseCollectionT<CaseCollection, implementation::CaseCollection>
    {
    };
}
