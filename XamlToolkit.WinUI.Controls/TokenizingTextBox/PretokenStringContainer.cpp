#include "pch.h"
#include "winrt_module_imports.h"
#include "PretokenStringContainer.h"
#if __has_include("PretokenStringContainer.g.cpp")
#include "PretokenStringContainer.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> PretokenStringContainer::TextProperty =
		winrt::DependencyProperty::Register(
			L"Text",
			winrt::xaml_typename<winrt::hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(L"")));

	winrt::hstring PretokenStringContainer::ToString() const
	{
		return Text();
	}
}
