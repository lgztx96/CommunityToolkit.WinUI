#pragma once

#include "CaseCollection.g.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct CaseCollection : CaseCollectionT<CaseCollection>
    {
        CaseCollection() = default;
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct CaseCollection : CaseCollectionT<CaseCollection, implementation::CaseCollection>
    {
    };
}
