#pragma once

#include "LayoutTransformControl.g.h"
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include "PropertyChangeEventSource.h"

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt::Microsoft::UI::Xaml::Media;
	using namespace winrt::Windows::Foundation;
	using event_source = winrt::CommunityToolkit::WinUI::Controls::PropertyChangeEventSource<double>;

	struct LayoutTransformControl : LayoutTransformControlT<LayoutTransformControl>
	{
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
		static bool IsSizeSmaller(Size a, Size b);

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
		Matrix GetTransformMatrix(winrt::Microsoft::UI::Xaml::Media::Transform const& transform);

		/// <summary>
		/// Provides the behavior for the "Measure" pass of layout.
		/// </summary>
		/// <param name="availableSize">The available size that this element can
		/// give to child elements. Infinity can be specified as a value to
		/// indicate that the element will size to whatever content is available.</param>
		/// <returns>The size that this element determines it needs during
		/// layout, based on its calculations of child element sizes.</returns>
		Size MeasureOverride(Size availableSize);

		/// <summary>
		/// Provides the behavior for the "Arrange" pass of layout.
		/// </summary>
		/// <param name="finalSize">The final area within the parent that this
		/// element should use to arrange itself and its children.</param>
		/// <returns>The actual size used.</returns>
		Size ArrangeOverride(Size finalSize);

		/// <summary>
		/// Computes the largest usable size after applying the transformation to the specified bounds.
		/// </summary>
		/// <param name="arrangeBounds">The size to arrange within.</param>
		/// <returns>The size required.</returns>
		Size ComputeLargestTransformedSize(Size arrangeBounds) const;

		/// <summary>
		/// Handle changes to the child dependency property.
		/// </summary>
		/// <param name="o">The source of the event.</param>
		/// <param name="e">Information about the event.</param>
		static void ChildChanged(DependencyObject const& o, DependencyPropertyChangedEventArgs const& e)
		{
			winrt::get_self<LayoutTransformControl>(o.as<class_type>())->OnChildChanged(
				winrt::unbox_value<FrameworkElement>(e.NewValue()));
		}
		/// <summary>
	/// Gets or sets the single child of the LayoutTransformControl.
	/// </summary>
	/// <remarks>
	/// Corresponds to WPF's Decorator.Child
	/// property.
	/// </remarks>
		FrameworkElement Child() { return winrt::unbox_value<FrameworkElement>(GetValue(ChildProperty)); }
		void Child(winrt::Windows::Foundation::IInspectable const& value) { return SetValue(ChildProperty, value); }

		/// <summary>
		/// Identifies the ChildProperty.
		/// </summary>
		static inline const wil::single_threaded_property<DependencyProperty> ChildProperty = DependencyProperty::Register(
			L"Child",
			winrt::xaml_typename<FrameworkElement>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, ChildChanged));


		/// <summary>
		/// Handles changes to the Transform DependencyProperty.
		/// </summary>
		/// <param name="o">The source of the event.</param>
		/// <param name="e">Information about the event.</param>
		static void TransformChanged(DependencyObject const& o, DependencyPropertyChangedEventArgs const& e)
		{
			winrt::get_self<LayoutTransformControl>(o.as<class_type>())->OnTransformChanged(
				e.OldValue().try_as<winrt::Microsoft::UI::Xaml::Media::Transform>(),
				winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Transform>(e.NewValue()));
		}
		/// <summary>
		/// Gets or sets the Transform of the LayoutTransformControl.
		/// </summary>
		/// <remarks>
		/// Corresponds to UIElement.RenderTransform.
		/// </remarks>
		winrt::Microsoft::UI::Xaml::Media::Transform Transform()
		{
			return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Media::Transform>(GetValue(TransformProperty));
		}
		void Transform(winrt::Windows::Foundation::IInspectable const& value)
		{
			return SetValue(TransformProperty, value);
		}

		/// <summary>
		/// Identifies the TransformProperty dependency property.
		/// </summary>
		static inline const wil::single_threaded_property<DependencyProperty> TransformProperty = DependencyProperty::Register(
			L"Transform",
			winrt::xaml_typename<FrameworkElement>(),
			winrt::xaml_typename<class_type>(),
			PropertyMetadata(nullptr, TransformChanged));

	private:

		/// <summary>
		/// Updates content when the child property is changed.
		/// </summary>
		/// <param name="newContent">The new child.</param>
		void OnChildChanged(FrameworkElement const& newContent)
		{
			if (_layoutRoot != nullptr)
			{
				// Clear current child
				_layoutRoot.Children().Clear();
				if (newContent != nullptr)
				{
					// Add the new child to the tree
					_layoutRoot.Children().Append(newContent);
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
		void OnTransformChanged(winrt::Microsoft::UI::Xaml::Media::Transform const& oldValue, winrt::Microsoft::UI::Xaml::Media::Transform const& newValue);

		void UnsubscribeFromTransformPropertyChanges(winrt::Microsoft::UI::Xaml::Media::Transform const& transform);

		void SubscribeToTransformPropertyChanges(winrt::Microsoft::UI::Xaml::Media::Transform const& transform);

		/// <summary>
		/// Called when a property of a Transform changes.
		/// </summary>
		void OnTransformPropertyChanged(winrt::Windows::Foundation::IInspectable const& sender, double e);

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
		std::unordered_map<
			winrt::Microsoft::UI::Xaml::Media::Transform,
			std::vector<std::unique_ptr<event_source>>>
			_transformPropertyChangeEventSources;

		/// <summary>
		/// Host panel for Child element.
		/// </summary>
		winrt::Microsoft::UI::Xaml::Controls::Panel _layoutRoot{ nullptr };

		/// <summary>
		/// RenderTransform/MatrixTransform applied to layout root.
		/// </summary>
		winrt::Microsoft::UI::Xaml::Media::MatrixTransform _matrixTransform;

		/// <summary>
		/// Transformation matrix corresponding to matrix transform.
		/// </summary>
		winrt::Microsoft::UI::Xaml::Media::Matrix _transformation;

		/// <summary>
		/// Actual DesiredSize of Child element.
		/// </summary>
		Size _childActualSize { 0, 0 };
	};
}

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct LayoutTransformControl : LayoutTransformControlT<LayoutTransformControl, implementation::LayoutTransformControl>
	{
	};
}
