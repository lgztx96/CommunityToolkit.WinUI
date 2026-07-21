#pragma once

#include "CameraHelper.g.h"
#include "../../XamlToolkit.WinUI/common.h"
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Media.MediaProperties.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <wil/resource.h>
#endif

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Media::Capture;
    using namespace Windows::Media::Capture::Frames;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    struct CameraHelper : CameraHelperT<CameraHelper>
    {
        CameraHelper() = default;

        winrt::MediaFrameSourceGroup FrameSourceGroup() const;

        void FrameSourceGroup(winrt::MediaFrameSourceGroup const& value);

        winrt::IVectorView<winrt::MediaFrameFormat> FrameFormatsAvailable() const;

        winrt::MediaFrameSource PreviewFrameSource() const;

        wil::untyped_event<FrameEventArgs> FrameArrived;

        winrt::IAsyncOperation<CameraHelperResult> InitializeAndStartCaptureAsync();

        static winrt::IAsyncOperation<winrt::IVectorView<winrt::MediaFrameSourceGroup>> GetFrameSourceGroupsAsync();

        winrt::IAsyncAction CleanUpAsync();

        winrt::fire_and_forget Close();

    private:
        winrt::async_mutex _mutex;

        static inline winrt::IVectorView<winrt::MediaFrameSourceGroup> _frameSourceGroups{ nullptr };

        winrt::MediaCapture _mediaCapture{ nullptr };
        winrt::MediaFrameReader _frameReader{ nullptr };
        winrt::MediaFrameSourceGroup _group{ nullptr };
        winrt::MediaFrameSource _previewFrameSource{ nullptr };
        winrt::IVectorView<winrt::MediaFrameFormat> _frameFormatsAvailable{ nullptr };

        bool _groupChanged = false;
        bool _initialized = false;
        bool _disposed = false;

        winrt::MediaFrameReader::FrameArrived_revoker _frameArrivedRevoker;

        winrt::IAsyncOperation<CameraHelperResult> InitializeMediaCaptureAsync();

        winrt::IAsyncAction StopReaderAsync();

        void Reader_FrameArrived(winrt::MediaFrameReader const& sender, winrt::MediaFrameArrivedEventArgs const& args);
    };
}

namespace winrt::XamlToolkit::WinUI::Helpers::factory_implementation
{
    struct CameraHelper : CameraHelperT<CameraHelper, implementation::CameraHelper>
    {
    };
}
