#include "pch.h"
#include "MetadataControl.h"
#if __has_include("MetadataControl.g.cpp")
#include "MetadataControl.g.cpp"
#endif
#include <winrt/Microsoft.UI.Xaml.Documents.h>

using namespace Microsoft::UI::Xaml::Documents;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	MetadataControl::MetadataControl() : _textContainer(nullptr)
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
		ActualThemeChanged({ this, &MetadataControl::OnActualThemeChanged });
	}

	void MetadataControl::OnApplyTemplate()
	{
		_textContainer = GetTemplateChild(TextContainerPart).try_as<TextBlock>();
		Update();
	}

	void MetadataControl::OnMetadataItemsChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto self = winrt::get_self<MetadataControl>(control)->get_strong();

			if (auto oldVec = e.OldValue().try_as<IObservableVector<IInspectable>>())
			{
				oldVec.VectorChanged(self->_vectorChangedToken);
			}

			if (auto newVec = e.NewValue().try_as<IObservableVector<IInspectable>>())
			{
				self->_vectorChangedToken = newVec.VectorChanged(
					[control](auto&, auto&)
					{
						auto self = winrt::get_self<MetadataControl>(control)->get_strong();
						self->Update();
					});
			}

			self->Update();
		}
	}

	void MetadataControl::OnPropertyChanged(DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
	{
		if (auto control = d.try_as<class_type>())
		{
			auto self = winrt::get_self<MetadataControl>(control)->get_strong();
			self->Update();
		}
	}

	void MetadataControl::OnActualThemeChanged([[maybe_unused]] FrameworkElement const& sender, [[maybe_unused]] IInspectable const& args) { Update(); }

	void MetadataControl::Update()
	{
		if (_textContainer == nullptr)
		{
			// The template is not ready yet.
			return;
		}

		_textContainer.Inlines().Clear();

		if (Items() == nullptr)
		{
			AutomationProperties::SetName(_textContainer, L"");
			NotifyLiveRegionChanged();
			return;
		}

		Inline unitToAppend{ nullptr };
		std::wstring accessibleString;
		for (const auto& unit : Items())
		{
			if (_textContainer.Inlines().Size() > 0)
			{
				Run run;
				run.Text(Separator());
				_textContainer.Inlines().Append(run);

				accessibleString.append(!AccessibleSeparator().empty() ? AccessibleSeparator() : Separator());
			}

			Run run;
			run.Text(unit.Label());
			unitToAppend = run;

			if (unit.Command())
			{
				Hyperlink hyperLink;
				hyperLink.UnderlineStyle(UnderlineStyle::None);
				hyperLink.Foreground(_textContainer.Foreground());
				hyperLink.Inlines().Append(unitToAppend);

				hyperLink.Click([weakUnit{ winrt::make_weak(unit) }](auto&, auto&)
					{
						if (auto strongUnit = weakUnit.get())
						{
							if (strongUnit.Command().CanExecute(strongUnit.CommandParameter()))
							{
								strongUnit.Command().Execute(strongUnit.CommandParameter());
							}
						}
					});

				unitToAppend = hyperLink;
			}

			auto unitAccessibleLabel = !unit.AccessibleLabel().empty() ? unit.AccessibleLabel() : unit.Label();
			AutomationProperties::SetName(unitToAppend, unitAccessibleLabel);
			accessibleString.append(unitAccessibleLabel);

			_textContainer.Inlines().Append(unitToAppend);
		}

		AutomationProperties::SetName(_textContainer, accessibleString);
		NotifyLiveRegionChanged();
	}

	void MetadataControl::NotifyLiveRegionChanged()
	{
		if (AutomationPeer::ListenerExists(AutomationEvents::LiveRegionChanged))
		{
			if (auto peer = FrameworkElementAutomationPeer::FromElement(*this))
			{
				peer.RaiseAutomationEvent(AutomationEvents::LiveRegionChanged);
			}
		}
	}
}
