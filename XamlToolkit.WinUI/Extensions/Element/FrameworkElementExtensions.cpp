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

    void FrameworkElementExtensions::AncestorType_PropertyChanged(DependencyObject const& obj, DependencyPropertyChangedEventArgs const& args)
    {
        if (auto fe = obj.try_as<FrameworkElement>())
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

    DependencyObject FrameworkElementExtensions::FindAscendant(DependencyObject element, TypeName const& name)
    {
        while (true)
        {
            auto parent = winrt::Microsoft::UI::Xaml::Media::VisualTreeHelper::GetParent(element);

            if (!parent || winrt::get_class_name(parent) == name.Name)
            {
                return parent;
            }

            element = parent;
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Loaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<FrameworkElement>())
        {
            SetAncestor(fe, FindAscendant(fe, GetAncestorType(fe)));
            if (const auto it = _handlerStates.find(fe); it != _handlerStates.end())
            {
                fe.Unloaded(it->second._unloadedToken);
                it->second._unloadedToken = fe.Unloaded(&FrameworkElementExtensions::FrameworkElement_Unloaded);
            }
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Unloaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<FrameworkElement>())
        {
            if (const auto it = _handlerStates.find(fe); it != _handlerStates.end()) 
            {
                fe.Unloaded(it->second._unloadedToken);
            }
           
            SetAncestor(fe, nullptr);
        }
    }

    IInspectable FrameworkElementExtensions::GetAncestor(DependencyObject const& obj)
    {
        return obj.GetValue(AncestorProperty);
    }

    void FrameworkElementExtensions::SetAncestor(DependencyObject const& obj, IInspectable const& value)
    {
        obj.SetValue(AncestorProperty, value);
    }

    TypeName FrameworkElementExtensions::GetAncestorType(FrameworkElement const& obj)
    {
        return winrt::unbox_value<TypeName>(obj.GetValue(AncestorTypeProperty));
    }

    void FrameworkElementExtensions::SetAncestorType(FrameworkElement const& obj, TypeName const& value)
    {
        obj.SetValue(AncestorTypeProperty, winrt::box_value(value));
    }
}