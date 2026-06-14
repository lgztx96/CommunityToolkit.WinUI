#pragma once
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <memory>
#include <wil/resource.h>
#endif

namespace winrt::XamlToolkit::WinUI::Future
{
	struct FrameworkElementExtensions
	{
		static winrt::Windows::Foundation::IAsyncOperation<bool>
			WaitUntilLoadedAsync(winrt::Microsoft::UI::Xaml::FrameworkElement const& element)
		{
			if (element.IsLoaded() && element.Parent() != nullptr)
			{
				co_return true;
			}

			winrt::apartment_context context;

			winrt::Microsoft::UI::Xaml::FrameworkElement::Loaded_revoker loadedRevoker;

			wil::shared_event completionEvent(wil::EventOptions::ManualReset);
			loadedRevoker = element.Loaded(winrt::auto_revoke, [completionEvent](auto&&...)
			{
				completionEvent.SetEvent();
			});

			co_await winrt::resume_on_signal(completionEvent.get());

			co_await context;

			co_return true;
		}
	};
}
