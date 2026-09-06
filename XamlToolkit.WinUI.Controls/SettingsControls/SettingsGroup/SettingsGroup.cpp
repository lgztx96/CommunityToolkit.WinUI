#include "pch.h"
#include "winrt_module_imports.h"
#include "SettingsGroup.h"
#if __has_include("SettingsGroup.g.cpp")
#include "SettingsGroup.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	SettingsGroup::SettingsGroup()
	{
		Items(winrt::single_threaded_observable_vector<winrt::IInspectable>());
		_itemsVectorChangedRevoker_ = Items().VectorChanged(winrt::auto_revoke, { this, &SettingsGroup::Items_CollectionChanged });
	}

	void SettingsGroup::OnApplyTemplate()
	{
		_itemsRepeater = GetTemplateChild(PartItemsRepeater).try_as<winrt::ItemsRepeater>();
		_contentPresenter = GetTemplateChild(PartContentPresenter).try_as<winrt::ContentPresenter>();
		_headerPresenter = GetTemplateChild(PartHeaderPresenter).try_as<winrt::ContentPresenter>();
		_descriptionPresenter = GetTemplateChild(PartDescriptionPresenter).try_as<winrt::ContentPresenter>();
		_rootGrid = GetTemplateChild(PartRootGrid).try_as<winrt::Grid>();

		if (_itemsRepeater)
		{
			RaiseItemsChanged();
		}

		SetEnabledState();

		_isEnabledChangedRevoker = IsEnabledChanged(winrt::auto_revoke, { this, &SettingsGroup::SettingsGroup_IsEnabledChanged });

		RaiseHeaderIconChanged();
		RaiseHeaderChanged();
		RaiseDescriptionChanged();
		RaiseContentChanged();

		RootGridCornerRadius(GetRootGridCornerRadius());

		base_type::OnApplyTemplate();
	}

	void SettingsGroup::SetEnabledState()
	{
		winrt::VisualStateManager::GoToState(*this, IsEnabled() ? L"Normal" : L"Disabled", true);
	}

	winrt::CornerRadius SettingsGroup::GetRootGridCornerRadius()
	{
		return _radiusConverter.Convert(winrt::box_value(Items().Size()), winrt::TypeName{}, nullptr, winrt::hstring{}).as<winrt::CornerRadius>();
	}
}
