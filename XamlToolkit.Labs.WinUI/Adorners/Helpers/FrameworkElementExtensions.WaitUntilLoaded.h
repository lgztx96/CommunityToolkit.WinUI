#pragma once
#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#endif
#include <synchapi.h>

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

			auto tcs = std::make_shared<winrt::handle>(CreateEventW(nullptr, true, false, nullptr));
			loadedRevoker = element.Loaded(winrt::auto_revoke, [=](auto&&...)
				{
					SetEvent(tcs->get());
				});

			co_await winrt::resume_on_signal(tcs->get());

			co_await context;

			co_return true;
		}
	};
}
