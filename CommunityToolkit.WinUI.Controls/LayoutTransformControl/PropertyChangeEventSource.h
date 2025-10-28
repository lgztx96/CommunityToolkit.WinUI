#pragma once
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Windows.Foundation.h>

namespace winrt::CommunityToolkit::WinUI::Controls
{
	template <typename TPropertyType>
	struct PropertyChangeEventSource
	{
	private:
		winrt::Microsoft::UI::Xaml::DependencyObject _source{ nullptr };
		winrt::Microsoft::UI::Xaml::DependencyProperty _property{ nullptr };
		int64_t _registrationToken{ 0 };

	public:
		winrt::event<winrt::Windows::Foundation::TypedEventHandler<
			winrt::Windows::Foundation::IInspectable,
			TPropertyType>> ValueChanged;

		PropertyChangeEventSource(
			winrt::Microsoft::UI::Xaml::DependencyObject const& source,
			winrt::Microsoft::UI::Xaml::DependencyProperty const& property)
			: _source(source), _property(property)
		{
			_registrationToken = source.RegisterPropertyChangedCallback(property,
				[this](winrt::Microsoft::UI::Xaml::DependencyObject const& sender,
					winrt::Microsoft::UI::Xaml::DependencyProperty const& dp)
				{
					auto value = winrt::unbox_value<TPropertyType>(sender.GetValue(dp));
					ValueChanged(nullptr, value);
				});
		}

		void Unregister()
		{
			if (_registrationToken)
			{
				_source.UnregisterPropertyChangedCallback(_property, _registrationToken);
				_registrationToken = 0;
			}
		}

		~PropertyChangeEventSource()
		{
			Unregister();
		}
	};
}