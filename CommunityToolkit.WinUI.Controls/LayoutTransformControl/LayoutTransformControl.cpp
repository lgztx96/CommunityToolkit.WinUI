#include "pch.h"
#include "LayoutTransformControl.h"
#if __has_include("LayoutTransformControl.g.cpp")
#include "LayoutTransformControl.g.cpp"
#endif
#include <winrt/CommunityToolkit.WinUI.h>
using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace CommunityToolkit::WinUI;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	/// <summary>
	/// Initializes a instance of the <see cref="LayoutTransformControl"/> class.
	/// </summary>
	LayoutTransformControl::LayoutTransformControl()
	{
		DefaultStyleKey(winrt::box_value(winrt::xaml_typename<class_type>()));

		// Can't tab to LayoutTransformControl
		IsTabStop(false);

		// Disable layout rounding because its rounding of values confuses things.
		UseLayoutRounding(false);
	}

	/// <summary>
	/// Called whenever the control's template changes.
	/// </summary>
	void LayoutTransformControl::OnApplyTemplate()
	{
		// Save existing content and remove it from the visual tree
		FrameworkElement savedContent = Child();
		Child(nullptr);

		// Apply template
		base_type::OnApplyTemplate();

		// Find template parts
		if (auto panel = GetTemplateChild(L"LayoutRoot").try_as<Panel>())
		{
			_layoutRoot = panel;
		}

		if (auto matrixTransform = GetTemplateChild(L"MatrixTransform").try_as<MatrixTransform>())
		{
			_matrixTransform = matrixTransform;
		}

		// RestoreAsync saved content
		Child(savedContent);

		// Apply the current transform
		TransformUpdated();
	}

	/// <summary>
	/// Notifies the LayoutTransformControl that some aspect of its Transform property has changed.
	/// </summary>
	/// <remarks>
	/// Call this to update the LayoutTransform in cases where
	/// LayoutTransformControl wouldn't otherwise know to do so.
	/// </remarks>
	void LayoutTransformControl::TransformUpdated()
	{
		ProcessTransform();
	}

	/// <summary>
	/// Return true if Size a is smaller than Size b in either dimension.
	/// </summary>
	/// <param name="a">The left size.</param>
	/// <param name="b">The right size.</param>
	/// <returns>A value indicating whether the left size is smaller than
	/// the right.</returns>
	bool LayoutTransformControl::IsSizeSmaller(Size a, Size b)
	{
		// WPF equivalent of following code:
		// return ((a.Width < b.Width) || (a.Height < b.Height));
		return (a.Width + AcceptableDelta < b.Width) || (a.Height + AcceptableDelta < b.Height);
	}

	/// <summary>
	/// Processes the current transform to determine the corresponding
	/// matrix.
	/// </summary>
	void LayoutTransformControl::ProcessTransform()
	{
		// Get the transform matrix and apply it
		_transformation = MatrixExtensions::Round(GetTransformMatrix(Transform()), DecimalsAfterRound);

		if (_matrixTransform)
		{
			_matrixTransform.Matrix(_transformation);
		}

		// transform means re-layout is necessary
		InvalidateMeasure();
	}

	/// <summary>
	/// Walks the Transform and returns the corresponding matrix.
	/// </summary>
	/// <param name="transform">The transform to create a matrix for.
	/// </param>
	/// <returns>The matrix calculated from the transform.</returns>
	Matrix LayoutTransformControl::GetTransformMatrix(winrt::Microsoft::UI::Xaml::Media::Transform const& transform)
	{
		if (transform)
		{
			// WPF equivalent of this entire method (why oh why only WPF...):
			// return transform.Value;

			// Process the TransformGroup

			if (auto transformGroup = transform.try_as<TransformGroup>())
			{
				auto groupMatrix = winrt::Microsoft::UI::Xaml::Media::MatrixHelper::Identity();

				for (auto child : transformGroup.Children())
				{
					groupMatrix = MatrixExtensions::Multiply(groupMatrix, GetTransformMatrix(child));
				}

				return groupMatrix;
			}

			// Process the RotateTransform
			if (auto rotateTransform = transform.try_as<RotateTransform>())
			{
				return TransformExtensions::GetMatrix(rotateTransform);
			}

			// Process the ScaleTransform

			if (auto scaleTransform = transform.try_as<ScaleTransform>())
			{
				return TransformExtensions::GetMatrix(scaleTransform);
			}

			// Process the SkewTransform
			if (auto skewTransform = transform.try_as<SkewTransform>())
			{
				return TransformExtensions::GetMatrix(skewTransform);
			}

			// Process the MatrixTransform
			if (auto matrixTransform = transform.try_as<MatrixTransform>())
			{
				return matrixTransform.Matrix();
			}

			if (transform.try_as<CompositeTransform>())
			{
				throw winrt::hresult_not_implemented(L"CompositeTransforms are not supported (yet) by the LayoutTransformControl.");
			}

			// TranslateTransform has no effect in LayoutTransform
		}

		return winrt::Microsoft::UI::Xaml::Media::MatrixHelper::Identity();
	}

	/// <summary>
	/// Provides the behavior for the "Measure" pass of layout.
	/// </summary>
	/// <param name="availableSize">The available size that this element can
	/// give to child elements. Infinity can be specified as a value to
	/// indicate that the element will size to whatever content is available.</param>
	/// <returns>The size that this element determines it needs during
	/// layout, based on its calculations of child element sizes.</returns>
	Size LayoutTransformControl::MeasureOverride(Size availableSize)
	{
		FrameworkElement child = Child();
		if (_layoutRoot == nullptr || child == nullptr)
		{
			// No content, no size
			return { 0, 0 };
		}

		Size measureSize;
		if (SizeHelper::Equals(_childActualSize, Size{ 0, 0 }))
		{
			// Determine the largest size after the transformation
			measureSize = ComputeLargestTransformedSize(availableSize);
		}
		else
		{
			// Previous measure/arrange pass determined that Child.DesiredSize was larger than believed.
			measureSize = _childActualSize;
		}

		// Perform a measure on the _layoutRoot (containing Child)
		_layoutRoot.Measure(measureSize);

		// Transform DesiredSize to find its width/height
		Rect startingRect = Rect(0, 0, _layoutRoot.DesiredSize().Width, _layoutRoot.DesiredSize().Height);
		Rect transformedDesiredRect = RectExtensions::Transform(startingRect, _transformation);
		Size transformedDesiredSize = Size(transformedDesiredRect.Width, transformedDesiredRect.Height);

		// Return result to allocate enough space for the transformation
		return transformedDesiredSize;
	}

	/// <summary>
	/// Provides the behavior for the "Arrange" pass of layout.
	/// </summary>
	/// <param name="finalSize">The final area within the parent that this
	/// element should use to arrange itself and its children.</param>
	/// <returns>The actual size used.</returns>
	Size LayoutTransformControl::ArrangeOverride(Size finalSize)
	{
		FrameworkElement child = Child();
		if (_layoutRoot == nullptr || child == nullptr)
		{
			// No child, use whatever was given
			return finalSize;
		}

		// Determine the largest available size after the transformation
		Size finalSizeTransformed = ComputeLargestTransformedSize(finalSize);
		if (IsSizeSmaller(finalSizeTransformed, _layoutRoot.DesiredSize()))
		{
			// Some elements do not like being given less space than they asked for (ex: TextBlock)
			// Bump the working size up to do the right thing by them
			finalSizeTransformed = _layoutRoot.DesiredSize();
		}

		// Transform the working size to find its width/height
		Rect startingRect = Rect(0, 0, finalSizeTransformed.Width, finalSizeTransformed.Height);
		Rect transformedRect = RectExtensions::Transform(startingRect, _transformation);

		// Create the Arrange rect to center the transformed content
		Rect finalRect = Rect(
			-RectHelper::GetLeft(transformedRect) + ((finalSize.Width - transformedRect.Width) / 2),
			-RectHelper::GetTop(transformedRect) + ((finalSize.Height - transformedRect.Height) / 2),
			finalSizeTransformed.Width,
			finalSizeTransformed.Height);

		// Perform an Arrange on _layoutRoot (containing Child)
		_layoutRoot.Arrange(finalRect);

		// This is the first opportunity to find out the Child's true DesiredSize
		if (IsSizeSmaller(finalSizeTransformed, child.RenderSize()) && (SizeHelper::Equals(_childActualSize, Size{ 0, 0 })))
		{
			// Unfortunately, all the work so far is invalid because the wrong DesiredSize was used
			// Make a note of the actual DesiredSize
			_childActualSize = Size(static_cast<float>(child.ActualWidth()), static_cast<float>(child.ActualHeight()));

			// Force a measure/arrange pass
			InvalidateMeasure();
		}
		else
		{
			// Clear the "need to measure/arrange again" flag
			_childActualSize = Size{ 0, 0 };
		}

		// Return result to perform the transformation
		return finalSize;
	}

	/// <summary>
	/// Computes the largest usable size after applying the transformation to the specified bounds.
	/// </summary>
	/// <param name="arrangeBounds">The size to arrange within.</param>
	/// <returns>The size required.</returns>
	Size LayoutTransformControl::ComputeLargestTransformedSize(Size arrangeBounds) const
	{
		// Computed largest transformed size
		Size computedSize{ 0, 0 };

		// Detect infinite bounds and constrain the scenario
		bool infiniteWidth = std::isinf(arrangeBounds.Width);

		if (infiniteWidth)
		{
			arrangeBounds.Width = arrangeBounds.Height;
		}

		bool infiniteHeight = std::isinf(arrangeBounds.Height);

		if (infiniteHeight)
		{
			arrangeBounds.Height = arrangeBounds.Width;
		}

		// Capture the matrix parameters
		double a = _transformation.M11;
		double b = _transformation.M12;
		double c = _transformation.M21;
		double d = _transformation.M22;

		// Compute maximum possible transformed width/height based on starting width/height
		// These constraints define two lines in the positive x/y quadrant
		double maxWidthFromWidth = std::abs(arrangeBounds.Width / a);
		double maxHeightFromWidth = std::abs(arrangeBounds.Width / c);
		double maxWidthFromHeight = std::abs(arrangeBounds.Height / b);
		double maxHeightFromHeight = std::abs(arrangeBounds.Height / d);

		// The transformed width/height that maximize the area under each segment is its midpoint
		// At most one of the two midpoints will satisfy both constraints
		double idealWidthFromWidth = maxWidthFromWidth / 2;
		double idealHeightFromWidth = maxHeightFromWidth / 2;
		double idealWidthFromHeight = maxWidthFromHeight / 2;
		double idealHeightFromHeight = maxHeightFromHeight / 2;

		// Compute slope of both constraint lines
		double slopeFromWidth = -(maxHeightFromWidth / maxWidthFromWidth);
		double slopeFromHeight = -(maxHeightFromHeight / maxWidthFromHeight);

		if (arrangeBounds.Width == 0 || arrangeBounds.Height == 0)
		{
			// Check for empty bounds
			computedSize = Size(0, 0);
		}
		else if (infiniteWidth && infiniteHeight)
		{
			// Check for completely unbound scenario
			computedSize = Size(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
		}
		else if (!MatrixExtensions::HasInverse(_transformation))
		{
			// Check for singular matrix
			computedSize = Size(0, 0);
		}
		else if (b == 0 || c == 0)
		{
			// Check for 0/180 degree special cases
			double maxHeight = infiniteHeight ? std::numeric_limits<double>::infinity() : maxHeightFromHeight;
			double maxWidth = infiniteWidth ? std::numeric_limits<double>::infinity() : maxWidthFromWidth;

			if (b == 0 && c == 0)
			{
				// No constraints
				computedSize = Size(static_cast<float>(maxWidth), static_cast<float>(maxHeight));
			}
			else if (b == 0)
			{
				// Constrained by width
				double computedHeight = std::min<double>(idealHeightFromWidth, maxHeight);
				computedSize = Size(
					static_cast<float>(maxWidth - std::abs((c * computedHeight) / a)),
					static_cast<float>(computedHeight));
			}
			else if (c == 0)
			{
				// Constrained by height
				double computedWidth = std::min<double>(idealWidthFromHeight, maxWidth);
				computedSize = Size(
					static_cast<float>(computedWidth),
					static_cast<float>(maxHeight - std::abs((b * computedWidth) / d)));
			}
		}
		else if (a == 0 || d == 0)
		{
			// Check for 90/270 degree special cases
			double maxWidth = infiniteHeight ? std::numeric_limits<double>::infinity() : maxWidthFromHeight;
			double maxHeight = infiniteWidth ? std::numeric_limits<double>::infinity() : maxHeightFromWidth;

			if (a == 0 && d == 0)
			{
				// No constraints
				computedSize = Size(static_cast<float>(maxWidth), static_cast<float>(maxHeight));
			}
			else if (a == 0)
			{
				// Constrained by width
				double computedHeight = std::min<double>(idealHeightFromHeight, maxHeight);
				computedSize = Size(
					static_cast<float>(maxWidth - std::abs((d * computedHeight) / b)),
					static_cast<float>(computedHeight));
			}
			else if (d == 0)
			{
				// Constrained by height.
				double computedWidth = std::min<double>(idealWidthFromWidth, maxWidth);
				computedSize = Size(
					static_cast<float>(computedWidth),
					static_cast<float>(maxHeight - std::abs((a * computedWidth) / c)));
			}
		}
		else if (idealHeightFromWidth <= ((slopeFromHeight * idealWidthFromWidth) + maxHeightFromHeight))
		{
			// Check the width midpoint for viability (by being below the height constraint line).
			computedSize = Size(static_cast<float>(idealWidthFromWidth), static_cast<float>(idealHeightFromWidth));
		}
		else if (idealHeightFromHeight <= ((slopeFromWidth * idealWidthFromHeight) + maxHeightFromWidth))
		{
			// Check the height midpoint for viability (by being below the width constraint line).
			computedSize = Size(static_cast<float>(idealWidthFromHeight), static_cast<float>(idealHeightFromHeight));
		}
		else
		{
			// Neither midpoint is viable; use the intersection of the two constraint lines instead.

			// Compute width by setting heights equal (m1*x+c1=m2*x+c2).
			double computedWidth = (maxHeightFromHeight - maxHeightFromWidth) / (slopeFromWidth - slopeFromHeight);

			// Compute height from width constraint line (y=m*x+c; using height would give same result).
			computedSize = Size(
				static_cast<float>(computedWidth),
				static_cast<float>((slopeFromWidth * computedWidth) + maxHeightFromWidth));
		}

		return computedSize;
	}

	/// <summary>
	/// Processes the transform when the transform is changed.
	/// </summary>
	/// <param name="oldValue">The old transform</param>
	/// <param name="newValue">The transform to process.</param>
	void LayoutTransformControl::OnTransformChanged(winrt::Microsoft::UI::Xaml::Media::Transform const& oldValue, winrt::Microsoft::UI::Xaml::Media::Transform const& newValue)
	{
		if (oldValue)
		{
			UnsubscribeFromTransformPropertyChanges(oldValue);
		}

		if (newValue)
		{
			SubscribeToTransformPropertyChanges(newValue);
		}

		ProcessTransform();
	}

	void LayoutTransformControl::UnsubscribeFromTransformPropertyChanges(winrt::Microsoft::UI::Xaml::Media::Transform const& transform)
	{
		const auto& propertyChangeEventSources =
			_transformPropertyChangeEventSources[transform];

		for (auto& propertyChangeEventSource : propertyChangeEventSources)
		{
			propertyChangeEventSource->ValueChanged.clear(); // -= OnTransformPropertyChanged;
			propertyChangeEventSource->Unregister();
		}

		_transformPropertyChangeEventSources.erase(transform);
	}

	void LayoutTransformControl::SubscribeToTransformPropertyChanges(winrt::Microsoft::UI::Xaml::Media::Transform const& transform)
	{
		if (auto transformGroup = transform.try_as<TransformGroup>())
		{
			for (const auto& childTransform : transformGroup.Children())
			{
				SubscribeToTransformPropertyChanges(childTransform);
			}

			return;
		}

		auto propertyChangeEventSources = std::vector<std::unique_ptr<event_source>>();
		_transformPropertyChangeEventSources.emplace(transform, std::move(propertyChangeEventSources));

		if (auto rotateTransform = transform.try_as<RotateTransform>())
		{
			auto anglePropertyChangeEventSource = std::make_unique<event_source>(rotateTransform, RotateTransform::AngleProperty());
			anglePropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(anglePropertyChangeEventSource));
			return;
		}

		if (auto scaleTransform = transform.try_as<ScaleTransform>())
		{
			auto scaleXPropertyChangeEventSource = std::make_unique<event_source>(scaleTransform, ScaleTransform::ScaleXProperty());
			scaleXPropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(scaleXPropertyChangeEventSource));

			auto scaleYPropertyChangeEventSource = std::make_unique<event_source>(scaleTransform, ScaleTransform::ScaleYProperty());
			scaleYPropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(scaleYPropertyChangeEventSource));
			return;
		}

		if (auto skewTransform = transform.try_as<SkewTransform>())
		{
			auto angleXPropertyChangeEventSource = std::make_unique<event_source>(skewTransform, SkewTransform::AngleXProperty());
			angleXPropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(angleXPropertyChangeEventSource));

			auto angleYPropertyChangeEventSource = std::make_unique<event_source>(skewTransform, SkewTransform::AngleYProperty());
			angleYPropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(angleYPropertyChangeEventSource));
			return;
		}

		if (auto matrixTransform = transform.try_as<MatrixTransform>())
		{
			auto matrixPropertyChangeEventSource = std::make_unique<event_source>(matrixTransform, MatrixTransform::MatrixProperty());
			matrixPropertyChangeEventSource->ValueChanged.add({ get_weak(), &LayoutTransformControl::OnTransformPropertyChanged });
			propertyChangeEventSources.emplace_back(std::move(matrixPropertyChangeEventSource));
		}
	}

	/// <summary>
	/// Called when a property of a Transform changes.
	/// </summary>
	void LayoutTransformControl::OnTransformPropertyChanged([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] double e)
	{
		TransformUpdated();
	}
}
