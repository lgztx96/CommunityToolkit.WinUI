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
#include <cassert>
#include <functional>
#include <map>
#include <mutex>
#include <regex>
