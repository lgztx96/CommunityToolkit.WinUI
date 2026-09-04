#pragma once
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <memory>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_helpers.h>
#include <wil/resource.h>
#endif

namespace winrt::XamlToolkit::WinUI::Future
{
	struct FrameworkElementExtensions
	{
		static winrt::Windows::Foundation::IAsyncOperation<bool> WaitUntilLoadedAsync(
			winrt::Microsoft::UI::Xaml::FrameworkElement element)
		{
			if (element.IsLoaded() && element.Parent())
			{
				co_return true;
			}

			winrt::Microsoft::UI::Xaml::FrameworkElement::Loaded_revoker loadedRevoker;

			wil::shared_event completionEvent(wil::EventOptions::ManualReset);
			loadedRevoker = element.Loaded(winrt::auto_revoke, [completionEvent](auto&&...)
			{
				completionEvent.SetEvent();
			});

			co_await winrt::resume_on_signal(completionEvent.get());

			co_await wil::resume_foreground(element.DispatcherQueue());

			co_return true;
		}
	};
}
