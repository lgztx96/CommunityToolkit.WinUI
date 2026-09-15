#pragma once

#include "CompositionTargetHelper.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#endif

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct CompositionTargetHelper
    {
        CompositionTargetHelper() = delete;

        static winrt::Windows::Foundation::IAsyncOperation<bool>
            ExecuteAfterCompositionRenderingAsync(CompositionRenderingCallback callback);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct CompositionTargetHelper : CompositionTargetHelperT<CompositionTargetHelper, implementation::CompositionTargetHelper>
    {
    };
}