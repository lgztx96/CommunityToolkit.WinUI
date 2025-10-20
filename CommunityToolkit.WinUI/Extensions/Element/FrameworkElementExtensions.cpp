#include "pch.h"
#include "FrameworkElementExtensions.h"
#if __has_include("FrameworkElementExtensions.g.cpp")
#include "FrameworkElementExtensions.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::implementation
{
    void FrameworkElementExtensions::AncestorType_PropertyChanged(DependencyObject const& obj, DependencyPropertyChangedEventArgs const& args)
    {
        if (auto fe = obj.try_as<FrameworkElement>())
        {
            _ancestorLoadRevoker.revoke();
            if (args.NewValue() != nullptr)
            {
                _ancestorLoadRevoker = fe.Loaded(winrt::auto_revoke, FrameworkElement_Loaded);
                if (fe.Parent() != nullptr)
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
            DependencyObject parent = winrt::Microsoft::UI::Xaml::Media::VisualTreeHelper::GetParent(element);

            if (!parent)
            {
                return { nullptr };
            }

            if (auto result = winrt::get_class_name(parent); result == name.Name)
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

            _elementUnloadRevoker = fe.Unloaded(winrt::auto_revoke, FrameworkElement_Unloaded);
        }
    }

    void FrameworkElementExtensions::FrameworkElement_Unloaded(IInspectable const& sender, [[maybe_unused]] RoutedEventArgs const& e)
    {
        if (auto fe = sender.try_as<FrameworkElement>())
        {
            _elementUnloadRevoker.revoke();
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
