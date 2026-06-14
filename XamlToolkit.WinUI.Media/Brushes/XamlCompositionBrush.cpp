// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "XamlCompositionBrush.h"
#if __has_include("XamlCompositionBrush.g.cpp")
#include "XamlCompositionBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    XamlCompositionBrush::XamlCompositionBrush(Pipelines::PipelineBuilder const& pipeline)
        : Pipeline(pipeline)
    {
    }

    Pipelines::PipelineBuilder XamlCompositionBrush::OnPipelineRequested()
    {
        return Pipeline();
    }

	winrt::XamlToolkit::WinUI::Media::XamlCompositionBrush XamlCompositionBrush::Clone()
	{
		return winrt::make<XamlCompositionBrush>(Pipeline());
	}
}