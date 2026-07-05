#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <optional>
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	template <typename TPropertyType>
	struct PropertyChangeEventSource
	{
	private:
		winrt::DependencyObject _source;
		winrt::DependencyProperty _property;
		std::optional<int64_t> _registrationToken;

	public:
		PropertyChangeEventSource(const PropertyChangeEventSource&) = delete;
		PropertyChangeEventSource& operator=(const PropertyChangeEventSource&) = delete;
		PropertyChangeEventSource(PropertyChangeEventSource&&) = delete;
		PropertyChangeEventSource& operator=(PropertyChangeEventSource&&) = delete;

		PropertyChangeEventSource(winrt::DependencyObject const& source, winrt::DependencyProperty const& property)
			: _source(source), _property(property)
		{
			_registrationToken = source.RegisterPropertyChangedCallback(property,
				[this](winrt::DependencyObject const& sender, winrt::DependencyProperty const& dp)
				{
					auto value = winrt::unbox_value<TPropertyType>(sender.GetValue(dp));
					ValueChanged.invoke(nullptr, value);
				});
		}

		wil::untyped_event<TPropertyType> ValueChanged;

		TPropertyType Value() const { return winrt::unbox_value<TPropertyType>(_source.GetValue(_property)); }

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
