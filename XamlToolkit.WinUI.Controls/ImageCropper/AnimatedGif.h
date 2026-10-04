#pragma once

// Cropping support for animated GIFs, kept apart from ImageCropper.Helpers.cpp because it is a good deal
// of WIC plumbing: frames are indexed, composite onto a canvas, and are re-encoded with their delays,
// disposal and transparency.

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Storage::Streams;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct AnimatedGif
	{
		AnimatedGif() = delete;

		// Crops the selection out of every frame of the source and writes the result to stream as a new GIF.
		//
		// Returns false when the source is not an animation - it has a single frame, or its canvas size cannot
		// be read - in which case nothing has been written and the caller falls back to the single frame path.
		// Anything that fails after that point throws.
		static bool TryCrop(winrt::IRandomAccessStream const& sourceStream, winrt::IRandomAccessStream const& stream, winrt::Rect const& croppedRect);
	};
}
