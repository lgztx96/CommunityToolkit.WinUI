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
    const wil::single_threaded_property<DependencyProperty> UIElementExtensions::VisualFactoryProperty =
        DependencyProperty::RegisterAttached(
            L"VisualFactory",
            winrt::xaml_typename<XamlToolkit::WinUI::Media::AttachedVisualFactoryBase>(),
            winrt::xaml_typename<winrt::XamlToolkit::WinUI::Media::UIElementExtensions>(),
            PropertyMetadata(nullptr, &UIElementExtensions::OnVisualFactoryPropertyChanged));

    AttachedVisualFactoryBase UIElementExtensions::GetVisualFactory(UIElement const& element)
    {
        return element.GetValue(VisualFactoryProperty).try_as<AttachedVisualFactoryBase>();
    }

    void UIElementExtensions::SetVisualFactory(UIElement const& element, AttachedVisualFactoryBase const& value)
    {
        element.SetValue(VisualFactoryProperty, value);
    }

    winrt::fire_and_forget UIElementExtensions::OnVisualFactoryPropertyChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
    {
        UIElement element = d.as<UIElement>();
        if (auto factory = e.NewValue().try_as<AttachedVisualFactoryBase>())
        {
            Visual attachedVisual = co_await factory.GetAttachedVisualAsync(element);
            attachedVisual.RelativeSizeAdjustment(float2::one());
            ElementCompositionPreview::SetElementChildVisual(element, attachedVisual);
        }
    }
}
