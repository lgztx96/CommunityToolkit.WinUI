#pragma once

#include "TokenItem.g.h"

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Controls::Primitives;

	struct TokenItem : TokenItemT<TokenItem>
	{
		static constexpr std::wstring_view IconLeftState = L"IconLeft";
		static constexpr std::wstring_view IconOnlyState = L"IconOnly";
		static constexpr std::wstring_view ContentOnlyState = L"ContentOnly";
		static constexpr std::wstring_view RemoveButtonVisibleState = L"RemoveButtonVisible";
		static constexpr std::wstring_view RemoveButtonNotVisibleState = L"RemoveButtonNotVisible";
		static constexpr std::wstring_view TokenItemRemoveButtonName = L"PART_RemoveButton";
		ButtonBase _tokenItemRemoveButton{ nullptr };

		ButtonBase::Click_revoker _removeButtonClickRevoker;

	public:
		TokenItem();

		void OnApplyTemplate();

		void OnContentChanged(IInspectable const& oldContent, IInspectable const& newContent);

		virtual void OnIconPropertyChanged(IconElement const& oldValue, IconElement const& newValue);

		virtual void OnIsRemoveablePropertyChanged(bool oldValue, bool newValue);

		wil::untyped_event<TokenItemRemovingEventArgs> Removing;

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IsRemoveableProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"IsRemoveable",
				winrt::xaml_typename<bool>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{
					winrt::box_value(false),
					[](auto& d, auto& e)
					{
						if (auto tokenItem = d.template try_as<class_type>())
						{
							auto self = winrt::get_self<TokenItem>(tokenItem)->get_strong();
							auto oldValue = winrt::unbox_value<bool>(e.OldValue());
							auto newValue = winrt::unbox_value<bool>(e.NewValue());
							self->OnIsRemoveablePropertyChanged(oldValue, newValue);
						}
					}
				}
			);

		static inline const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> IconProperty =
			winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
				L"Icon",
				winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Controls::IconElement>(),
				winrt::xaml_typename<class_type>(),
				winrt::Microsoft::UI::Xaml::PropertyMetadata{
					nullptr,
					[](auto& d, auto& e)
					{
						if (auto tokenItem = d.template try_as<class_type>())
						{
							auto self = winrt::get_self<TokenItem>(tokenItem)->get_strong();
							auto oldValue = e.OldValue().try_as<IconElement>();
							auto newValue = e.NewValue().try_as<IconElement>();
							self->OnIconPropertyChanged(oldValue, newValue);
						}
					}
				}
			);

		bool IsRemoveable() const
		{
			return winrt::unbox_value<bool>(GetValue(IsRemoveableProperty));
		}

		void IsRemoveable(bool value)
		{
			SetValue(IsRemoveableProperty, winrt::box_value(value));
		}

		winrt::Microsoft::UI::Xaml::Controls::IconElement Icon() const
		{
			return GetValue(IconProperty).try_as<winrt::Microsoft::UI::Xaml::Controls::IconElement>();
		}

		void Icon(winrt::Microsoft::UI::Xaml::Controls::IconElement const& value)
		{
			SetValue(IconProperty, value);
		}

	private:
		void TokenItemRemoveButton_Click(IInspectable const& sender, RoutedEventArgs const& e);

		void ContentChanged();

		void IconChanged();

		void IsRemoveableChanged();
	};
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
	struct TokenItem : TokenItemT<TokenItem, implementation::TokenItem>
	{
	};
}
