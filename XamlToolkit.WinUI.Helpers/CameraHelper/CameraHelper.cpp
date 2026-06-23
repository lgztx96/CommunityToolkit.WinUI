#include "pch.h"
#include "winrt_module_imports.h"
#include "CameraHelper.h"
#if __has_include("CameraHelper.g.cpp")
#include "CameraHelper.g.cpp"
#endif
#include "FrameEventArgs.h"

namespace winrt
{
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
    using namespace Windows::Foundation::Numerics;
    using namespace Windows::Devices::Enumeration;
    using namespace Windows::Media::Capture;
    using namespace Windows::Media::Capture::Frames;
    using namespace Windows::Media::MediaProperties;
}

namespace winrt::XamlToolkit::WinUI::Helpers::implementation
{
    winrt::IAsyncOperation<winrt::IVectorView<winrt::MediaFrameSourceGroup>> CameraHelper::GetFrameSourceGroupsAsync()
    {
        if (!_frameSourceGroups)
        {
            auto videoDevices = co_await winrt::DeviceInformation::FindAllAsync(winrt::DeviceClass::VideoCapture);
            auto groups = co_await winrt::MediaFrameSourceGroup::FindAllAsync();

            std::vector<winrt::MediaFrameSourceGroup> filtered;

            for (auto const& g : groups)
            {
                bool match = false;
                for (auto const& s : g.SourceInfos())
                {
                    if (s.SourceKind() == winrt::MediaFrameSourceKind::Color &&
                        (s.MediaStreamType() == winrt::MediaStreamType::VideoPreview ||
                            s.MediaStreamType() == winrt::MediaStreamType::VideoRecord))
                    {
                        for (auto const& vd : videoDevices)
                        {
                            if (vd.Id() == s.DeviceInformation().Id())
                            {
                                match = true;
                                break;
                            }
                        }
                    }
                }
                if (match)
                    filtered.push_back(g);
            }

            _frameSourceGroups = winrt::single_threaded_vector(std::move(filtered)).GetView();
        }

        co_return _frameSourceGroups;
    }

    winrt::MediaFrameSourceGroup CameraHelper::FrameSourceGroup() { return _group; }

    void CameraHelper::FrameSourceGroup(winrt::MediaFrameSourceGroup const& value)
    {
        _groupChanged = _group != value;
        _group = value;
    }

    winrt::Collections::IVectorView<winrt::MediaFrameFormat> CameraHelper::FrameFormatsAvailable()
    {
        return _frameFormatsAvailable;
    }

    winrt::MediaFrameSource CameraHelper::PreviewFrameSource() { return _previewFrameSource; }

    winrt::IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> CameraHelper::InitializeAndStartCaptureAsync()
    {
        _semaphoreSlim.acquire();

        auto cleanup = wil::scope_exit([this]
        {
            _semaphoreSlim.release();
        });

        winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult result = winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success;

        try
        {
            // if FrameSourceGroup hasn't changed from last initialization, just return back.
            if (_initialized && _group && !_groupChanged)
            {
                co_return winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success;
            }

            _groupChanged = false;

            co_await StopReaderAsync();

            if (_mediaCapture)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }

            if (_frameSourceGroups == nullptr)
            {
                _frameSourceGroups = co_await GetFrameSourceGroupsAsync();
            }

            if (_group == nullptr)
            {
                _group = _frameSourceGroups.Size() > 0 ? _frameSourceGroups.GetAt(0) : nullptr;
            }
            else
            {
                // Verify selected group is part of existing FrameSourceGroups
                for (auto const& group : _frameSourceGroups)
                {
                    if (group.Id() == _group.Id())
                    {
                        _group = group;
                        break;
                    }
                }
            }

            // If there is no camera source available, we can't proceed
            if (_group == nullptr)
            {
                co_return winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::NoFrameSourceGroupAvailable;
            }

            result = co_await InitializeMediaCaptureAsync();

            if (_previewFrameSource && _mediaCapture)
            {
                _frameReader = co_await _mediaCapture.CreateFrameReaderAsync(_previewFrameSource);
                if (_frameReader == nullptr)
                {
                    result = winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::CreateFrameReaderFailed;
                }
                else
                {
                    _frameReader.AcquisitionMode(winrt::MediaFrameReaderAcquisitionMode::Realtime);
                    _frameArrivedRevoker = _frameReader.FrameArrived(winrt::auto_revoke, { this, &CameraHelper::Reader_FrameArrived });
                    auto status = co_await _frameReader.StartAsync();
                    if (status != winrt::MediaFrameReaderStartStatus::Success)
                    {
                        result = winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::StartFrameReaderFailed;
                    }
                }
            }

            _initialized = (result == winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success);
        }
        catch (...)
        {
            result = winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success)
        {
            cleanup.reset();
            co_await CleanUpAsync();
        }

