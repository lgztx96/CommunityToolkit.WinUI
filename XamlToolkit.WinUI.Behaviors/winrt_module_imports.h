#pragma once

#ifdef __INTELLISENSE__
// Include headers directly for IntelliSense type resolution
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

#include <winrt/XamlToolkit.WinUI.Animations.h>
#include <winrt/XamlToolkit.WinUI.Interactivity.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#else
#define WINRT_IMPORT_MODULE
// WinRT namespace modules
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI.Core;
import winrt.Windows.UI.Xaml.Interop;

import winrt.Microsoft.UI.Dispatching;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Input;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Media;

import winrt.XamlToolkit.WinUI.Animations;
import winrt.XamlToolkit.WinUI.Interactivity;

#include <wil_cppwinrt_module.h>
#endif
