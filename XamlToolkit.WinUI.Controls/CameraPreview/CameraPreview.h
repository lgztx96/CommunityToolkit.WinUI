#pragma once

#include "CameraPreview.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <winrt/Windows.Media.Playback.h>
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
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Controls::Primitives;
	using namespace Windows::Media::Capture::Frames;
	using namespace Windows::Media::Playback;
	using namespace XamlToolkit::WinUI::Helpers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	struct CameraPreview : CameraPreviewT<CameraPreview>
	{
	private:
		static constexpr std::wstring_view Preview_MediaPlayerElementControl = L"MediaPlayerElementControl";
		static constexpr std::wstring_view Preview_FrameSourceGroupButton = L"FrameSourceGroupButton";

		CameraHelper _cameraHelper{ nullptr };
		MediaPlayer _mediaPlayer{ nullptr };
		MediaPlayerElement _mediaPlayerElementControl{ nullptr };
		Button _frameSourceGroupButton{ nullptr };

		IVectorView<MediaFrameSourceGroup> _frameSourceGroups{ nullptr };

		ButtonBase::Click_revoker _frameSourceGroupButtonClickRevoker;

		bool IsFrameSourceGroupButtonAvailable() const { return _frameSourceGroups && _frameSourceGroups.Size() > 1; }

		static void IsFrameSourceGroupButtonVisibleChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

	public:
		static inline const wil::single_threaded_property<DependencyProperty> IsFrameSourceGroupButtonVisibleProperty =
			DependencyProperty::Register(
				L"IsFrameSourceGroupButtonVisible",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				PropertyMetadata{ winrt::box_value(true), &CameraPreview::IsFrameSourceGroupButtonVisibleChanged });

		bool IsFrameSourceGroupButtonVisible() const
		{
			return winrt::unbox_value<bool>(GetValue(IsFrameSourceGroupButtonVisibleProperty));
		}

		void IsFrameSourceGroupButtonVisible(bool value)
		{
			SetValue(IsFrameSourceGroupButtonVisibleProperty, winrt::box_value(value));
		}

		wil::single_threaded_rw_property<winrt::XamlToolkit::WinUI::Helpers::CameraHelper> CameraHelper{ nullptr };

		wil::untyped_event<PreviewFailedEventArgs> PreviewFailed;

		IAsyncAction StartAsync();

		IAsyncAction StartAsync(winrt::XamlToolkit::WinUI::Helpers::CameraHelper cameraHelper);

		void Stop();

		CameraPreview();

		winrt::fire_and_forget OnApplyTemplate();

	private:
		IAsyncAction InitializeAsync();

		IAsyncAction FrameSourceGroupButton_ClickAsync(IInspectable const& sender, RoutedEventArgs const& e);

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
