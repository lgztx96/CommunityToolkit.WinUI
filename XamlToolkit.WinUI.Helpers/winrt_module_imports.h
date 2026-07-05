#pragma once

#ifdef __INTELLISENSE__
// Include headers directly for IntelliSense type resolution
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.ApplicationModel.h>;

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Xaml.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#else
#define WINRT_IMPORT_MODULE
// WinRT namespace modules
import winrt.Windows.Media;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Metadata;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI.Xaml.Interop;
import winrt.Windows.ApplicationModel;

import winrt.Microsoft.UI;
import winrt.Microsoft.UI.Xaml;

import winrt.XamlToolkit.WinUI;

#include <wil_cppwinrt_module.h>

#endif
