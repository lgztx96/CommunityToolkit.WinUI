#include "pch.h"
#include "winrt_module_imports.h"
#include <shcore.h> // CreateStreamOverRandomAccessStream
#include <wincodec.h>
#ifdef __INTELLISENSE__
#include <algorithm>
#include <limits>
#include <map>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#endif
#include "AnimatedGif.h"

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Storage::Streams;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	static winrt::com_ptr<::IStream> ToIStream(winrt::IRandomAccessStream const& stream)
	{
		winrt::com_ptr<::IStream> result;
		winrt::check_hresult(::CreateStreamOverRandomAccessStream(static_cast<::IUnknown*>(winrt::get_abi(stream)), __uuidof(::IStream), result.put_void()));
		return result;
	}

	static constexpr UINT16 DefaultDelay = 10;

	static constexpr UINT32 DisposalNone = 0;
	static constexpr UINT32 DisposalBackground = 2;
	static constexpr UINT32 DisposalPrevious = 3;

	struct PropVariant
	{
		PROPVARIANT value{};
		PropVariant() { PropVariantInit(&value); }

		~PropVariant() { PropVariantClear(&value); }

		PropVariant(const PropVariant&) = delete;
		PropVariant& operator=(const PropVariant&) = delete;
		PROPVARIANT* operator&() { return &value; }

		operator PROPVARIANT*() { return &value; }
	};

	static UINT32 ReadGifMetadata(::IWICMetadataQueryReader* reader, LPCWSTR name, UINT32 fallback)
	{
		if (!reader)
		{
			return fallback;
		}

		PropVariant property;
		if (FAILED(reader->GetMetadataByName(name, &property.value)))
		{
			return fallback;
		}

		switch (property.value.vt)
		{
		case VT_BOOL:
			return property.value.boolVal == VARIANT_FALSE ? 0 : 1;
		case VT_UI1:
			return property.value.bVal;
		case VT_UI2:
			return property.value.uiVal;
		case VT_UI4:
			return property.value.ulVal;
		case VT_I1:
			return static_cast<UINT32>(property.value.cVal);
		case VT_I2:
			return static_cast<UINT32>(property.value.iVal);
		case VT_I4:
			return static_cast<UINT32>(property.value.lVal);
		default:
			return fallback;
		}
	}

	static void SetGifMetadataWord(::IWICMetadataQueryWriter* writer, LPCWSTR name, UINT16 value)
	{
		PropVariant property;
		property.value.vt = VT_UI2;
		property.value.uiVal = value;
		winrt::check_hresult(writer->SetMetadataByName(name, &property.value));
	}

	static void SetGifMetadataByte(::IWICMetadataQueryWriter* writer, LPCWSTR name, BYTE value)
	{
		PropVariant property;
		property.value.vt = VT_UI1;
		property.value.bVal = value;
		winrt::check_hresult(writer->SetMetadataByName(name, &property.value));
	}

	static void SetGifMetadataBool(::IWICMetadataQueryWriter* writer, LPCWSTR name, bool value)
	{
		PropVariant property;
		property.value.vt = VT_BOOL;
		property.value.boolVal = value ? VARIANT_TRUE : VARIANT_FALSE;
		winrt::check_hresult(writer->SetMetadataByName(name, &property.value));
	}

	static winrt::com_ptr<::IWICMetadataQueryReader> GetGifContainerMetadata(::IWICBitmapDecoder* decoder, ::IWICBitmapFrameDecode* frame)
	{
		winrt::com_ptr<::IWICMetadataQueryReader> reader;
		if (FAILED(decoder->GetMetadataQueryReader(reader.put())) || !reader)
		{
			if (frame)
			{
				frame->GetMetadataQueryReader(reader.put());
			}
		}

		return reader;
	}

	static winrt::com_ptr<::IWICBitmap> CreateCanvas(::IWICImagingFactory* factory, UINT32 width, UINT32 height)
	{
		winrt::com_ptr<::IWICBitmap> canvas;
		winrt::check_hresult(factory->CreateBitmap(width, height, GUID_WICPixelFormat32bppBGRA, WICBitmapCacheOnLoad, canvas.put()));
		return canvas;
	}

	// Reads the GIF logical screen size.
	static bool GetGifCanvasSize(::IWICBitmapDecoder* decoder, UINT32& width, UINT32& height)
	{
		width = 0;
		height = 0;
		winrt::com_ptr<::IWICBitmapFrameDecode> frame;
		winrt::check_hresult(decoder->GetFrame(0, frame.put()));

		// The logical screen descriptor is container metadata, which lives on the decoder; some decoders
		// also expose it per frame, so both readers are tried before giving up on the path.
		if (const auto reader = GetGifContainerMetadata(decoder, frame.get()))
		{
			width = ReadGifMetadata(reader.get(), L"/logscrdesc/Width", 0);
			height = ReadGifMetadata(reader.get(), L"/logscrdesc/Height", 0);
		}

		if (width == 0 || height == 0)
		{
			// The screen descriptor could not be read, so the canvas becomes the area the frames actually
			// cover. Taking frame 0 alone would be wrong: an optimised GIF often starts with a small frame
			// and later frames sit beyond it, and everything outside the canvas is dropped.
			UINT frameCount = 0;
			winrt::check_hresult(decoder->GetFrameCount(&frameCount));
			width = 0;
			height = 0;
			for (UINT index = 0; index < frameCount; ++index)
			{
				winrt::com_ptr<::IWICBitmapFrameDecode> current;
				winrt::check_hresult(decoder->GetFrame(index, current.put()));
				UINT frameWidth = 0;
				UINT frameHeight = 0;
				winrt::check_hresult(current->GetSize(&frameWidth, &frameHeight));
				winrt::com_ptr<::IWICMetadataQueryReader> reader;
				current->GetMetadataQueryReader(reader.put());
				const auto left = ReadGifMetadata(reader.get(), L"/imgdesc/Left", 0);
				const auto top = ReadGifMetadata(reader.get(), L"/imgdesc/Top", 0);
				width = std::max<UINT32>(width, left + frameWidth);
				height = std::max<UINT32>(height, top + frameHeight);
			}
		}

		return width != 0 && height != 0;
	}

	// Whether any frame declares a transparent index. Only then is the background transparent: viewers
	// treat "restore to background" as transparent whenever the GIF uses transparency, and a GIF whose
	// first frame happens to be opaque can still introduce transparency later on.
	static bool GifUsesTransparency(::IWICBitmapDecoder* decoder)
	{
		UINT frameCount = 0;
		winrt::check_hresult(decoder->GetFrameCount(&frameCount));
		for (UINT index = 0; index < frameCount; ++index)
		{
			winrt::com_ptr<::IWICBitmapFrameDecode> frame;
			winrt::check_hresult(decoder->GetFrame(index, frame.put()));
			winrt::com_ptr<::IWICMetadataQueryReader> reader;
			frame->GetMetadataQueryReader(reader.put());
			if (ReadGifMetadata(reader.get(), L"/grctlext/TransparencyFlag", 0) != 0)
			{
				return true;
			}
		}

		return false;
	}

	// Returns the color represented by the GIF logical-screen background.
	//
	// WICColor is 0xAARRGGBB.
	// The canvas uses 32bpp BGRA.
	static UINT32 ReadGifBackgroundColor(::IWICImagingFactory* factory, ::IWICBitmapDecoder* decoder)
	{
		winrt::com_ptr<::IWICBitmapFrameDecode> frame;
		winrt::check_hresult(decoder->GetFrame(0, frame.put()));
		if (GifUsesTransparency(decoder))
		{
			return 0;
		}

		// The background colour index sits in the logical screen descriptor, i.e. container metadata.
		const auto reader = GetGifContainerMetadata(decoder, frame.get());
		if (!reader)
		{
			return 0;
		}

		const auto backgroundIndex = ReadGifMetadata(reader.get(), L"/logscrdesc/BackgroundColorIndex", 0);
		winrt::com_ptr<::IWICPalette> palette;
		winrt::check_hresult(factory->CreatePalette(palette.put()));
		if (FAILED(frame->CopyPalette(palette.get())))
		{
			return 0;
		}

		UINT paletteSize = 0;
		winrt::check_hresult(palette->GetColorCount(&paletteSize));
		if (backgroundIndex >= paletteSize)
		{
			return 0;
		}

		std::vector<WICColor> colors(paletteSize);
		UINT written = 0;
		winrt::check_hresult(palette->GetColors(paletteSize, colors.data(), &written));
		if (backgroundIndex >= written)
		{
			return 0;
		}

		// WICColor is 0xAARRGGBB, the canvas is BGRA, and the
		// background of an opaque GIF counts as opaque.
		const auto color = colors[backgroundIndex];
		return ((color & 0x0000FF00) << 16) | ((color & 0x00FF0000) >> 16) | ((color & 0x000000FF) << 24) | 0x000000FF;
	}

	static void FillCanvas(::IWICBitmap* canvas, WICRect const* region, UINT32 fill)
	{
		winrt::com_ptr<::IWICBitmapLock> lock;
		winrt::check_hresult(canvas->Lock(region, WICBitmapLockWrite, lock.put()));
		UINT stride = 0;
		UINT size = 0;
		BYTE* data = nullptr;
		winrt::check_hresult(lock->GetStride(&stride));
		winrt::check_hresult(lock->GetDataPointer(&size, &data));
		const UINT rows = region ? static_cast<UINT>(region->Height) : size / stride;
		const size_t rowBytes = region ? static_cast<size_t>(region->Width) * 4 : stride;
		for (UINT row = 0; row < rows; ++row)
		{
			auto* destination = data + static_cast<size_t>(row) * stride;
			for (size_t x = 0; x < rowBytes; x += 4)
			{
				std::memcpy(destination + x, &fill, sizeof(fill));
			}
		}
	}

	static void SnapshotCanvas(::IWICBitmap* canvas, std::vector<uint8_t>& snapshot, UINT32 width, UINT32 height)
	{
		const size_t rowBytes = static_cast<size_t>(width) * 4;
		snapshot.resize(rowBytes * height);
		winrt::com_ptr<::IWICBitmapLock> lock;
		winrt::check_hresult(canvas->Lock(nullptr, WICBitmapLockRead, lock.put()));
		UINT stride = 0;
		UINT size = 0;
		BYTE* data = nullptr;
		winrt::check_hresult(lock->GetStride(&stride));
		winrt::check_hresult(lock->GetDataPointer(&size, &data));
		for (UINT row = 0; row < height; ++row)
		{
			std::memcpy(snapshot.data() + static_cast<size_t>(row) * rowBytes, data + static_cast<size_t>(row) * stride, rowBytes);
		}
	}

	static void RestoreCanvas(::IWICBitmap* canvas, std::vector<uint8_t> const& snapshot, UINT32 width, UINT32 height)
	{
		const size_t rowBytes = static_cast<size_t>(width) * 4;
		if (snapshot.size() < rowBytes * height)
		{
			return;
		}

		winrt::com_ptr<::IWICBitmapLock> lock;
		winrt::check_hresult(canvas->Lock(nullptr, WICBitmapLockWrite, lock.put()));
		UINT stride = 0;
		UINT size = 0;
		BYTE* data = nullptr;
		winrt::check_hresult(lock->GetStride(&stride));
		winrt::check_hresult(lock->GetDataPointer(&size, &data));
		for (UINT row = 0; row < height; ++row)
		{
			std::memcpy(data + static_cast<size_t>(row) * stride, snapshot.data() + static_cast<size_t>(row) * rowBytes, rowBytes);
		}
	}

	static void DrawFrameOntoCanvas(::IWICBitmapSource* frame, ::IWICBitmap* canvas, UINT32 left, UINT32 top, UINT32 width, UINT32 height, std::vector<WICColor> const& palette,
									UINT32 transparentIndex, std::vector<uint8_t>& indices)
	{
		const size_t rowBytes = static_cast<size_t>(width);
		indices.resize(rowBytes * height);

		// Only the part of the frame that fits on the canvas is copied, which is what the buffer holds.
		const WICRect sourceRegion{ .X = 0, .Y = 0, .Width = static_cast<INT>(width), .Height = static_cast<INT>(height) };
		winrt::check_hresult(frame->CopyPixels(&sourceRegion, static_cast<UINT>(rowBytes), static_cast<UINT>(indices.size()), indices.data()));
		const WICRect region{ .X = static_cast<INT>(left), .Y = static_cast<INT>(top), .Width = static_cast<INT>(width), .Height = static_cast<INT>(height) };
		winrt::com_ptr<::IWICBitmapLock> lock;
		winrt::check_hresult(canvas->Lock(&region, WICBitmapLockWrite, lock.put()));
		UINT stride = 0;
		UINT size = 0;
		BYTE* data = nullptr;
		winrt::check_hresult(lock->GetStride(&stride));
		winrt::check_hresult(lock->GetDataPointer(&size, &data));
		for (UINT row = 0; row < height; ++row)
		{
			const auto* source = indices.data() + static_cast<size_t>(row) * rowBytes;
			auto* destination = data + static_cast<size_t>(row) * stride;
			for (UINT column = 0; column < width; ++column, destination += 4)
			{
				const auto index = static_cast<UINT32>(source[column]);

				// The transparent index paints nothing, exactly as a viewer leaves the canvas alone.
				if (index == transparentIndex || index >= palette.size())
				{
					continue;
				}

				// WICColor is 0xAARRGGBB, the canvas is BGRA, and GIF palette entries are opaque.
				const auto color = palette[index];
				destination[0] = static_cast<BYTE>(color);
				destination[1] = static_cast<BYTE>(color >> 8);
				destination[2] = static_cast<BYTE>(color >> 16);
				destination[3] = 0xFF;
			}
		}
	}

	// crop stays mutable: IWICBitmapFrameEncode::WriteSource takes a WICRect*, not a const one.
	static void WriteAnimatedGifFrames(::IWICImagingFactory* factory, ::IWICBitmapDecoder* decoder, ::IWICBitmapEncoder* encoder, UINT frameCount, WICRect& crop, UINT32 canvasWidth,
									   UINT32 canvasHeight)
	{
		const auto background = ReadGifBackgroundColor(factory, decoder);
		const auto canvas = CreateCanvas(factory, canvasWidth, canvasHeight);

		// Start with the GIF logical-screen background.
		FillCanvas(canvas.get(), nullptr, background);

		// Reused across frames so the frame's indices are not reallocated every time.
		std::vector<uint8_t> frameIndices;
		std::vector<uint8_t> snapshot;

		// The cropped region of the canvas: each frame's palette and indices are built from it.
		std::vector<uint8_t> cropPixels;
		UINT32 previousDisposal = DisposalNone;
		WICRect previousRegion{};
		for (UINT index = 0; index < frameCount; ++index)
		{
			// Apply the disposal method of the
			// previous frame before drawing this frame.
			if (previousDisposal == DisposalBackground)
			{
				if (previousRegion.Width > 0 && previousRegion.Height > 0)
				{
					FillCanvas(canvas.get(), &previousRegion, background);
				}
			}
			else if (previousDisposal == DisposalPrevious)
			{
				if (!snapshot.empty())
				{
					RestoreCanvas(canvas.get(), snapshot, canvasWidth, canvasHeight);
				}
			}

			winrt::com_ptr<::IWICBitmapFrameDecode> sourceFrame;
			winrt::check_hresult(decoder->GetFrame(index, sourceFrame.put()));
			UINT frameWidth = 0;
			UINT frameHeight = 0;
			winrt::check_hresult(sourceFrame->GetSize(&frameWidth, &frameHeight));
			winrt::com_ptr<::IWICMetadataQueryReader> reader;
			sourceFrame->GetMetadataQueryReader(reader.put());
			const auto left = ReadGifMetadata(reader.get(), L"/imgdesc/Left", 0);
			const auto top = ReadGifMetadata(reader.get(), L"/imgdesc/Top", 0);
			const auto disposal = ReadGifMetadata(reader.get(), L"/grctlext/Disposal", 0);
			const bool hasTransparency = ReadGifMetadata(reader.get(), L"/grctlext/TransparencyFlag", 0) != 0;

			// The transparent index only counts when the frame says it has one; some encoders leave a
			// stale index behind. Anything else keeps the canvas untouched for that pixel.
			auto transparentIndex = UINT32_MAX;
			if (hasTransparency)
			{
				transparentIndex = ReadGifMetadata(reader.get(), L"/grctlext/TransparentColorIndex", UINT32_MAX);
			}

			// A frame may carry its own palette, so it is read per frame rather than once. It is also handed
			// to the encoder below, which keeps the colours exact and makes the transparent index mean the
			// same entry to both sides.
			std::vector<WICColor> palette;
			winrt::com_ptr<::IWICPalette> framePalette;
			winrt::check_hresult(factory->CreatePalette(framePalette.put()));
			if (SUCCEEDED(sourceFrame->CopyPalette(framePalette.get())))
			{
				UINT paletteSize = 0;
				if (SUCCEEDED(framePalette->GetColorCount(&paletteSize)) && paletteSize > 0)
				{
					palette.resize(paletteSize);
					UINT written = 0;
					if (FAILED(framePalette->GetColors(paletteSize, palette.data(), &written)))
					{
						palette.clear();
					}
					else
					{
						palette.resize(written);
					}
				}
			}

			const auto delay = static_cast<UINT16>(ReadGifMetadata(reader.get(), L"/grctlext/Delay", DefaultDelay));

			// Clip the source frame to the logical GIF canvas.
			const UINT32 drawWidth = std::min<UINT32>(frameWidth, canvasWidth > left ? canvasWidth - left : 0);
			const UINT32 drawHeight = std::min<UINT32>(frameHeight, canvasHeight > top ? canvasHeight - top : 0);

			// Disposal = 3 means restore to the canvas
			// as it was before this frame was drawn.
			if (disposal == DisposalPrevious)
			{
				SnapshotCanvas(canvas.get(), snapshot, canvasWidth, canvasHeight);
			}

			if (drawWidth > 0 && drawHeight > 0)
			{
				DrawFrameOntoCanvas(sourceFrame.get(), canvas.get(), left, top, drawWidth, drawHeight, palette, transparentIndex, frameIndices);
			}

			// The cropped region of the canvas: it decides the frame's palette and, later, how much the
			// frame changed.
			const auto cropRowBytes = static_cast<size_t>(crop.Width) * 4;
			cropPixels.resize(cropRowBytes * static_cast<size_t>(crop.Height));
			winrt::check_hresult(canvas->CopyPixels(&crop, static_cast<UINT>(cropRowBytes), static_cast<UINT>(cropPixels.size()), cropPixels.data()));

			// The palette and the indices are built here rather than left to the encoder, which is what makes
			// the transparent index below mean something: a BGRA frame is quantized into a palette of the
			// encoder's own choosing, so an index written into the Graphic Control Extension would refer to
			// a different colour than it does to us. This follows the approach PhotoSauce.MagicScaler takes -
			// own the palette, keep the transparent colour at a known slot (here: the last entry), and hand
			// the encoder indexed pixels plus all four /grctlext properties in one pass.
			std::vector<WICColor> outputColors;
			std::map<WICColor, BYTE> outputIndexes;
			bool cropHasTransparency = false;
			for (size_t pixel = 0; pixel + 4 <= cropPixels.size(); pixel += 4)
			{
				if (cropPixels[pixel + 3] == 0)
				{
					cropHasTransparency = true;
					continue;
				}

				const auto color =
					static_cast<WICColor>((static_cast<WICColor>(cropPixels[pixel + 2]) << 16) | (static_cast<WICColor>(cropPixels[pixel + 1]) << 8) | static_cast<WICColor>(cropPixels[pixel]));

				// One slot is kept free for the transparent colour, whether or not this crop uses it.
				if (outputIndexes.find(color) == outputIndexes.end() && outputColors.size() < 255)
				{
					outputIndexes.emplace(color, static_cast<BYTE>(outputColors.size()));
					outputColors.push_back(0xFF000000 | color);
				}
			}

			auto declaredIndex = UINT32_MAX;
			auto opaqueColors = outputColors.size();
			if (cropHasTransparency)
			{
				declaredIndex = static_cast<UINT32>(outputColors.size());
				outputColors.push_back(0x00000000);
			}

			if (outputColors.empty())
			{
				// A fully transparent crop has nothing to draw; the frame is written empty instead of
				// failing, and the transparent entry covers it.
				opaqueColors = 0;
				declaredIndex = 0;
				outputColors.push_back(0x00000000);
			}

			std::vector<uint8_t> cropIndices(cropPixels.size() / 4);
			for (size_t pixel = 0, entry = 0; pixel + 4 <= cropPixels.size(); pixel += 4, ++entry)
			{
				if (cropPixels[pixel + 3] == 0)
				{
					cropIndices[entry] = static_cast<uint8_t>(declaredIndex);
					continue;
				}

				const auto color =
					static_cast<WICColor>((static_cast<WICColor>(cropPixels[pixel + 2]) << 16) | (static_cast<WICColor>(cropPixels[pixel + 1]) << 8) | static_cast<WICColor>(cropPixels[pixel]));
				if (const auto found = outputIndexes.find(color); found != outputIndexes.end())
				{
					cropIndices[entry] = found->second;
					continue;
				}

				// The palette filled up: this frame has more colours than a GIF can hold, so the closest
				// entry is used and remembered, which keeps the cost to one scan per distinct colour. The
				// transparent entry is not a candidate - matching it would make a real colour disappear.
				auto closest = BYTE{ 0 };
				auto closestDistance = std::numeric_limits<uint32_t>::max();
				for (size_t candidate = 0; candidate < opaqueColors; ++candidate)
				{
					const auto difference = outputColors[candidate] ^ color;
					const auto distance =
						((difference >> 16) & 0xFF) * ((difference >> 16) & 0xFF) + ((difference >> 8) & 0xFF) * ((difference >> 8) & 0xFF) + (difference & 0xFF) * (difference & 0xFF);
					if (distance < closestDistance)
					{
						closestDistance = distance;
						closest = static_cast<BYTE>(candidate);
					}
				}

				outputIndexes.emplace(color, closest);
				cropIndices[entry] = closest;
			}

			winrt::com_ptr<::IWICPalette> outputPalette;
			winrt::check_hresult(factory->CreatePalette(outputPalette.put()));
			winrt::check_hresult(outputPalette->InitializeCustom(outputColors.data(), static_cast<UINT>(outputColors.size())));
			winrt::com_ptr<::IWICBitmapFrameEncode> frameEncode;
			winrt::com_ptr<::IPropertyBag2> options;
			winrt::check_hresult(encoder->CreateNewFrame(frameEncode.put(), options.put()));
			winrt::check_hresult(frameEncode->Initialize(options.get()));
			winrt::check_hresult(frameEncode->SetSize(static_cast<UINT>(crop.Width), static_cast<UINT>(crop.Height)));
			WICPixelFormatGUID format = GUID_WICPixelFormat8bppIndexed;
			winrt::check_hresult(frameEncode->SetPixelFormat(&format));
			winrt::check_hresult(frameEncode->SetPalette(outputPalette.get()));

			// All four properties in one pass, before the pixels: the encoder serialises the whole Graphic
			// Control Extension from its property store once any of them is set, so a field left out comes
			// out as zero. This is also the only window in which the store is live.
			winrt::com_ptr<::IWICMetadataQueryWriter> writer;
			if (SUCCEEDED(frameEncode->GetMetadataQueryWriter(writer.put())))
			{
				SetGifMetadataWord(writer.get(), L"/grctlext/Delay", delay);
				SetGifMetadataByte(writer.get(), L"/grctlext/Disposal", static_cast<BYTE>(DisposalBackground));
				if (declaredIndex != UINT32_MAX)
				{
					SetGifMetadataBool(writer.get(), L"/grctlext/TransparencyFlag", true);
					SetGifMetadataByte(writer.get(), L"/grctlext/TransparentColorIndex", static_cast<BYTE>(declaredIndex));
				}
			}

			winrt::check_hresult(frameEncode->WritePixels(static_cast<UINT>(crop.Height), static_cast<UINT>(crop.Width), static_cast<UINT>(cropIndices.size()), cropIndices.data()));
			winrt::check_hresult(frameEncode->Commit());
			previousDisposal = disposal;
			previousRegion = { .X = static_cast<INT>(left), .Y = static_cast<INT>(top), .Width = static_cast<INT>(drawWidth), .Height = static_cast<INT>(drawHeight) };
		}
	}

	static void SetGifLoopMetadata(::IWICBitmapEncoder* encoder)
	{
		winrt::com_ptr<::IWICMetadataQueryWriter> writer;
		if (FAILED(encoder->GetMetadataQueryWriter(writer.put())) || !writer)
		{
			return;
		}

		// NETSCAPE2.0 application extension.
		//
		// These two variants point at arrays with static storage, so they must not go through the
		// PropVariant RAII wrapper: clearing a VT_UI1 | VT_VECTOR frees caub.pElems with CoTaskMemFree,
		// which on a static array is a heap corruption (c0000374). Plain PROPVARIANTs and no clear, which
		// is also leak-free here because nothing was allocated.
		static constexpr BYTE application[] = { 'N', 'E', 'T', 'S', 'C', 'A', 'P', 'E', '2', '.', '0' };
		PROPVARIANT applicationProperty;
		PropVariantInit(&applicationProperty);
		applicationProperty.vt = VT_UI1 | VT_VECTOR;
		applicationProperty.caub.cElems = static_cast<ULONG>(ARRAYSIZE(application));
		applicationProperty.caub.pElems = const_cast<BYTE*>(application);
		winrt::check_hresult(writer->SetMetadataByName(L"/appext/Application", &applicationProperty));

		// NETSCAPE loop extension. The payload IS the extension's data sub-block, so it has to carry the
		// block size and the loop type, not just the flag:
		//
		//   03 01 00 00     size 3, loop type 1, loop count 0 = forever
		//
		// Passing { 1, 0, 0 } here writes a sub-block of size 1, which players treat as malformed and
		// ignore, so the animation plays once and then holds its last frame.
		static constexpr BYTE loopData[] = { 3, 1, 0, 0 };
		PROPVARIANT dataProperty;
		PropVariantInit(&dataProperty);
		dataProperty.vt = VT_UI1 | VT_VECTOR;
		dataProperty.caub.cElems = static_cast<ULONG>(ARRAYSIZE(loopData));
		dataProperty.caub.pElems = const_cast<BYTE*>(loopData);
		winrt::check_hresult(writer->SetMetadataByName(L"/appext/Data", &dataProperty));
	}

	bool AnimatedGif::TryCrop(winrt::IRandomAccessStream const& sourceStream, winrt::IRandomAccessStream const& stream, winrt::Rect const& croppedRect)
	{
		// The decoder reads from the current position, and the single-frame path seeks too.
		sourceStream.Seek(0);
		winrt::com_ptr<::IWICImagingFactory> factory;
		winrt::check_hresult(::CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(::IWICImagingFactory), factory.put_void()));
		const auto source = ToIStream(sourceStream);
		winrt::com_ptr<::IWICBitmapDecoder> decoder;
		winrt::check_hresult(factory->CreateDecoderFromStream(source.get(), nullptr, WICDecodeMetadataCacheOnDemand, decoder.put()));
		// This path is for animations, so a source with a single frame is left to the caller's still path.
		UINT frameCount = 0;
		winrt::check_hresult(decoder->GetFrameCount(&frameCount));
		if (frameCount < 2)
		{
			return false;
		}

		// Read the actual GIF logical screen size: frames are partial rectangles over it, so each one has to
		// be composited onto a canvas of exactly this size to be cropped correctly.
		UINT32 canvasWidth = 0;
		UINT32 canvasHeight = 0;
		if (!GetGifCanvasSize(decoder.get(), canvasWidth, canvasHeight))
		{
			return false;
		}

		// Clamp the requested crop to the GIF canvas.
		const auto left = std::max<double>(0.0, std::min<double>(croppedRect.X, static_cast<double>(canvasWidth)));
		const auto top = std::max<double>(0.0, std::min<double>(croppedRect.Y, static_cast<double>(canvasHeight)));
		const auto right = std::max<double>(left, std::min<double>(croppedRect.X + croppedRect.Width, static_cast<double>(canvasWidth)));
		const auto bottom = std::max<double>(top, std::min<double>(croppedRect.Y + croppedRect.Height, static_cast<double>(canvasHeight)));
		const auto cropWidth = static_cast<INT>(right - left);
		const auto cropHeight = static_cast<INT>(bottom - top);
		if (cropWidth <= 0 || cropHeight <= 0)
		{
			return false;
		}

		WICRect crop{ .X = static_cast<INT>(left), .Y = static_cast<INT>(top), .Width = cropWidth, .Height = cropHeight };

		const auto destination = ToIStream(stream);
		winrt::com_ptr<::IWICBitmapEncoder> encoder;
		winrt::check_hresult(factory->CreateEncoder(GUID_ContainerFormatGif, nullptr, encoder.put()));

		try
		{
			winrt::check_hresult(encoder->Initialize(destination.get(), WICBitmapEncoderNoCache));

			// Preserve GIF looping.
			//
			// This creates a NETSCAPE2.0 infinite-loop extension.
			SetGifLoopMetadata(encoder.get());
			WriteAnimatedGifFrames(factory.get(), decoder.get(), encoder.get(), frameCount, crop, canvasWidth, canvasHeight);
			winrt::check_hresult(encoder->Commit());
		}
		catch (...)
		{
			// A half written animation is worse than none: hand the stream back the way it was found.
			stream.Size(0);
			stream.Seek(0);
			throw;
		}

		// A commit that produced nothing leaves the stream as it was found, so the single frame path can
		// still write a still of the selection into it.
		return true;
	}
}
