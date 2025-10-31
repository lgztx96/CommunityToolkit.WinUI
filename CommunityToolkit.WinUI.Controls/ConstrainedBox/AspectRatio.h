#pragma once

#include "../TokenizingTextBox/StringExtensions.h"
#include "AspectRatio.g.h"
#include <ranges>
#include <string>
#include <vector>
#include <winrt/base.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

using namespace std::literals;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	struct AspectRatio : AspectRatioT<AspectRatio>
	{
		wil::single_threaded_property<double> Width;

		wil::single_threaded_property<double> Height;

		double Value() const { return Width / Height; }

		AspectRatio(double width, double height) : Width(width), Height(height) {}

		AspectRatio(double ratio) : Width(ratio), Height(1) {}

		operator double() const { return Value(); }

		static winrt::CommunityToolkit::WinUI::Controls::AspectRatio ConvertToAspectRatio(winrt::hstring const& rawString);

		winrt::hstring ToString() const;
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct AspectRatio : AspectRatioT<AspectRatio, implementation::AspectRatio>
	{
	};
}
