// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>
#else
import std;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.UI;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Xaml.Media;
import winrt.XamlToolkit.WinUI;
import winrt.XamlToolkit.WinUI.Controls;
#endif

namespace winrt
{
    using namespace Windows::UI;
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Microsoft::UI::Xaml::Controls;
	using namespace XamlToolkit::WinUI;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	struct ColorPickerRenderingHelpers
	{
		static winrt::Color CheckerBackgroundColor();

		static winrt::IAsyncOperation<winrt::IVector<std::uint8_t>> CreateChannelBitmapAsync(
			int width,
			int height,
			winrt::Orientation orientation,
			ColorRepresentation colorRepresentation,
			ColorChannel channel,
			HsvColor hsvColor,
			std::optional<winrt::Color> checkerColor,
			bool isAlphaMaxForced,
			bool isSaturationValueMaxForced);

		static winrt::IAsyncOperation<winrt::IVector<std::uint8_t>> CreateCheckeredBitmapAsync(int width, int height, winrt::Color checkerColor);

		static winrt::ImageBrush BitmapToBrush(winrt::IVector<std::uint8_t> const& bitmap, int width, int height);

		static winrt::IAsyncAction UpdateBorderBackgroundWithCheckerAsync(winrt::Border border, winrt::Color color);
	};
}
