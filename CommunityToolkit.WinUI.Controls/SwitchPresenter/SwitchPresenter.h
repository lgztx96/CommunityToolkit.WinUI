#pragma once

#include "SwitchPresenter.g.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
    struct SwitchPresenter : SwitchPresenterT<SwitchPresenter>
    {
        winrt::CommunityToolkit::WinUI::Controls::Case CurrentCase()
        {
            return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::Case>(GetValue(CurrentCaseProperty));
        }

        void CurrentCase(winrt::CommunityToolkit::WinUI::Controls::Case const& value)
        {
            SetValue(CurrentCaseProperty, value);
        }

        static inline const wil::single_threaded_property<DependencyProperty> CurrentCaseProperty =
            DependencyProperty::Register(
                L"CurrentCase",
                winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::Case>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr));

        Controls::CaseCollection SwitchCases()
        {
            return winrt::unbox_value<Controls::CaseCollection>(GetValue(SwitchCasesProperty));
        }

        void SwitchCases(Controls::CaseCollection const& value)
        {
            SetValue(SwitchCasesProperty, value);
        }

        static void OnSwitchCasesPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> SwitchCasesProperty =
            DependencyProperty::Register(
                L"SwitchCases",
                winrt::xaml_typename<Controls::CaseCollection>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr, &SwitchPresenter::OnSwitchCasesPropertyChanged));

        winrt::Windows::Foundation::IInspectable Value()
        {
            return GetValue(ValueProperty);
        }

        void Value(winrt::Windows::Foundation::IInspectable const& value)
        {
            SetValue(ValueProperty, value);
        }

        static void OnValuePropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e);

        static inline const wil::single_threaded_property<DependencyProperty> ValueProperty =
            DependencyProperty::Register(
                L"Value",
                winrt::xaml_typename<winrt::Windows::Foundation::IInspectable>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr, &SwitchPresenter::OnValuePropertyChanged));

        IReference<winrt::Windows::UI::Xaml::Interop::TypeName> TargetType()
        {
            return GetValue(TargetTypeProperty).try_as<IReference<winrt::Windows::UI::Xaml::Interop::TypeName>>();
        }

        void TargetType(IReference<winrt::Windows::UI::Xaml::Interop::TypeName> const& value)
        {
            SetValue(TargetTypeProperty, winrt::box_value(value));
        }

        static inline const wil::single_threaded_property<DependencyProperty> TargetTypeProperty =
            DependencyProperty::Register(
                L"TargetType",
                winrt::xaml_typename<winrt::Windows::UI::Xaml::Interop::TypeName>(),
                winrt::xaml_typename<class_type>(),
                PropertyMetadata(nullptr));

        SwitchPresenter();

        void OnApplyTemplate();

    private:

        void SwitchPresenter_Loaded(IInspectable const& sender, RoutedEventArgs const& e);

        void EvaluateCases();
    };
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
    struct SwitchPresenter : SwitchPresenterT<SwitchPresenter, implementation::SwitchPresenter>
    {
    };
}
