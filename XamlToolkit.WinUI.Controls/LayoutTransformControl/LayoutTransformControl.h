#pragma once

#include "LayoutTransformControl.g.h"
#include "PropertyChangeEventSource.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Media;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	using event_source = winrt::XamlToolkit::WinUI::Controls::PropertyChangeEventSource<double>;

	struct event_source_registration
	{
	private:
		event_source event_source;
		std::optional<winrt::event_token> token;
	public:
		event_source_registration(const event_source_registration&) = delete;
		event_source_registration& operator=(const event_source_registration&) = delete;
		event_source_registration(const event_source_registration&&) = delete;
		event_source_registration& operator=(const event_source_registration&&) = delete;

		event_source_registration(winrt::DependencyObject const& source, winrt::DependencyProperty const& property)
			: event_source(source, property) { }

		void subscribe(winrt::EventHandler<double> const& handler)
		{
			unsubscribe();
			token = event_source.ValueChanged(handler);
		}

		void unsubscribe()
		{
			if (token)
			{
				event_source.ValueChanged(*token);
				token.reset();
			}
		}

		~event_source_registration()
		{
			unsubscribe();
		}
	};

	struct LayoutTransformControl : LayoutTransformControlT<LayoutTransformControl>
	{
		static constexpr auto LayoutRootPartName = L"LayoutRoot";
		static constexpr auto MatrixTransformPartName = L"MatrixTransform";

		LayoutTransformControl();

		/// <summary>
		/// Called whenever the control's template changes.
		/// </summary>
		void OnApplyTemplate();

		/// <summary>
		/// Notifies the LayoutTransformControl that some aspect of its Transform property has changed.
		/// </summary>
		/// <remarks>
		/// Call this to update the LayoutTransform in cases where
		/// LayoutTransformControl wouldn't otherwise know to do so.
		/// </remarks>
		void TransformUpdated();

		/// <summary>
		/// Return true if Size a is smaller than Size b in either dimension.
		/// </summary>
		/// <param name="a">The left size.</param>
		/// <param name="b">The right size.</param>
		/// <returns>A value indicating whether the left size is smaller than
		/// the right.</returns>
		static bool IsSizeSmaller(winrt::Size a, winrt::Size b);

		/// <summary>
		/// Processes the current transform to determine the corresponding
		/// matrix.
		/// </summary>
		void ProcessTransform();

		/// <summary>
		/// Walks the Transform and returns the corresponding matrix.
		/// </summary>
		/// <param name="transform">The transform to create a matrix for.
		/// </param>
		/// <returns>The matrix calculated from the transform.</returns>
		winrt::Matrix GetTransformMatrix(winrt::Transform const& transform);

		/// <summary>
		/// Provides the behavior for the "Measure" pass of layout.
		/// </summary>
		/// <param name="availableSize">The available size that this element can
		/// give to child elements. Infinity can be specified as a value to
		/// indicate that the element will size to whatever content is available.</param>
		/// <returns>The size that this element determines it needs during
		/// layout, based on its calculations of child element sizes.</returns>
		winrt::Size MeasureOverride(winrt::Size availableSize);

		/// <summary>
		/// Provides the behavior for the "Arrange" pass of layout.
		/// </summary>
		/// <param name="finalSize">The final area within the parent that this
		/// element should use to arrange itself and its children.</param>
		/// <returns>The actual size used.</returns>
		winrt::Size ArrangeOverride(winrt::Size finalSize);

		/// <summary>
		/// Computes the largest usable size after applying the transformation to the specified bounds.
		/// </summary>
		/// <param name="arrangeBounds">The size to arrange within.</param>
		/// <returns>The size required.</returns>
		winrt::Size ComputeLargestTransformedSize(winrt::Size arrangeBounds) const;

		/// <summary>
		/// Handle changes to the child dependency property.
		/// </summary>
		/// <param name="o">The source of the event.</param>
		/// <param name="e">Information about the event.</param>
		static void ChildChanged(winrt::DependencyObject const& o, winrt::DependencyPropertyChangedEventArgs const& e)
		{
			winrt::get_self<LayoutTransformControl>(o.as<class_type>())->OnChildChanged(
				e.NewValue().try_as<winrt::FrameworkElement>());
		}
		/// <summary>
		/// Gets or sets the single child of the LayoutTransformControl.
		/// </summary>
		/// <remarks>
		/// Corresponds to WPF's Decorator.Child
		/// property.
		/// </remarks>
		winrt::FrameworkElement Child() const { return GetValue(ChildProperty()).try_as<winrt::FrameworkElement>(); }
		void Child(winrt::IInspectable const& value) { return SetValue(ChildProperty(), value); }

		/// <summary>
		/// Identifies the ChildProperty.
		/// </summary>
		static inline const wil::single_threaded_property<winrt::DependencyProperty> ChildProperty = 
			winrt::DependencyProperty::Register(
				L"Child",
				winrt::xaml_typename<winrt::FrameworkElement>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr, &LayoutTransformControl::ChildChanged));

		/// <summary>
		/// Handles changes to the Transform DependencyProperty.
		/// </summary>
		/// <param name="o">The source of the event.</param>
		/// <param name="e">Information about the event.</param>
		static void TransformChanged(winrt::DependencyObject const& o, winrt::DependencyPropertyChangedEventArgs const& e)
		{
			winrt::get_self<LayoutTransformControl>(o.as<class_type>())->OnTransformChanged(
				e.OldValue().try_as<winrt::Transform>(),
				e.NewValue().try_as<winrt::Transform>());
		}
		/// <summary>
		/// Gets or sets the Transform of the LayoutTransformControl.
		/// </summary>
		/// <remarks>
		/// Corresponds to UIElement.RenderTransform.
		/// </remarks>
		winrt::Transform Transform() const
		{
			return GetValue(TransformProperty()).try_as<winrt::Transform>();
		}
		void Transform(winrt::Windows::Foundation::IInspectable const& value)
		{
			return SetValue(TransformProperty(), value);
		}

		/// <summary>
		/// Identifies the TransformProperty dependency property.
		/// </summary>
		static inline const wil::single_threaded_property<winrt::DependencyProperty> TransformProperty =
			winrt::DependencyProperty::Register(
				L"Transform",
				winrt::xaml_typename<winrt::FrameworkElement>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(nullptr, &LayoutTransformControl::TransformChanged));

	private:
		/// <summary>
		/// Updates content when the child property is changed.
		/// </summary>
		/// <param name="newContent">The new child.</param>
		void OnChildChanged(winrt::FrameworkElement const& newContent)
		{
			if (_layoutRoot)
			{
				// Clear current child
				auto child = _layoutRoot.Children();
				child.Clear();
				if (newContent)
				{
					// Add the new child to the tree
					child.Append(newContent);
				}

				// New child means re-layout is necessary
				InvalidateMeasure();
			}
		}

		/// <summary>
		/// Processes the transform when the transform is changed.
		/// </summary>
		/// <param name="oldValue">The old transform</param>
		/// <param name="newValue">The transform to process.</param>
		void OnTransformChanged(winrt::Transform const& oldValue, winrt::Transform const& newValue);

		void UnsubscribeFromTransformPropertyChanges(winrt::Transform const& transform);

		void SubscribeToTransformPropertyChanges(winrt::Transform const& transform);

		/// <summary>
		/// Called when a property of a Transform changes.
		/// </summary>
		void OnTransformPropertyChanged(winrt::IInspectable const& sender, double e);

		/// <summary>
		/// Value used to work around double arithmetic rounding issues.
		/// </summary>
		static constexpr double AcceptableDelta = 0.0001;

		/// <summary>
		/// Value used to work around double arithmetic rounding issues.
		/// </summary>
		static constexpr int DecimalsAfterRound = 4;

		/// <summary>
		/// List of property change event sources for events when properties of the Transform tree change
		/// </summary>
		std::map<winrt::Transform, std::vector<std::unique_ptr<event_source_registration>>> _transformPropertyChangeEventSources;

		/// <summary>
		/// Host panel for Child element.
		/// </summary>
		winrt::Panel _layoutRoot{ nullptr };

		/// <summary>
		/// RenderTransform/MatrixTransform applied to layout root.
		/// </summary>
		winrt::MatrixTransform _matrixTransform;

		/// <summary>
		/// Transformation matrix corresponding to matrix transform.
		/// </summary>
		winrt::Matrix _transformation;

		/// <summary>
		/// Actual DesiredSize of Child element.
		/// </summary>
		winrt::Size _childActualSize{ 0, 0 };
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct LayoutTransformControl : LayoutTransformControlT<LayoutTransformControl, implementation::LayoutTransformControl>
	{
	};
}