        co_return result;
    }

    winrt::IAsyncOperation<winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult> CameraHelper::InitializeMediaCaptureAsync()
    {
        if (_mediaCapture == nullptr)
        {
            _mediaCapture = winrt::MediaCapture();
        }

        winrt::MediaCaptureInitializationSettings settings;
        settings.SourceGroup(_group);
        settings.MemoryPreference(winrt::MediaCaptureMemoryPreference::Cpu);
        settings.StreamingCaptureMode(winrt::StreamingCaptureMode::Video);

        winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult result = winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success;

        try
        {
            co_await _mediaCapture.InitializeAsync(settings);

            for (auto const& kvp : _mediaCapture.FrameSources())
            {
                auto const& value = kvp.Value();
                auto const& info = value.Info();
                if (info.MediaStreamType() == winrt::MediaStreamType::VideoPreview &&
                    info.SourceKind() == winrt::MediaFrameSourceKind::Color)
                {
                    _previewFrameSource = value;
                    break;
                }
            }

            if (_previewFrameSource == nullptr)
            {
                for (auto const& kvp : _mediaCapture.FrameSources())
                {
                    auto const& value = kvp.Value();
                    auto const& info = value.Info();
                    if (info.MediaStreamType() == winrt::MediaStreamType::VideoRecord &&
                        info.SourceKind() == winrt::MediaFrameSourceKind::Color)
                    {
                        _previewFrameSource = value;
                        break;
                    }
                }
            }

            if (_previewFrameSource == nullptr)
            {
                co_return winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::NoFrameSourceAvailable;
            }

            std::vector<winrt::MediaFrameFormat> formats;

            auto compare_ignore_case = [](std::wstring_view lhs, std::wstring_view rhs)
            {
                return std::ranges::equal(lhs, rhs, [](wchar_t c1, wchar_t c2) { return std::tolower(c1) == std::tolower(c2); });
            };

            for (auto const& fmt : _previewFrameSource.SupportedFormats())
            {
                double fps = static_cast<double>(fmt.FrameRate().Numerator()) / fmt.FrameRate().Denominator();
                auto subtype = fmt.Subtype();
                if (fps >= 15 &&
                    (compare_ignore_case(subtype, winrt::MediaEncodingSubtypes::Nv12()) ||
                     compare_ignore_case(subtype, winrt::MediaEncodingSubtypes::Bgra8()) ||
                     compare_ignore_case(subtype, winrt::MediaEncodingSubtypes::Yuy2()) ||
                     compare_ignore_case(subtype, winrt::MediaEncodingSubtypes::Rgb32())))
                {
                    formats.emplace_back(fmt);
                }
            }

            if (formats.empty())
            {
                co_return winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::NoCompatibleFrameFormatAvailable;
            }

            std::sort(formats.begin(), formats.end(), [](auto const& a, auto const& b)
                { return a.VideoFormat().Width() * a.VideoFormat().Height() <
                b.VideoFormat().Width() * b.VideoFormat().Height(); });

            // Set the format with the highest resolution available by default
            _frameFormatsAvailable = winrt::single_threaded_vector(std::move(formats)).GetView();
            co_await _previewFrameSource.SetFormatAsync(_frameFormatsAvailable.GetAt(_frameFormatsAvailable.Size() - 1));
        }
        catch (winrt::hresult_error const& e)
        {
            result = e.code() == E_ACCESSDENIED ? winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::CameraAccessDenied : winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != winrt::XamlToolkit::WinUI::Helpers::CameraHelperResult::Success)
        {
            co_await StopReaderAsync();

            if (_mediaCapture != nullptr)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }
        }

        co_return result;
    }

    winrt::IAsyncAction CameraHelper::StopReaderAsync()
    {
        if (_frameReader)
        {
            _frameArrivedRevoker.revoke();
            co_await _frameReader.StopAsync();
            _frameReader.Close();
            _frameReader = nullptr;
        }
    }

    void CameraHelper::Reader_FrameArrived(winrt::MediaFrameReader const& sender, winrt::MediaFrameArrivedEventArgs const&)
    {
        // TryAcquireLatestFrame will return the latest frame that has not yet been acquired.
        // This can return null if there is no such frame, or if the reader is not in the
        // "Started" state. The latter can occur if a FrameArrived event was in flight
        // when the reader was stopped.
        if (auto frame = sender.TryAcquireLatestFrame())
        {
            if (auto vmf = frame.VideoMediaFrame())
            {
                auto args = winrt::make_self<FrameEventArgs>();
                args->VideoFrame(vmf.GetVideoFrame());
                FrameArrived.invoke(*this, *args);
            }
        }
    }

    winrt::IAsyncAction CameraHelper::CleanUpAsync()
    {
        _semaphoreSlim.acquire();

        try
        {
            _initialized = false;
            co_await StopReaderAsync();

            if (_mediaCapture)
            {
                _mediaCapture.Close();
                _mediaCapture = nullptr;
            }
        }
        catch (...) {}

        _semaphoreSlim.release();
    }

    void CameraHelper::Close()
    {
        if (!_disposed)
        {
            CleanUpAsync().get();
            _disposed = true;
        }
    }
}
