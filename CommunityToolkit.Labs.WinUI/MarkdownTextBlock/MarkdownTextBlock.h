#pragma once

#include "LinkClickedEventArgs.h"
#include "MarkdownTextBlock.g.h"
#include "Renderer\WinUIRenderer.h"
#include "TextElements/MdFlowDocument.h"
#include <MarkdownTextBlock/MarkdownConfig.h>
#include <winrt/CommunityToolkit.Labs.WinUI.h>

namespace winrt::CommunityToolkit::Labs::WinUI::implementation
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
    using namespace winrt::CommunityToolkit::Labs::WinUI::TextElements;

    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock>
    {
    private:
        static constexpr auto MarkdownContainerName = L"MarkdownContainer";
        Grid _container;
        std::shared_ptr<MdFlowDocument> _document;
        std::unique_ptr<WinUIRenderer> _renderer;

    public:

        MarkdownTextBlock() : _container(nullptr), _document(std::make_shared<MdFlowDocument>())
        {
            DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        }

        wil::typed_event<class_type, LinkClickedEventArgs::class_type> OnLinkClicked;

        bool RaiseLinkClickedEvent(winrt::Windows::Foundation::Uri const& uri);

        static void OnConfigChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
        {
            if (auto markdownTextBlock = d.try_as<class_type>(); markdownTextBlock && e.NewValue())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(markdownTextBlock)->get_strong();

                self->ApplyConfig(self->Config());
            }
        }

        static void OnTextChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
        {
            if (auto markdownTextBlock = d.try_as<class_type>(); markdownTextBlock && e.NewValue())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(markdownTextBlock)->get_strong();
                self->ApplyText(self->Text(), true);
            }
        }

        static void OnIsTextSelectionEnabledChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
        {
            if (auto mtb = d.try_as<class_type>())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(mtb)->get_strong();
                if (self->_document) {
                    self->_document->RichTextBlock().IsTextSelectionEnabled(winrt::unbox_value<bool>(e.NewValue()));
                }
            }
        }

        static inline const wil::single_threaded_property<DependencyProperty> ConfigProperty = DependencyProperty::Register(
            L"Config",
            winrt::xaml_typename<CommunityToolkit::Labs::WinUI::MarkdownConfig>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr, MarkdownTextBlock::OnConfigChanged));

        static inline const wil::single_threaded_property<DependencyProperty> TextProperty = DependencyProperty::Register(
            L"Text",
            winrt::xaml_typename<winrt::hstring>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(nullptr, MarkdownTextBlock::OnTextChanged));

        static inline const wil::single_threaded_property<DependencyProperty> IsTextSelectionEnabledProperty = DependencyProperty::Register(
            L"IsTextSelectionEnabled",
            winrt::xaml_typename<bool>(),
            winrt::xaml_typename<class_type>(),
            PropertyMetadata(winrt::box_value(true), OnIsTextSelectionEnabledChanged));

        CommunityToolkit::Labs::WinUI::MarkdownConfig Config() { return winrt::unbox_value<CommunityToolkit::Labs::WinUI::MarkdownConfig>(GetValue(ConfigProperty)); }
        void Config(CommunityToolkit::Labs::WinUI::MarkdownConfig const& value) { SetValue(ConfigProperty, value); }

        winrt::hstring Text() { return winrt::unbox_value_or<winrt::hstring>(GetValue(TextProperty), L""); }
        void Text(winrt::hstring const& value) { SetValue(TextProperty, winrt::box_value(value)); }

        bool IsTextSelectionEnabled() { return winrt::unbox_value<bool>(GetValue(IsTextSelectionEnabledProperty)); }
        void IsTextSelectionEnabled(bool value) { SetValue(IsTextSelectionEnabledProperty, winrt::box_value(value)); }

        void OnApplyTemplate();

        void ApplyConfig(CommunityToolkit::Labs::WinUI::MarkdownConfig const& config) const;

        void ApplyText(std::wstring_view text, bool rerender) const;

        void Build();
    };
}

namespace winrt::CommunityToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock, implementation::MarkdownTextBlock>
    {
    };
}
