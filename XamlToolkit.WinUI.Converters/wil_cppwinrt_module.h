#pragma once

#pragma warning(push)
#pragma warning(disable : 4348)

import winrt_base;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;
import winrt.Windows.System;
import winrt.Windows.UI.Core;

#define WINRT_IMPORT_MODULE
#define WINRT_BASE_H
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Core.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

#pragma warning(pop)
