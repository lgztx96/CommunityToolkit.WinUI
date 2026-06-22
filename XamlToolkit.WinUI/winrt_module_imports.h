#pragma once

#ifdef __INTELLISENSE__
// Include headers directly for IntelliSense type resolution
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Text.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Hosting.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Windows.System.Profile.h>
#include <winrt/Windows.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Windows.ApplicationModel.DataTransfer.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#else
#define WINRT_IMPORT_MODULE
// WinRT namespace modules
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.Foundation.Numerics;
import winrt.Windows.UI.Xaml.Interop;

import winrt.Microsoft.UI;
import winrt.Microsoft.UI.Composition;
import winrt.Microsoft.UI.Text;
import winrt.Microsoft.UI.Xaml;
import winrt.Microsoft.UI.Xaml.Hosting;
import winrt.Microsoft.UI.Xaml.Interop;
import winrt.Microsoft.UI.Xaml.Markup;
import winrt.Windows.System.Profile;
import winrt.Windows.UI.Input;
import winrt.Microsoft.UI.Xaml.Controls;
import winrt.Microsoft.UI.Xaml.Shapes;
import winrt.Windows.ApplicationModel.DataTransfer;

#include <wil_cppwinrt_module.h>

#endif
