#pragma once

#include "LinkClickedEventArgs.h"
#include "MarkdownTextBlock.g.h"
#include "Renderer\WinUIRenderer.h"
#include "TextElements/MdFlowDocument.h"
#include <MarkdownTextBlock/MarkdownConfig.h>

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <memory>
#include <string_view>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Documents;
    using namespace XamlToolkit::Labs::WinUI::TextElements;
}

namespace winrt::XamlToolkit::Labs::WinUI::implementation
{
    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock>
    {
    private:
        static constexpr auto MarkdownContainerName = L"MarkdownContainer";
        winrt::Grid _container;
        std::shared_ptr<MdFlowDocument> _document;
        std::unique_ptr<WinUIRenderer> _renderer;

    public:

        MarkdownTextBlock() : _container(nullptr), _document(std::make_shared<MdFlowDocument>())
        {
            DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));
        }

        wil::untyped_event<winrt::XamlToolkit::Labs::WinUI::LinkClickedEventArgs> OnLinkClicked;

        bool RaiseLinkClickedEvent(winrt::Uri const& uri);

        static void OnConfigChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
        {
            if (auto markdownTextBlock = d.try_as<class_type>(); markdownTextBlock && e.NewValue())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(markdownTextBlock)->get_strong();

                self->ApplyConfig(self->Config());
            }
        }

        static void OnTextChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
        {
            if (auto markdownTextBlock = d.try_as<class_type>(); markdownTextBlock && e.NewValue())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(markdownTextBlock)->get_strong();
                self->ApplyText(self->Text(), true);
            }
        }

        static void OnIsTextSelectionEnabledChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
        {
            if (auto mtb = d.try_as<class_type>())
            {
                auto self = winrt::get_self<MarkdownTextBlock>(mtb)->get_strong();
                if (self->_document) {
                    self->_document->RichTextBlock().IsTextSelectionEnabled(winrt::unbox_value<bool>(e.NewValue()));
                }
            }
        }

        static inline const wil::single_threaded_property<winrt::DependencyProperty> ConfigProperty = 
            winrt::DependencyProperty::Register(
                L"Config",
                winrt::xaml_typename<winrt::XamlToolkit::Labs::WinUI::MarkdownConfig>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata::Create([](){ return winrt::XamlToolkit::Labs::WinUI::MarkdownConfig::Default(); }, &MarkdownTextBlock::OnConfigChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> TextProperty =
            winrt::DependencyProperty::Register(
                L"Text",
                winrt::xaml_typename<winrt::hstring>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(nullptr, &MarkdownTextBlock::OnTextChanged));

        static inline const wil::single_threaded_property<winrt::DependencyProperty> IsTextSelectionEnabledProperty = 
            winrt::DependencyProperty::Register(
                L"IsTextSelectionEnabled",
                winrt::xaml_typename<bool>(),
                winrt::xaml_typename<class_type>(),
                winrt::PropertyMetadata(winrt::box_value(true), &MarkdownTextBlock::OnIsTextSelectionEnabledChanged));

        winrt::XamlToolkit::Labs::WinUI::MarkdownConfig Config() const { return GetValue(ConfigProperty()).try_as<winrt::XamlToolkit::Labs::WinUI::MarkdownConfig>(); }
        void Config(winrt::XamlToolkit::Labs::WinUI::MarkdownConfig const& value) { SetValue(ConfigProperty(), value); }

        winrt::hstring Text() const { return winrt::unbox_value_or<winrt::hstring>(GetValue(TextProperty()), L""); }
        void Text(winrt::hstring const& value) { SetValue(TextProperty(), winrt::box_value(value)); }

        bool IsTextSelectionEnabled() const { return winrt::unbox_value<bool>(GetValue(IsTextSelectionEnabledProperty())); }
        void IsTextSelectionEnabled(bool value) { SetValue(IsTextSelectionEnabledProperty(), winrt::box_value(value)); }

        void OnApplyTemplate();

        void ApplyConfig(XamlToolkit::Labs::WinUI::MarkdownConfig const& config) const;

        void ApplyText(std::wstring_view text, bool rerender) const;

        void Build();
    };
}

namespace winrt::XamlToolkit::Labs::WinUI::factory_implementation
{
    struct MarkdownTextBlock : MarkdownTextBlockT<MarkdownTextBlock, implementation::MarkdownTextBlock>
    {
    };
}
