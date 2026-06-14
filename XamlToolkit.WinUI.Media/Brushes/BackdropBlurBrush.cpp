// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#include "pch.h"
#include "winrt_module_imports.h"
#include "BackdropBlurBrush.h"
#if __has_include("BackdropBlurBrush.g.cpp")
#include "BackdropBlurBrush.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Media::implementation
{
    const wil::single_threaded_property<winrt::Microsoft::UI::Xaml::DependencyProperty> BackdropBlurBrush::AmountProperty =
        winrt::Microsoft::UI::Xaml::DependencyProperty::Register(
            L"Amount",
            winrt::xaml_typename<double>(),
            winrt::xaml_typename<class_type>(),
            winrt::Microsoft::UI::Xaml::PropertyMetadata{ winrt::box_value(0.0), &BackdropBlurBrush::OnAmountChanged });

    double BackdropBlurBrush::Amount() const
    {
        return winrt::unbox_value<double>(GetValue(AmountProperty()));
    }

    void BackdropBlurBrush::Amount(double value)
    {
        SetValue(AmountProperty(), winrt::box_value(value));
    }

    void BackdropBlurBrush::OnAmountChanged(
        winrt::Microsoft::UI::Xaml::DependencyObject const& d,
        [[maybe_unused]] winrt::Microsoft::UI::Xaml::DependencyPropertyChangedEventArgs const& e)
    {
        auto brush = d.as<winrt::XamlToolkit::WinUI::Media::BackdropBlurBrush>();
		auto self = winrt::get_self<BackdropBlurBrush>(brush);
        if (auto target = self->CompositionBrush())
        {
            if (self->_amountSetter)
            {
                self->_amountSetter(target, static_cast<float>(self->Amount()));
            }
        }
    }

    Pipelines::PipelineBuilder BackdropBlurBrush::OnPipelineRequested()
    {
        return winrt::make<Pipelines::implementation::PipelineBuilder>(
            Pipelines::PipelineBuilderImpl::FromBackdrop()
            .Blur(static_cast<float>(Amount()), _amountSetter));
    }
}