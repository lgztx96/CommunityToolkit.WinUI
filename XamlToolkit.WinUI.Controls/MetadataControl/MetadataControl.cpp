#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Automation.h>
#include <winrt/Microsoft.UI.Xaml.Automation.Peers.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif
#include "MetadataControl.h"
#if __has_include("MetadataControl.g.cpp")
#include "MetadataControl.g.cpp"
#endif

namespace winrt
{
	using namespace Microsoft::UI::Xaml::Documents;
	using namespace Microsoft::UI::Xaml::Automation;
	using namespace Microsoft::UI::Xaml::Automation::Peers;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> MetadataControl::SeparatorProperty =
		winrt::DependencyProperty::Register(
			L"Separator",
			winrt::xaml_typename<hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(L" • "), &MetadataControl::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> MetadataControl::AccessibleSeparatorProperty =
		winrt::DependencyProperty::Register(
			L"AccessibleSeparator",
			winrt::xaml_typename<hstring>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ winrt::box_value(L", "), &MetadataControl::OnPropertyChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> MetadataControl::ItemsProperty =
		winrt::DependencyProperty::Register(
			L"Items",
			winrt::xaml_typename<winrt::IVector<MetadataItem>>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr, &MetadataControl::OnMetadataItemsChanged });

	const wil::single_threaded_property<winrt::DependencyProperty> MetadataControl::TextBlockStyleProperty =
		winrt::DependencyProperty::Register(
			L"TextBlockStyle",
			winrt::xaml_typename<winrt::Style>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata{ nullptr });

	MetadataControl::MetadataControl() : _textContainer(nullptr)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		ActualThemeChanged({ this, &MetadataControl::OnActualThemeChanged });
	}

	void MetadataControl::OnApplyTemplate()
	{
		_textContainer = GetTemplateChild(TextContainerPart).try_as<winrt::TextBlock>();
		Update();
	}

	void MetadataControl::OnMetadataItemsChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto self = winrt::get_self<MetadataControl>(control)->get_strong();

			if (auto oldVec = e.OldValue().try_as<winrt::IObservableVector<winrt::IInspectable>>())
			{
				self->_vectorChangedRevoker.revoke();
			}

			if (auto newVec = e.NewValue().try_as<winrt::IObservableVector<winrt::IInspectable>>())
			{
				self->_vectorChangedRevoker = newVec.VectorChanged(winrt::auto_revoke, [controlWeak{ winrt::make_weak(control) }](auto&, auto&)
				{
					if (auto controlStrong = controlWeak.get()) 
					{
						auto self = winrt::get_self<MetadataControl>(controlStrong)->get_strong();
						self->Update();
					}
				});
			}

			self->Update();
		}
	}

	void MetadataControl::OnPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto self = winrt::get_self<MetadataControl>(control)->get_strong();
			self->Update();
		}
	}

	void MetadataControl::OnActualThemeChanged(
		[[maybe_unused]] winrt::FrameworkElement const& sender, 
		[[maybe_unused]] winrt::IInspectable const& args)
	{ 
		Update();
	}

	void MetadataControl::Update()
	{
		if (_textContainer == nullptr)
		{
			// The template is not ready yet.
			return;
		}

		auto textInlines = _textContainer.Inlines();
		textInlines.Clear();

		auto items = Items();
		if (items == nullptr)
		{
			winrt::AutomationProperties::SetName(_textContainer, L"");
			NotifyLiveRegionChanged();
			return;
		}

		winrt::Inline unitToAppend{ nullptr };
		std::wstring accessibleString;
		for (const auto& unit : items)
		{
			if (textInlines.Size() > 0)
			{
				winrt::Run run;
				run.Text(Separator());
				textInlines.Append(run);
				const auto accessibleSeparator = AccessibleSeparator();
				accessibleString.append(!accessibleSeparator.empty() ? accessibleSeparator : Separator());
			}

			winrt::Run run;
			run.Text(unit.Label());
			unitToAppend = run;

			if (unit.Command())
			{
				winrt::Hyperlink hyperlink;
				hyperlink.UnderlineStyle(winrt::UnderlineStyle::None);
				hyperlink.Foreground(_textContainer.Foreground());
				hyperlink.Inlines().Append(unitToAppend);

				hyperlink.Click([weakUnit{ winrt::make_weak(unit) }](auto&, auto&)
				{
					if (auto strongUnit = weakUnit.get())
					{
						auto command = strongUnit.Command();
						auto parameter = strongUnit.CommandParameter();
						if (command.CanExecute(parameter))
						{
							command.Execute(parameter);
						}
					}
				});

				unitToAppend = hyperlink;
			}

			auto accessibleLabel = unit.AccessibleLabel();
			auto unitAccessibleLabel = !accessibleLabel.empty() ? accessibleLabel : unit.Label();
			winrt::AutomationProperties::SetName(unitToAppend, unitAccessibleLabel);
			accessibleString.append(unitAccessibleLabel);

			textInlines.Append(unitToAppend);
		}

		winrt::AutomationProperties::SetName(_textContainer, accessibleString);
		NotifyLiveRegionChanged();
	}

	void MetadataControl::NotifyLiveRegionChanged()
	{
		if (winrt::AutomationPeer::ListenerExists(winrt::AutomationEvents::LiveRegionChanged))
		{
			if (auto peer = winrt::FrameworkElementAutomationPeer::FromElement(*this))
			{
				peer.RaiseAutomationEvent(winrt::AutomationEvents::LiveRegionChanged);
			}
		}
	}
}
