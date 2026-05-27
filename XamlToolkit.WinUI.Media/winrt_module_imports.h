#pragma once

#ifdef __INTELLISENSE__
// Include headers directly for IntelliSense type resolution
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>

#include <winrt/Microsoft.Graphics.Canvas.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#else
#define WINRT_IMPORT_MODULE
// WinRT namespace modules
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI.Xaml.Interop;

import winrt.Microsoft.UI;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Markup;

import winrt.Microsoft.Graphics.Canvas;

#include <wil_cppwinrt_module.h>

#endif
