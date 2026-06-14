// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#include "AttachedVisualFactoryBase.g.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#endif

import winrt.Windows.Foundation;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Composition;

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    /// <summary>
    /// A type responsible for creating <see cref="Visual"/> instances to attach to target elements.
    /// </summary>
    struct AttachedVisualFactoryBase : AttachedVisualFactoryBaseT<AttachedVisualFactoryBase>
    {
        AttachedVisualFactoryBase() = default;

        /// <summary>
        /// Creates a <see cref="Visual"/> to attach to the target element.
        /// </summary>
        /// <param name="element">The target <see cref="UIElement"/> the visual will be attached to.</param>
        /// <returns>A <see cref="Visual"/> instance that the caller will attach to the target element.</returns>
        virtual winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Composition::Visual> GetAttachedVisualAsync(winrt::Microsoft::UI::Xaml::UIElement const& element)
        {
			co_return nullptr;
        }
    };
}

namespace winrt::XamlToolkit::WinUI::Media::factory_implementation
{
    struct AttachedVisualFactoryBase : AttachedVisualFactoryBaseT<AttachedVisualFactoryBase, implementation::AttachedVisualFactoryBase>
    {
    };
}