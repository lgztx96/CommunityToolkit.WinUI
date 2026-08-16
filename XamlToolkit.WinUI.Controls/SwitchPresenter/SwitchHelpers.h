#pragma once

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/XamlToolkit.WinUI.Controls.h>
#include <ranges>
#endif
#include "../../XamlToolkit.WinUI/Extensions/Foundation/EqualsHelper.h"

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::UI::Xaml::Interop;
	using namespace Microsoft::UI::Xaml::Markup;
}

namespace winrt::XamlToolkit::WinUI::Controls
{
	struct SwitchHelpers
	{
		static Case EvaluateCases(CaseCollection const& switchCases, winrt::IInspectable const& value, std::optional<winrt::TypeName> const& targetType = std::nullopt)
		{
			if (switchCases == nullptr || 
				switchCases.Size() == 0)
			{
				// If we have no cases, then we can't match anything.
				return nullptr;
			}

			Case xdefault { nullptr };
			Case newcase { nullptr };

			auto collection = switchCases
				| std::views::transform([](auto&& item) { return item.template as<Case>(); });

			for (const Case& xcase : collection)
			{
				if (xcase.IsDefault())
				{
					// If there are multiple default cases provided, this will override and just grab the last one, the developer will have to fix this in their XAML. We call this out in the case comments.
					xdefault = xcase;
					continue;
				}

				if (CompareValues(value, xcase.Value(), targetType))
				{
					newcase = xcase;
					break;
				}
			}

			if (newcase == nullptr && xdefault)
			{
				// Inject default if we found one without matching anything
				newcase = xdefault;
			}

			return newcase;
		}

		static bool CompareValues(winrt::IInspectable const& compare, winrt::IInspectable const& value,
			[[maybe_unused]] std::optional<winrt::TypeName> const& targetType = std::nullopt)
		{
			return EqualsHelper::ConvertibleEquals(compare, value);
		}
	};
}
