#pragma once

#include "ResizeElementAdorner.g.h"
#include "../Adorner.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <array>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
	struct ResizeElementAdorner : ResizeElementAdornerT<ResizeElementAdorner, implementation::Adorner>
	{
		static constexpr std::wstring_view TopThumbPartName = L"TopThumbPart";
		static constexpr std::wstring_view BottomThumbPartName = L"BottomThumbPart";
		static constexpr std::wstring_view LeftThumbPartName = L"LeftThumbPart";
		static constexpr std::wstring_view RightThumbPartName = L"RightThumbPart";
		static constexpr std::wstring_view TopLeftThumbPartName = L"TopLeftThumbPart";
		static constexpr std::wstring_view TopRightThumbPartName = L"TopRightThumbPart";
		static constexpr std::wstring_view BottomLeftThumbPartName = L"BottomLeftThumbPart";
		static constexpr std::wstring_view BottomRightThumbPartName = L"BottomRightThumbPart";

		ResizeElementAdorner();

		FrameworkElement AdornedElement() const
		{
			return base_type::AdornedElement().try_as<FrameworkElement>();
		}

		void OnApplyTemplate();

		void OnAttached() override;

		void OnDetaching() override;

	private:
		void OnTargetManipulated(IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

		void OnTargetControlResized(winrt::XamlToolkit::Labs::WinUI::ResizeThumb const& sender, ITargetControlResizedEventArgs const& args);

		winrt::XamlToolkit::Labs::WinUI::ResizeThumb TopThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb BottomThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb LeftThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb RightThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb TopLeftThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb TopRightThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb BottomLeftThumbPart{ nullptr };
		winrt::XamlToolkit::Labs::WinUI::ResizeThumb BottomRightThumbPart{ nullptr };

		UIElement::ManipulationDelta_revoker _manipulationDeltaRevoker;
		std::array<winrt::event_token, 8> _thumbTokens{};
	};
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
	struct ResizeElementAdorner : ResizeElementAdornerT<ResizeElementAdorner, implementation::ResizeElementAdorner>
	{
	};
}
