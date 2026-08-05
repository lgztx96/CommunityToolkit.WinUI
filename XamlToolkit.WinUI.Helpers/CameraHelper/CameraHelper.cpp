#include "pch.h"
#include "winrt_module_imports.h"
#include "CameraHelper.h"
#if __has_include("CameraHelper.g.cpp")
#include "CameraHelper.g.cpp"
#endif
#include "FrameEventArgs.h"

namespace winrt
{
    using namespace Windows::Foundation::Numerics;
    using namespace Windows::Devices::Enumeration;
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

            std::vector<winrt::MediaFrameSourceGroup> result;
            std::unordered_set<winrt::hstring> deviceIds;
   
            for (const auto& vd : videoDevices)
            {
                deviceIds.insert(vd.Id());
            }

            for (const auto& group : groups)
            {
				const auto& sources = group.SourceInfos();
                bool match = std::any_of(sources.begin(), sources.end(),
                    [&](const auto& source)
                    {
                        if (source.SourceKind() != winrt::MediaFrameSourceKind::Color)
                            return false;

                        auto type = source.MediaStreamType();
                        if (type != winrt::MediaStreamType::VideoPreview &&
                            type != winrt::MediaStreamType::VideoRecord)
                            return false;

                        return deviceIds.contains(source.DeviceInformation().Id());
                    });

                if (match) 
                {
                    result.emplace_back(group);
                }
            }

            _frameSourceGroups = winrt::single_threaded_vector(std::move(result)).GetView();
        }

        co_return _frameSourceGroups;
    }

    winrt::MediaFrameSourceGroup CameraHelper::FrameSourceGroup() const { return _group; }

    void CameraHelper::FrameSourceGroup(winrt::MediaFrameSourceGroup const& value)
    {
        _groupChanged = _group != value;
        _group = value;
    }

    winrt::IVectorView<winrt::MediaFrameFormat> CameraHelper::FrameFormatsAvailable() const
    {
        return _frameFormatsAvailable;
    }

    winrt::MediaFrameSource CameraHelper::PreviewFrameSource() const { return _previewFrameSource; }

    winrt::IAsyncOperation<CameraHelperResult> CameraHelper::InitializeAndStartCaptureAsync()
    {
        auto strongThis = get_strong();

        CameraHelperResult result = CameraHelperResult::Success;

        auto guard = co_await _mutex.lock_async();

        try
        {
            // if FrameSourceGroup hasn't changed from last initialization, just return back.
            if (_initialized && _group && !_groupChanged)
            {
                co_return CameraHelperResult::Success;
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
                for (const auto& group : _frameSourceGroups)
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
                co_return CameraHelperResult::NoFrameSourceGroupAvailable;
            }

            result = co_await InitializeMediaCaptureAsync();

            if (_previewFrameSource && _mediaCapture)
            {
                _frameReader = co_await _mediaCapture.CreateFrameReaderAsync(_previewFrameSource);
                if (_frameReader == nullptr)
                {
                    result = CameraHelperResult::CreateFrameReaderFailed;
                }
                else
                {
                    _frameReader.AcquisitionMode(winrt::MediaFrameReaderAcquisitionMode::Realtime);
                    _frameArrivedRevoker = _frameReader.FrameArrived(winrt::auto_revoke, { this, &CameraHelper::Reader_FrameArrived });
                    auto status = co_await _frameReader.StartAsync();
                    if (status != winrt::MediaFrameReaderStartStatus::Success)
                    {
                        result = CameraHelperResult::StartFrameReaderFailed;
                    }
                }
            }

            _initialized = (result == CameraHelperResult::Success);
        }
        catch (...)
        {
            result = CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != CameraHelperResult::Success)
        {
			guard.unlock();
            co_await CleanUpAsync();
        }

        co_return result;
    }

    winrt::IAsyncOperation<CameraHelperResult> CameraHelper::InitializeMediaCaptureAsync()
    {
        if (_mediaCapture == nullptr)
        {
            _mediaCapture = winrt::MediaCapture();
        }

        winrt::MediaCaptureInitializationSettings settings;
        settings.SourceGroup(_group);
        settings.MemoryPreference(winrt::MediaCaptureMemoryPreference::Cpu);
        settings.StreamingCaptureMode(winrt::StreamingCaptureMode::Video);

        CameraHelperResult result = CameraHelperResult::Success;

        try
        {
            co_await _mediaCapture.InitializeAsync(settings);

            for (const auto& kvp : _mediaCapture.FrameSources())
            {
                const auto& value = kvp.Value();
                const auto& info = value.Info();
                if (info.MediaStreamType() == winrt::MediaStreamType::VideoPreview &&
                    info.SourceKind() == winrt::MediaFrameSourceKind::Color)
                {
                    _previewFrameSource = value;
                    break;
                }
            }

            if (_previewFrameSource == nullptr)
            {
                for (const auto& kvp : _mediaCapture.FrameSources())
                {
                    const auto& value = kvp.Value();
                    const auto& info = value.Info();
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
                co_return CameraHelperResult::NoFrameSourceAvailable;
            }

            std::vector<winrt::MediaFrameFormat> formats;

            auto compare_ignore_case = [](std::wstring_view lhs, std::wstring_view rhs)
            {
                return std::ranges::equal(lhs, rhs, [](wchar_t c1, wchar_t c2) { return std::tolower(c1) == std::tolower(c2); });
            };

            for (const auto& fmt : _previewFrameSource.SupportedFormats())
            {
				auto frameRate = fmt.FrameRate();
                double fps = static_cast<double>(frameRate.Numerator()) / frameRate.Denominator();
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
                co_return CameraHelperResult::NoCompatibleFrameFormatAvailable;
            }

            std::sort(formats.begin(), formats.end(), [](const auto& a, const auto& b)
            { 
                const auto& fa = a.VideoFormat();
                const auto& fb = b.VideoFormat();
                return fa.Width() * fa.Height() < fb.Width() * fb.Height();
            });

            // Set the format with the highest resolution available by default
            co_await _previewFrameSource.SetFormatAsync(formats.back());
            _frameFormatsAvailable = winrt::single_threaded_vector(std::move(formats)).GetView();
        }
        catch (winrt::hresult_error const& e)
        {
            result = e.code() == E_ACCESSDENIED ? CameraHelperResult::CameraAccessDenied : CameraHelperResult::InitializationFailed_UnknownError;
        }

        if (result != CameraHelperResult::Success)
        {
            co_await StopReaderAsync();

            if (_mediaCapture)
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
                FrameArrived.invoke(sender, *args);
            }
        }
    }

    winrt::IAsyncAction CameraHelper::CleanUpAsync()
    {
        auto strongThis = get_strong();

        auto guard = co_await _mutex.lock_async();

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
    }

    winrt::fire_and_forget CameraHelper::Close()
    {
        if (!_disposed)
        {
            _disposed = true;
            co_await CleanUpAsync();
        }
    }
}
