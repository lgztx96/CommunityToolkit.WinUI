#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#endif

namespace winrt::XamlToolkit::WinUI::Controls
{
	template <typename TPropertyType>
	struct PropertyChangeEventSource
	{
	private:
		winrt::Microsoft::UI::Xaml::DependencyObject _source;
		winrt::Microsoft::UI::Xaml::DependencyProperty _property;
		std::optional<int64_t> _registrationToken;

	public:
		PropertyChangeEventSource(const PropertyChangeEventSource&) = delete;
		PropertyChangeEventSource& operator=(const PropertyChangeEventSource&) = delete;
		PropertyChangeEventSource(PropertyChangeEventSource&&) = delete;
		PropertyChangeEventSource& operator=(PropertyChangeEventSource&&) = delete;

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
					ValueChanged.invoke(nullptr, value);
				});
		}

		wil::untyped_event<TPropertyType> ValueChanged;

		TPropertyType Value() { return winrt::unbox_value<TPropertyType>(_source.GetValue(_property)); }

		void Value(TPropertyType const& value) { _source.SetValue(winrt::box_value(_property)); }

		void Unregister()
		{
			if (_registrationToken)
			{
				_source.UnregisterPropertyChangedCallback(_property, *_registrationToken);
				_registrationToken.reset();
			}
		}

		~PropertyChangeEventSource()
		{
			Unregister();
		}
	};
}
