#include "pch.h"
#include "winrt_module_imports.h"
#include "FrameworkElementExtensions.h"
#if __has_include("FrameworkElementExtensions.g.cpp")
#include "FrameworkElementExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::implementation
{
    void FrameworkElementExtensions::RemoveHandlers() noexcept
    {
        for (auto it = _handlerStates.begin(); it != _handlerStates.end();)
        {
            if (!it->first.get())
            {
                it = _handlerStates.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void FrameworkElementExtensions::AncestorType_PropertyChanged(winrt::DependencyObject const& obj, winrt::DependencyPropertyChangedEventArgs const& args)
    {
        if (auto fe = obj.try_as<winrt::FrameworkElement>())
        {
            RemoveHandlers();

            if (args.NewValue())
            {
                auto [it, inserted] = _handlerStates.try_emplace(fe);
                auto& handlerState = it->second;

                if (!inserted)
                {
                    fe.Loaded(handlerState._loadedToken);
                }

                handlerState._loadedToken = fe.Loaded(&FrameworkElementExtensions::FrameworkElement_Loaded);

                if (fe.Parent())
                {
                    FrameworkElement_Loaded(fe, nullptr);
                }
            }
        }
    }

    winrt::DependencyObject FrameworkElementExtensions::FindAscendant(winrt::DependencyObject element, winrt::TypeName const& name)
    {
        while (true)
        {
            auto parent = winrt::VisualTreeHelper::GetParent(element);

            if (!parent || winrt::get_class_name(parent) == name.Name)
            {
                return parent;
            }

            element = parent;
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Loaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<winrt::FrameworkElement>())
        {
            SetAncestor(fe, FindAscendant(fe, GetAncestorType(fe)));
            if (const auto it = _handlerStates.find(fe); it != _handlerStates.end())
            {
                fe.Unloaded(it->second._unloadedToken);
                it->second._unloadedToken = fe.Unloaded(&FrameworkElementExtensions::FrameworkElement_Unloaded);
            }
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Unloaded(winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<winrt::FrameworkElement>())
        {
            if (const auto it = _handlerStates.find(fe); it != _handlerStates.end()) 
            {
                fe.Unloaded(it->second._unloadedToken);
            }
           
            SetAncestor(fe, nullptr);
        }
    }

    winrt::IInspectable FrameworkElementExtensions::GetAncestor(winrt::DependencyObject const& obj)
    {
        return obj.GetValue(AncestorProperty);
    }

    void FrameworkElementExtensions::SetAncestor(winrt::DependencyObject const& obj, winrt::IInspectable const& value)
    {
        obj.SetValue(AncestorProperty, value);
    }

    winrt::TypeName FrameworkElementExtensions::GetAncestorType(winrt::FrameworkElement const& obj)
    {
        return winrt::unbox_value<winrt::TypeName>(obj.GetValue(AncestorTypeProperty));
    }

    void FrameworkElementExtensions::SetAncestorType(winrt::FrameworkElement const& obj, winrt::TypeName const& value)
    {
        obj.SetValue(AncestorTypeProperty, winrt::box_value(value));
    }
}