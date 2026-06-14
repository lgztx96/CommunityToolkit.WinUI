// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "ImageSourceBaseExtension.h"
#if __has_include("ImageSourceBaseExtension.g.cpp")
#include "ImageSourceBaseExtension.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
	winrt::Windows::Foundation::IInspectable ImageSourceBaseExtension::ProvideValue() { return nullptr; }

	winrt::Windows::Foundation::IInspectable ImageSourceBaseExtension::ProvideValue(
		[[maybe_unused]] winrt::Microsoft::UI::Xaml::IXamlServiceProvider const& provider)
	{
		return ProvideValue();
	}
}