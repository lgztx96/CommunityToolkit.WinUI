#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOMCX
#define NOHELP
#define NOCOMM
#include <windows.h>

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

#include <algorithm>
#include <cmath>
#include <format>
#include <generator>
#include <iterator>
#include <memory>
#include <regex>
#include <string>
#include <string_view>