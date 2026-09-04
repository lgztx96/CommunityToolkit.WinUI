#include "pch.h"
#include "winrt_module_imports.h"
#include "SwitchPresenter.h"
#if __has_include("SwitchPresenter.g.cpp")
#include "SwitchPresenter.g.cpp"
#endif
#include "SwitchHelpers.h"
#include "CaseCollection.h"

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
    SwitchPresenter::SwitchPresenter()
    {
        SwitchCases(winrt::make<implementation::CaseCollection>());
		Loaded({ this, &SwitchPresenter::SwitchPresenter_Loaded });
    }

    void SwitchPresenter::OnValuePropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // When our Switch's expression changes, re-evaluate.
        if (auto xswitch = d.try_as<class_type>())
        {
            auto self = winrt::get_self<SwitchPresenter>(xswitch);
            self->EvaluateCases();
        }
    }

    void SwitchPresenter::OnSwitchCasesPropertyChanged(winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
    {
        // If our collection somehow changes, we should re-evaluate.
        if (auto xswitch = d.try_as<class_type>())
        {
			auto self = winrt::get_self<SwitchPresenter>(xswitch);
            self->EvaluateCases();
        }
    }

	void SwitchPresenter::SwitchPresenter_Loaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        // In case we're in a template, we may have loaded cases later.
        EvaluateCases();
    }

    void SwitchPresenter::OnApplyTemplate()
    {
        base_type::OnApplyTemplate();

        EvaluateCases();
    }

    void SwitchPresenter::EvaluateCases()
    {
        const auto value = Value();
        const auto currentCase = CurrentCase();

        if (currentCase && currentCase.Value() == value)
        {
            // If the current case we're on already matches our current value,
            // then we don't have any work to do.
            return;
        }

        auto result = SwitchHelpers::EvaluateCases(SwitchCases(), value);

        // Only bother changing things around if we actually have a new case. (this should handle prior null case as well)
        if (result != currentCase)
        {
            // If we don't have any cases or default, setting these to null is what we want to be blank again.
            if (auto content = result.Content()) 
            {
                Content(content);
            }

            CurrentCase(result);
        }
    }
}
