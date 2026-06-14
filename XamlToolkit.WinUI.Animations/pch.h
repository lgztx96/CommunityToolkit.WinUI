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

// STL headers must be included before import std; to avoid redefinition errors
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <string_view>
#include <istream>
#include <iomanip>

#include <DirectXMath.h>
