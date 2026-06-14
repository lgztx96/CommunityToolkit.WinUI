// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "BackdropInvertBrush.h"
#if __has_include("BackdropInvertBrush.g.cpp")
#include "BackdropInvertBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    Pipelines::PipelineBuilder BackdropInvertBrush::OnPipelineRequested()
    {
        return Pipelines::PipelineBuilder::FromBackdrop().Invert();
    }
}