#pragma once

#include "IncrementalUpdateBehavior.g.h"
#include "../Behavior.h"

#ifdef __INTELLISENSE__
#include <map>
#include <memory>
#include <vector>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#endif

namespace winrt
{
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Xaml::Controls::Primitives;
    using namespace Microsoft::UI::Xaml::Media;
    using namespace Windows::Foundation;
    using namespace Windows::Foundation::Collections;
}

namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
    struct IncrementalUpdater : winrt::implements<IncrementalUpdater, winrt::IInspectable>
    {
    private:
        class PhasedElementRecord
        {
        private:
            winrt::FrameworkElement _frameworkElement;
            winrt::IInspectable _localOpacity;
            winrt::IInspectable _localDataContext;
            bool _isFrozen;

        public:
            PhasedElementRecord(winrt::FrameworkElement const& frameworkElement) : _frameworkElement(frameworkElement), _isFrozen(false) { }

            winrt::FrameworkElement FrameworkElement() const noexcept { return _frameworkElement; }

            void FreezeAndHide();

            void ThawAndShow();
        };

        class ElementCacheRecord
        {
        private:
            std::vector<int> _phases;
            std::vector<std::vector<PhasedElementRecord>> _elementsByPhase;

        public:
            std::vector<int>& Phases() { return _phases; }

            std::vector<std::vector<PhasedElementRecord>>& ElementsByPhase() { return _elementsByPhase; }
        };

    private:
        void OnContainerContentChanging(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& e);

        void OnContainerContentChangingCallback(winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& e);

        static winrt::UIElement FindContentTemplateRoot(winrt::FrameworkElement const& phaseElement);

    public:
        void CachePhaseElement(FrameworkElement const& phaseElement, int phase);

        void UncachePhaseElement(FrameworkElement const& phaseElement, int phase);

        void Attach(DependencyObject const& dependencyObject);

        void Detach();

    private:
        winrt::ListViewBase _associatedListViewBase{ nullptr };
        std::map<winrt::UIElement, std::unique_ptr<ElementCacheRecord>> _elementCache;
        winrt::ListViewBase::ContainerContentChanging_revoker _containerContentChangingRevoker;
    };

    /// <summary>
    /// A behavior that allows incremental updating of <seealso cref="ListView"/> and <seealso cref="GridView"/> contents to support faster updating.
    /// By attaching this behavior to elements in the <seealso cref="ItemsControl.ItemTemplate"/> used by these views, some of the updates can be deferred until there is render time available, resulting in a smoother experience.
    /// </summary>
    struct IncrementalUpdateBehavior : IncrementalUpdateBehaviorT<IncrementalUpdateBehavior, winrt::XamlToolkit::WinUI::Interactivity::implementation::Behavior>
    {
        /// <summary>
        /// Identifies the <seealso cref="Phase"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> PhaseProperty;
        /// <summary>
        /// Identifies the <seealso cref="IncrementalUpdater"/> dependency property.
        /// </summary>
        static const wil::single_threaded_property<winrt::DependencyProperty> IncrementalUpdaterProperty;

        /// <summary>
        /// Gets or sets the relative priority of this incremental update. Lower Phase values are addressed first.
        /// </summary>
        int Phase() const;
        void Phase(int value);

        static void OnPhaseChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        static winrt::com_ptr<IncrementalUpdater> GetIncrementalUpdater(winrt::DependencyObject const& dependencyObject);

        static void SetIncrementalUpdater(winrt::DependencyObject const& dependencyObject, winrt::com_ptr<IncrementalUpdater> const& value);

        static void OnIncrementalUpdaterChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args);

        void OnAssociatedObjectLoaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        void OnAssociatedObjectUnloaded(winrt::IInspectable const& sender, winrt::RoutedEventArgs const& e);

        winrt::com_ptr<IncrementalUpdater> FindUpdater();

        /// <summary>
        /// Gets the object to which this behavior is attached.
        /// </summary>
        winrt::FrameworkElement AssociatedObject()
        {
            return Behavior::AssociatedObject().try_as<winrt::FrameworkElement>();
        }

        /// <summary>
        /// Called after the behavior is attached to the <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>.
        /// </summary>
        void OnAttached() override;

        /// <summary>
        /// Called when the behavior is being detached from its <see cref="Microsoft.Xaml.Interactivity.Behavior.AssociatedObject"/>.
        /// </summary>
        void OnDetaching() override;

        IncrementalUpdateBehavior() = default;

    private:
        winrt::com_ptr<IncrementalUpdater> _updater = nullptr;
        winrt::FrameworkElement::Loaded_revoker _associatedObjectLoadedRevoker;
        winrt::FrameworkElement::Unloaded_revoker _associatedObjectUnloadedRevoker;
    };
}

namespace winrt::XamlToolkit::WinUI::Interactivity::factory_implementation
{
    struct IncrementalUpdateBehavior : IncrementalUpdateBehaviorT<IncrementalUpdateBehavior, implementation::IncrementalUpdateBehavior>
    {
    };
}
