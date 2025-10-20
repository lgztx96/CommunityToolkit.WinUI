#include "pch.h"
#include "Effects.h"
#if __has_include("Effects.g.cpp")
#include "Effects.g.cpp"
#endif

namespace winrt::CommunityToolkit::WinUI::implementation
{
    void Effects::OnShadowChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        if (auto element = d.try_as<FrameworkElement>())
        {
            if (auto oldShadow = e.OldValue().try_as<CommunityToolkit::WinUI::AttachedShadowBase>())
            {
                oldShadow.DisconnectElement(element);
            }

            if (auto newShadow = e.NewValue().try_as<CommunityToolkit::WinUI::AttachedShadowBase>())
            {
                newShadow.ConnectElement(element);
            }
        }
    }
}
