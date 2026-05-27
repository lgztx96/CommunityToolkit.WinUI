#pragma once

#include "FrameEventArgs.g.h"

#ifdef __INTELLISENSE__
#include <semaphore>
#include <winrt/Windows.Media.h>
#endif

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct FrameEventArgs : FrameEventArgsT<FrameEventArgs>
    {
    private:
        std::binary_semaphore _semaphore{ 1 };
        winrt::Windows::Media::VideoFrame _videoFrame{ nullptr };
        winrt::Windows::Media::VideoFrame _videoFrameCopy{ nullptr };

    public:
        winrt::Windows::Media::VideoFrame VideoFrame();

        void VideoFrame(winrt::Windows::Media::VideoFrame const& value);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct FrameEventArgs : FrameEventArgsT<FrameEventArgs, implementation::FrameEventArgs>
    {
    };
}
