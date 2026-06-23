#include "pch.h"
#include "winrt_module_imports.h"
#include "FrameEventArgs.h"
#if __has_include("FrameEventArgs.g.cpp")
#include "FrameEventArgs.g.cpp"
#endif

#ifdef __INTELLISENSE__
#include <winrt/Windows.Graphics.Imaging.h>
#endif

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    winrt::Windows::Media::VideoFrame FrameEventArgs::VideoFrame()
    {
        _semaphore.acquire();

        // The VideoFrame could be disposed at any time so we need to create a copy we can use.
        if (_videoFrameCopy == nullptr && _videoFrame && _videoFrame.SoftwareBitmap())
        {
            try
            {
                auto copied = winrt::Windows::Graphics::Imaging::SoftwareBitmap::Copy(_videoFrame.SoftwareBitmap());
                _videoFrameCopy = winrt::Windows::Media::VideoFrame::CreateWithSoftwareBitmap(copied);
            }
            catch (...)
            {

            }
        }

        _semaphore.release();

        if (_videoFrame)
        {
            if (auto bitmap = _videoFrame.SoftwareBitmap())
            {
                bitmap.Close();
            }

            _videoFrame.Close();
        }

        return _videoFrameCopy ? _videoFrameCopy : _videoFrame;
    }

    void FrameEventArgs::VideoFrame(winrt::Windows::Media::VideoFrame const& value)
    {
        _videoFrame = value;
    }
}
