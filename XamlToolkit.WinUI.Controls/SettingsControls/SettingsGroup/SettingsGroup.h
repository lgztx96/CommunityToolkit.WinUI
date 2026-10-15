#pragma once

#include "SettingsGroup.g.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::UI::Xaml::Interop;

    using namespace winrt::Microsoft::UI::Xaml;
    using namespace winrt::Microsoft::UI::Xaml::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    struct SettingsGroup : SettingsGroupT<SettingsGroup>
    {
        static constexpr auto PartContentPresenter = L"ContentPresenter";
        static constexpr auto PartHeaderIconPresenter = L"HeaderIconPresenter";
        static constexpr auto PartHeaderPresenter = L"HeaderPresenter";
        static constexpr auto PartDescriptionPresenter = L"DescriptionPresenter";
        static constexpr auto PartRootGrid = L"RootGrid";
        static constexpr auto PartItemsRepeater = L"ItemsRepeater";

        SettingsGroup();

        void OnApplyTemplate();

        void SetEnabledState();

#pragma region Properties

        static const wil::single_threaded_property<winrt::DependencyProperty> RootGridCornerRadiusProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ItemsProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> ContentProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> HeaderIconProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> HeaderProperty;

        static const wil::single_threaded_property<winrt::DependencyProperty> DescriptionProperty;

        [[nodiscard]] winrt::CornerRadius RootGridCornerRadius() const noexcept;
        void RootGridCornerRadius(winrt::CornerRadius const& value) noexcept;

        [[nodiscrad]] winrt::IObservableVector<winrt::IInspectable> Items() const noexcept;
        void Items(winrt::IObservableVector<winrt::IInspectable> const& value) noexcept;

        [[nodiscard]] winrt::IInspectable Content() const noexcept;
        void Content(winrt::IInspectable const& value) noexcept;

        [[nodiscard]] winrt::IconElement HeaderIcon() const noexcept;
        void HeaderIcon(winrt::IconElement const& value) noexcept;

        [[nodiscard]] winrt::IInspectable Header() const noexcept;
        void Header(winrt::IInspectable const& value) noexcept;

        [[nodiscard]] winrt::IInspectable Description() const noexcept;
        void Description(winrt::IInspectable const& value) noexcept;

#pragma endregion

#pragma region Callbacks

        static void OnItemsChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnContentChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnHeaderIconChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnHeaderChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static void OnDescriptionChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        void RaiseItemsChanged();

        void RaiseContentChanged();

        void RaiseDescriptionChanged();

        void RaiseHeaderChanged();

        void RaiseHeaderIconChanged();

        void Items_CollectionChanged(winrt::IInspectable const& s, winrt::IVectorChangedEventArgs const& e);

        void SettingsGroup_IsEnabledChanged(winrt::IInspectable const& s, DependencyPropertyChangedEventArgs const& e);

#pragma endregion

    private:

        SettingsGroup::IsEnabledChanged_revoker _isEnabledChangedRevoker;

        winrt::IObservableVector<winrt::IInspectable>::VectorChanged_revoker _itemsVectorChangedRevoker_;

        winrt::ItemsRepeater _itemsRepeater;

        winrt::ContentPresenter _contentPresenter;

        winrt::ContentPresenter _descriptionPresenter;

        winrt::ContentPresenter _headerPresenter;

        winrt::Grid _rootGrid;

        static winrt::XamlToolkit::WinUI::Converters::ItemsCountToCornerRadiusConverter _radiusConverter;

    private:

        [[nodiscard]] winrt::CornerRadius GetRootGridCornerRadius();
    };
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
    struct SettingsGroup : SettingsGroupT<SettingsGroup, implementation::SettingsGroup>
    {
    };
}
