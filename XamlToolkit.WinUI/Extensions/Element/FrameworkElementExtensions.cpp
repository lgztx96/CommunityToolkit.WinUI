#include "pch.h"
#include "winrt_module_imports.h"
#include "FrameworkElementExtensions.h"
#if __has_include("FrameworkElementExtensions.g.cpp")
#include "FrameworkElementExtensions.g.cpp"
#endif
#include "../Tree/DependencyObjectExtensions.h"

namespace winrt::XamlToolkit::WinUI::implementation
{
    winrt::com_ptr<AncestorTracker> FrameworkElementExtensions::GetAncestorTracker(winrt::FrameworkElement const& element)
    {
        auto value = element.GetValue(AncestorTrackerProperty());
        return winrt::get_self<AncestorTracker>(value)->get_strong();
    }

    winrt::IInspectable FrameworkElementExtensions::GetAncestor(winrt::DependencyObject const& obj)
    {
        return obj.GetValue(AncestorProperty());
    }

    void FrameworkElementExtensions::SetAncestor(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
    {
        obj.SetValue(AncestorProperty(), value);
    }

    winrt::TypeName FrameworkElementExtensions::GetAncestorType(winrt::FrameworkElement const& obj)
    {
        return winrt::unbox_value<winrt::TypeName>(obj.GetValue(AncestorTypeProperty()));
    }

    void FrameworkElementExtensions::SetAncestorType(winrt::FrameworkElement const& obj, winrt::TypeName const& value)
    {
        obj.SetValue(AncestorTypeProperty(), winrt::box_value(value));
    }

    void FrameworkElementExtensions::AncestorType_PropertyChanged(winrt::DependencyObject const& obj, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto fe = obj.try_as<winrt::FrameworkElement>())
        {
            if (args.NewValue())
            {
                auto ancestorTracker = GetAncestorTracker(fe);

                if (!ancestorTracker)
                {
                    ancestorTracker = winrt::make_self<AncestorTracker>();
                    fe.SetValue(AncestorTrackerProperty(), *ancestorTracker);
                }
                else
                {
                    if (ancestorTracker->loadedToken) 
                    { 
                        fe.Loaded(ancestorTracker->loadedToken); 
                    }
                }

                ancestorTracker->loadedToken = fe.Loaded(&FrameworkElementExtensions::FrameworkElement_Loaded);

                if (fe.Parent())
                {
                    FrameworkElement_Loaded(fe, nullptr);
                }
            }
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<winrt::FrameworkElement>())
        {
            SetAncestor(fe, DependencyObjectEx::FindAscendant(fe, GetAncestorType(fe)));

            if (auto ancestorTracker = GetAncestorTracker(fe))
            {
                if (ancestorTracker->unloadedToken)
                {
                    fe.Unloaded(ancestorTracker->unloadedToken);
                }

                ancestorTracker->unloadedToken = fe.Unloaded(&FrameworkElementExtensions::FrameworkElement_Unloaded);
            }
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Unloaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<winrt::FrameworkElement>())
        {
            if (auto ancestorTracker = GetAncestorTracker(fe))
            {
                if (ancestorTracker->unloadedToken)
                {
                    fe.Unloaded(ancestorTracker->unloadedToken);
                    ancestorTracker->unloadedToken = { 0 };
                }
            }

            SetAncestor(fe, nullptr);
        }
    }
}