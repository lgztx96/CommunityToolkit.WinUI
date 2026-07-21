// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "UIElementExtensions.h"
#if __has_include("UIElementExtensions.g.cpp")
#include "UIElementExtensions.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::DependencyProperty> UIElementExtensions::VisualFactoryProperty =
        winrt::DependencyProperty::RegisterAttached(
            L"VisualFactory",
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Media::AttachedVisualFactoryBase>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Media::UIElementExtensions>(),
            winrt::PropertyMetadata(nullptr, &UIElementExtensions::OnVisualFactoryPropertyChanged));

    AttachedVisualFactoryBase UIElementExtensions::GetVisualFactory(winrt::UIElement const& element)
    {
        return element.GetValue(VisualFactoryProperty()).try_as<AttachedVisualFactoryBase>();
    }

    void UIElementExtensions::SetVisualFactory(winrt::UIElement const& element, AttachedVisualFactoryBase const& value)
    {
        element.SetValue(VisualFactoryProperty(), value);
    }

    winrt::fire_and_forget UIElementExtensions::OnVisualFactoryPropertyChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
    {
        winrt::UIElement element = d.as<winrt::UIElement>();
        if (auto factory = e.NewValue().try_as<AttachedVisualFactoryBase>())
        {
            winrt::Visual attachedVisual = co_await factory.GetAttachedVisualAsync(element);
            attachedVisual.RelativeSizeAdjustment(winrt::float2::one());
            winrt::ElementCompositionPreview::SetElementChildVisual(element, attachedVisual);
        }
    }
}
