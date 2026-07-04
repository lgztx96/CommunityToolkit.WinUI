#pragma once

#include "CameraPreview.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/XamlToolkit.WinUI.Helpers.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Foundation::Collections;
	using namespace Windows::Media::Capture::Frames;
	using namespace Windows::Media::Core;
	using namespace Windows::Media::Playback;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace XamlToolkit::WinUI::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct CameraPreview : CameraPreviewT<CameraPreview>
	{
	private:
		static constexpr std::wstring_view Preview_MediaPlayerElementControl = L"MediaPlayerElementControl";
		static constexpr std::wstring_view Preview_FrameSourceGroupButton = L"FrameSourceGroupButton";

		winrt::MediaPlayer _mediaPlayer{ nullptr };
		winrt::MediaPlayerElement _mediaPlayerElementControl{ nullptr };
		winrt::Button _frameSourceGroupButton{ nullptr };

		winrt::IVectorView<winrt::MediaFrameSourceGroup> _frameSourceGroups{ nullptr };

		winrt::ButtonBase::Click_revoker _frameSourceGroupButtonClickRevoker;

		bool IsFrameSourceGroupButtonAvailable() const { return _frameSourceGroups && _frameSourceGroups.Size() > 1; }

		static void IsFrameSourceGroupButtonVisibleChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e);

	public:
		static inline const wil::single_threaded_property<winrt::DependencyProperty> IsFrameSourceGroupButtonVisibleProperty =
			winrt::DependencyProperty::Register(
				L"IsFrameSourceGroupButtonVisible",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata{ winrt::box_value(true), &CameraPreview::IsFrameSourceGroupButtonVisibleChanged });

		bool IsFrameSourceGroupButtonVisible() const
		{
			return winrt::unbox_value<bool>(GetValue(IsFrameSourceGroupButtonVisibleProperty));
		}

		void IsFrameSourceGroupButtonVisible(bool value)
		{
			SetValue(IsFrameSourceGroupButtonVisibleProperty, winrt::box_value(value));
		}

		wil::single_threaded_rw_property<winrt::CameraHelper> CameraHelper{ nullptr };

		wil::untyped_event<PreviewFailedEventArgs> PreviewFailed;

		winrt::IAsyncAction StartAsync();

		winrt::IAsyncAction StartAsync(winrt::CameraHelper const& cameraHelper);

		void Stop();

		CameraPreview();

		winrt::fire_and_forget OnApplyTemplate();

	private:
		winrt::IAsyncAction InitializeAsync();

		winrt::IAsyncAction FrameSourceGroupButton_ClickAsync(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

		void InvokePreviewFailed(winrt::hstring const& error);

		void SetMediaPlayerSource();

		void SetUIControls(CameraHelperResult result);

		void SetFrameSourceGroupButtonVisibility();
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct CameraPreview : CameraPreviewT<CameraPreview, implementation::CameraPreview>
	{
	};
}
