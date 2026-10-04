#pragma once

// Profiler tracing (ETW) for the XamlToolkit.
//
// NOTE: This file is duplicated in each instrumented toolkit project. Keep the provider name and
// GUID identical in all copies so a single ETW session captures events from every DLL.
//
// Writing an event:
//
//     XAMLTOOLKIT_TRACE("SaveCompleted",
//         TraceLoggingUInt32(bytesWritten, "Bytes"),
//         TraceLoggingUInt32(pixelWidth, "PixelWidth"));
//
// Event and field names are narrow UTF-8 literals. Event arguments are evaluated only when the
// provider is enabled.
//
// Object creation/destruction:
//
//     ImageCropper::ImageCropper()
//     {
//         ...;
//         XAMLTOOLKIT_TRACE_OBJECT_CREATED(winrt::name_of<class_type>());
//     }
//
//     ImageCropper::~ImageCropper()
//     {
//         XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(winrt::name_of<class_type>());
//     }
//
// No additional members are required, so tracing does not change the class layout. Creation should
// be reported as the last statement of the constructor body, and destruction as the first statement
// of the destructor body.
//
// Object lifetime events:
//     ObjectCreated / ObjectDestroyed
//         Object      - runtime class name
//         Instance    - object address, used to match creation and destruction
//         ThreadId    - thread that emitted the event
//         LiveObjects - outstanding instances in the current module
//
// Lifetime duration is determined from the timestamps of the creation and destruction events.
// Avoid adding user data such as URIs, paths, or text to event payloads.

#include <cstdint>
#include <string_view>

#include <windows.h>
#include <winmeta.h>
#include <TraceLoggingProvider.h>

#pragma comment(lib, "advapi32.lib")

namespace winrt::XamlToolkit::WinUI::Diagnostics
{
	namespace details
	{
		inline TraceLoggingHProvider Provider() noexcept
		{
			static TraceLoggingHProvider const provider = []() noexcept
			{
				TRACELOGGING_DEFINE_PROVIDER_STORAGE(g_storage, "XamlToolkit-WinUI-Profiler", (0x099ac36f, 0xe0dd, 0x4795, 0xbc, 0x1e, 0x85, 0x2a, 0xa5, 0x4e, 0xf0, 0x8f));

				TraceLoggingRegister(&g_storage);

				return &g_storage;
			}();

			return provider;
		}

		inline volatile long long g_liveObjects{ 0 };
	}

	struct ToolkitProfilerTracing
	{
		ToolkitProfilerTracing() = delete;

		static TraceLoggingHProvider Provider() noexcept
		{
			return details::Provider();
		}

		static void ObjectCreated(std::wstring_view objectName, void const* instance) noexcept
		{
			TraceLoggingWrite(
				details::Provider(),
				"ObjectCreated",
				TraceLoggingLevel(WINEVENT_LEVEL_INFO),
				TraceLoggingCountedWideString(objectName.data(), static_cast<std::uint16_t>(objectName.size()), "Object"),
				TraceLoggingPointer(instance, "Instance"),
				TraceLoggingUInt32(GetCurrentThreadId(), "ThreadId"),
				TraceLoggingInt64(InterlockedIncrement64(&details::g_liveObjects), "LiveObjects"));
		}

		static void ObjectDestroyed(std::wstring_view objectName, void const* instance) noexcept
		{
			TraceLoggingWrite(
				details::Provider(),
				"ObjectDestroyed",
				TraceLoggingLevel(WINEVENT_LEVEL_INFO),
				TraceLoggingCountedWideString(objectName.data(), static_cast<std::uint16_t>(objectName.size()), "Object"),
				TraceLoggingPointer(instance, "Instance"),
				TraceLoggingUInt32(GetCurrentThreadId(), "ThreadId"),
				TraceLoggingInt64(InterlockedDecrement64(&details::g_liveObjects), "LiveObjects"));
		}
	};
}

// Any event on the toolkit provider, level Informational; one or more fields are required.
#define XAMLTOOLKIT_TRACE(eventName, ...) \
	TraceLoggingWrite(::winrt::XamlToolkit::WinUI::Diagnostics::ToolkitProfilerTracing::Provider(), eventName, TraceLoggingLevel(WINEVENT_LEVEL_INFO), __VA_ARGS__)

// Same, with an explicit level (WINEVENT_LEVEL_ERROR / WINEVENT_LEVEL_WARNING / WINEVENT_LEVEL_VERBOSE).
#define XAMLTOOLKIT_TRACE_AT(level, eventName, ...) \
	TraceLoggingWrite(::winrt::XamlToolkit::WinUI::Diagnostics::ToolkitProfilerTracing::Provider(), eventName, TraceLoggingLevel(level), __VA_ARGS__)

// For the last line of a constructor body / the first line of a destructor body; both take the
// same name. `this` is the object's address.
#define XAMLTOOLKIT_TRACE_OBJECT_CREATED(name) \
	::winrt::XamlToolkit::WinUI::Diagnostics::ToolkitProfilerTracing::ObjectCreated(name, this)

#define XAMLTOOLKIT_TRACE_OBJECT_DESTROYED(name) \
	::winrt::XamlToolkit::WinUI::Diagnostics::ToolkitProfilerTracing::ObjectDestroyed(name, this)
