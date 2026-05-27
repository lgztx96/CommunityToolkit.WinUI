#pragma once

#ifdef __INTELLISENSE__
// Include headers directly for IntelliSense type resolution
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.Graphics.Imaging.h>

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>

#include <winrt/XamlToolkit.WinUI.h>
#include <winrt/XamlToolkit.WinUI.Converters.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#else
#define WINRT_IMPORT_MODULE
// WinRT namespace modules
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Metadata;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Windows.Graphics.Imaging;

import winrt.Microsoft.UI;
import winrt.Microsoft.UI.Dispatching;
import winrt.Microsoft.UI.Text;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Markup;
import winrt.Microsoft.UI.Xaml.Shapes;
import winrt.Microsoft.UI.Xaml.XamlTypeInfo;

import winrt.XamlToolkit.WinUI;
import winrt.XamlToolkit.WinUI.Converters;
import winrt.XamlToolkit.WinUI.Helpers;
import winrt.XamlToolkit.WinUI.Controls;

#include <wil_cppwinrt_module.h>
#endif