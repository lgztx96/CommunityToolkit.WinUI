// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "PipelineVisualFactory.h"
#if __has_include("PipelineVisualFactory.g.cpp")
#include "PipelineVisualFactory.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> PipelineVisualFactory::EffectsProperty =
		winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
			L"Effects",
			winrt::xaml_typename<winrt::Windows::Foundation::Collections::IVector<IPipelineEffect>>(),
			winrt::xaml_typename<class_type>(),
			nullptr);
}