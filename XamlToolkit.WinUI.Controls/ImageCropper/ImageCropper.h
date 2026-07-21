#pragma once

#include "ImageCropper.g.h"
#include "ImageCropperThumb.h"

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <string_view>
#include <vector>
#else
import winrt.Microsoft.Graphics.Canvas;
import winrt.Microsoft.Graphics.Canvas.Geometry;
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Windows::Storage;
	using namespace Windows::Storage::Streams;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;
	using namespace Microsoft::UI::Xaml::Shapes;
	using namespace Microsoft::UI::Xaml::Media;
	using namespace Microsoft::UI::Xaml::Media::Imaging;
	using namespace Microsoft::UI::Xaml::Media::Animation;
	using namespace XamlToolkit::WinUI::Controls;
	using namespace Microsoft::Graphics::Canvas;
	using namespace Microsoft::Graphics::Canvas::Geometry;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	using namespace std::chrono_literals;

	struct ImageCropper : ImageCropperT<ImageCropper>
	{
		static constexpr std::wstring_view LayoutGridName = L"PART_LayoutGrid";

		static constexpr std::wstring_view ImageCanvasPartName = L"PART_ImageCanvas";

		static constexpr std::wstring_view SourceImagePartName = L"PART_SourceImage";

		static constexpr std::wstring_view MaskAreaPathPartName = L"PART_MaskAreaPath";

		static constexpr std::wstring_view OverlayAreaPathPartName = L"PART_OverlayAreaPath";

		static constexpr std::wstring_view TopThumbPartName = L"PART_TopThumb";

		static constexpr std::wstring_view BottomThumbPartName = L"PART_BottomThumb";

		static constexpr std::wstring_view LeftThumbPartName = L"PART_LeftThumb";

		static constexpr std::wstring_view RightThumbPartName = L"PART_RightThumb";

		static constexpr std::wstring_view UpperLeftThumbPartName = L"PART_UpperLeftThumb";

		static constexpr std::wstring_view UpperRightThumbPartName = L"PART_UpperRightThumb";

		static constexpr std::wstring_view LowerLeftThumbPartName = L"PART_LowerLeftThumb";

		static constexpr std::wstring_view LowerRightThumbPartName = L"PART_LowerRightThumb";

		static winrt::Rect ToRect(winrt::Size size)
		{
			return winrt::Rect(0, 0, size.Width, size.Height);
		}

		static winrt::Rect ToRect(winrt::Point point, winrt::Point end)
		{
			return winrt::RectHelper::FromPoints(point, end);
		}

		ImageCropper();

		void OnApplyTemplate();

		winrt::Size MeasureOverride(winrt::Size availableSize);

		winrt::IAsyncAction LoadImageFromFile(winrt::StorageFile const& imageFile);

		winrt::IAsyncAction SaveAsync(
			winrt::IRandomAccessStream const& stream, 
			winrt::BitmapFileFormat bitmapFileFormat, 
			bool keepRectangularOutput = false);

		void Reset();

		bool TrySetCroppedRegion(Rect rect);

		wil::single_threaded_rw_property<double> MinCroppedPixelLength{ 40 };

		wil::single_threaded_rw_property<double> MinSelectedLength{ 40 };

		wil::single_threaded_property<winrt::Rect> CroppedRegion{ _currentCroppedRect };

		static void OnSourceChanged(winrt::DependencyObject const& d, winrt::DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();
			if (auto bitmap = e.NewValue().try_as<winrt::WriteableBitmap>())
			{
				if (bitmap.PixelWidth() < target->MinCropSize().Width || bitmap.PixelHeight() < target->MinCropSize().Height)
				{
					target->Source(nullptr);
					throw winrt::hresult_invalid_argument(L"The resolution of the image is too small!");
				}
			}

			target->InvalidateMeasure();
			target->UpdateCropShape();
			target->InitImageLayout();
		}

		static void OnAspectRatioChanged(
			winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();

			if (target->TryUpdateAspectRatio())
			{
				if (target->TryUpdateImageLayout(true))
				{
					target->UpdateSelectionThumbs(true);
					target->UpdateMaskArea(true);
				}
			}
		}

		static void OnCropShapeChanged(
			winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();
			target->UpdateCropShape();
			target->UpdateThumbsVisibility();

			if (target->TryUpdateAspectRatio())
			{
				if (target->TryUpdateImageLayout())
				{
					target->UpdateSelectionThumbs();
				}
			}

			target->UpdateMaskArea();
		}

		static void OnThumbPlacementChanged(
			winrt::DependencyObject const& d, [[maybe_unused]] winrt::DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();
			target->UpdateThumbsVisibility();
		}

		winrt::WriteableBitmap Source() const { return GetValue(SourceProperty).try_as<winrt::WriteableBitmap>(); }
		void Source(winrt::WriteableBitmap const& value) { SetValue(SourceProperty, value); }

		winrt::IReference<double> AspectRatio() const { return GetValue(AspectRatioProperty).try_as<winrt::IReference<double>>(); }
		void AspectRatio(IReference<double> value) { SetValue(AspectRatioProperty, value); }

		CropShape CropShape() const { return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::CropShape>(GetValue(CropShapeProperty)); }
		void CropShape(winrt::XamlToolkit::WinUI::Controls::CropShape value) { SetValue(CropShapeProperty, winrt::box_value(value)); }

		winrt::Brush Mask() const { return GetValue(MaskProperty).try_as<winrt::Brush>(); }
		void Mask(Brush const& value) { SetValue(MaskProperty, value); }

		winrt::Brush Overlay() const { return GetValue(OverlayProperty).try_as<winrt::Brush>(); }
		void Overlay(Brush const& value) { SetValue(OverlayProperty, value); }

		winrt::Style PrimaryThumbStyle() const { return GetValue(PrimaryThumbStyleProperty).try_as<winrt::Style>(); }
		void PrimaryThumbStyle(winrt::Style const& value) { SetValue(PrimaryThumbStyleProperty, value); }

		winrt::Style SecondaryThumbStyle() const { return GetValue(SecondaryThumbStyleProperty).try_as<winrt::Style>(); }
		void SecondaryThumbStyle(winrt::Style const& value) { SetValue(SecondaryThumbStyleProperty, value); }

		winrt::XamlToolkit::WinUI::Controls::ThumbPlacement ThumbPlacement() const { return winrt::unbox_value<winrt::XamlToolkit::WinUI::Controls::ThumbPlacement>(GetValue(ThumbPlacementProperty)); }
		void ThumbPlacement(winrt::XamlToolkit::WinUI::Controls::ThumbPlacement value) { SetValue(ThumbPlacementProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<winrt::DependencyProperty> AspectRatioProperty =
			winrt::DependencyProperty::Register(
				L"AspectRatio", 
				winrt::xaml_typename<winrt::IReference<double>>(),
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr, &ImageCropper::OnAspectRatioChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> SourceProperty =
			winrt::DependencyProperty::Register(
				L"Source", 
				winrt::xaml_typename<winrt::WriteableBitmap>(),
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr, &ImageCropper::OnSourceChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> CropShapeProperty =
			winrt::DependencyProperty::Register(
				L"CropShape", 
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::CropShape>(),
				winrt::xaml_typename<class_type>(),
				winrt::PropertyMetadata(winrt::box_value(CropShape::
				Rectangular), OnCropShapeChanged));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> MaskProperty =
			winrt::DependencyProperty::Register(
				L"Mask", 
				winrt::xaml_typename<winrt::Brush>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> OverlayProperty =
			winrt::DependencyProperty::Register(
				L"Overlay", 
				winrt::xaml_typename<winrt::Brush>(),
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> PrimaryThumbStyleProperty =
			winrt::DependencyProperty::Register(
				L"PrimaryThumbStyle", 
				winrt::xaml_typename<winrt::Style>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> SecondaryThumbStyleProperty =
			winrt::DependencyProperty::Register(
				L"SecondaryThumbStyle", 
				winrt::xaml_typename<winrt::Style>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<winrt::DependencyProperty> ThumbPlacementProperty =
			winrt::DependencyProperty::Register(
				L"ThumbPlacement", 
				winrt::xaml_typename<winrt::XamlToolkit::WinUI::Controls::ThumbPlacement>(), 
				winrt::xaml_typename<class_type>(), 
				winrt::PropertyMetadata(winrt::box_value(ThumbPlacement::All), &ImageCropper::OnThumbPlacementChanged));

	private:
		winrt::CompositeTransform _imageTransform;
		winrt::CompositeTransform _inverseImageTransform;
		winrt::GeometryGroup _maskAreaGeometryGroup;

		winrt::Grid _layoutGrid{ nullptr };
		winrt::Canvas _imageCanvas{ nullptr };
		winrt::Image _sourceImage{ nullptr };
		winrt::Path _maskAreaPath{ nullptr };
		winrt::Path _overlayAreaPath{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _topThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _bottomThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _leftThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _rightThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _upperLeftThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _upperRightThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _lowerLeftThumb{ nullptr };
		winrt::XamlToolkit::WinUI::Controls::ImageCropperThumb _lowerRightThumb{ nullptr };

		// Selection area
		float _startX;
		float _startY;
		float _endX;
		float _endY;

		winrt::Rect _currentCroppedRect;
		winrt::Rect _restrictedCropRect;
		winrt::Rect _restrictedSelectRect;
		winrt::RectangleGeometry _outerGeometry;
		winrt::Microsoft::UI::Xaml::Media::Geometry _innerGeometry{ nullptr };
		winrt::Microsoft::UI::Xaml::Media::Geometry _overlayGeometry{ nullptr };
		winrt::TimeSpan _animationDuration = 300ms;

		winrt::Rect CanvasRect()
		{
			auto width = _imageCanvas ? _imageCanvas.ActualWidth() : 0;
			auto height = _imageCanvas ? _imageCanvas.ActualHeight() : 0;
			return winrt::Rect(0, 0, static_cast<float>(width), static_cast<float>(height));
		}

		bool KeepAspectRatio() { return ActualAspectRatio() > 0; }

		double ActualAspectRatio();

		winrt::Size MinCropSize();

		winrt::Size MinSelectSize();

		void HookUpEvents();

		void UnhookEvents();

		void InitImageLayout(bool animate = false);

		bool TryUpdateImageLayout(bool animate = false);

		bool TryUpdateImageLayoutWithViewport(winrt::Rect viewport, winrt::Rect viewportImageRect, bool animate = false);

		void UpdateCroppedRect(ThumbPosition position, winrt::Point diffPos);

		void UpdateSelectionThumbs(bool animate = false);

		void UpdateSelectionThumbs(winrt::Point startPoint, winrt::Point endPoint, bool animate = false);

		void UpdateCropShape();

		void UpdateMaskArea(bool animate = false);

		bool TryUpdateAspectRatio();

		void UpdateThumbsVisibility();

		winrt::Point SelectionAreaCenter();

		static winrt::IAsyncAction CropImageAsync(winrt::WriteableBitmap const& writeableBitmap, winrt::IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat);

		static winrt::IAsyncAction CropImageWithShapeAsync(winrt::WriteableBitmap const& writeableBitmap, winrt::IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat, winrt::XamlToolkit::WinUI::Controls::CropShape cropShape);

		static winrt::CanvasGeometry CreateClipGeometry(winrt::ICanvasResourceCreator resourceCreator, winrt::XamlToolkit::WinUI::Controls::CropShape cropShape, Size croppedSize);

		static winrt::guid GetEncoderId(winrt::BitmapFileFormat bitmapFileFormat);

		static winrt::Point GetSafePoint(winrt::Rect targetRect, winrt::Point point);

		static bool IsSafePoint(winrt::Rect targetRect, winrt::Point point);

		static bool IsSafeRect(winrt::Point startPoint, winrt::Point endPoint, winrt::Size minSize);

		static winrt::Rect GetSafeRect(winrt::Point startPoint, winrt::Point endPoint, winrt::Size minSize, ThumbPosition position);

		static winrt::Rect GetUniformRect(Rect targetRect, double aspectRatio);

		static bool IsValidRect(winrt::Rect targetRect);

		static winrt::Point GetSafeSizeChangeWhenKeepAspectRatio(winrt::Rect targetRect, ThumbPosition thumbPosition, winrt::Rect selectedRect, Point originSizeChange, double aspectRatio);

		static bool CanContains(winrt::Rect targetRect, winrt::Rect testRect);

		static bool TryGetContainedRect(winrt::Rect targetRect, winrt::Rect& testRect);

		static bool IsCornerThumb(ThumbPosition thumbPosition);

		void ImageCropperThumb_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void ImageCropperThumb_KeyUp(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e);

		void ImageCropperThumb_ManipulationCompleted(winrt::IInspectable const& sender, winrt::ManipulationCompletedRoutedEventArgs const& e);

		void ImageCropperThumb_ManipulationDelta(winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e);

		void SourceImage_ManipulationDelta(winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e);

		void ImageCanvas_SizeChanged(winrt::IInspectable const& sender, winrt::SizeChangedEventArgs const& e);

		static void AnimateUIElementOffset(winrt::Point to, winrt::TimeSpan duration, winrt::UIElement const& target);

		static void AnimateUIElementScale(double to, winrt::TimeSpan duration, winrt::UIElement const& target);

		static winrt::DoubleAnimation CreateDoubleAnimation(double to, winrt::TimeSpan duration, winrt::DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation);

		static winrt::PointAnimation CreatePointAnimation(winrt::Point to, winrt::TimeSpan duration, winrt::DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation);

		static winrt::ObjectAnimationUsingKeyFrames CreateRectangleAnimation(winrt::Rect to, winrt::TimeSpan duration, winrt::RectangleGeometry rectangle, bool enableDependentAnimation);

		static std::vector<winrt::DiscreteObjectKeyFrame> GetRectKeyframes(winrt::Rect from, winrt::Rect to, winrt::TimeSpan duration);

		// Top
		winrt::event_token _topThumbManipulationDeltaToken{};
		winrt::event_token _topThumbManipulationCompletedToken{};
		winrt::event_token _topThumbKeyDownToken{};
		winrt::event_token _topThumbKeyUpToken{};

		// Bottom
		winrt::event_token _bottomThumbManipulationDeltaToken{};
		winrt::event_token _bottomThumbManipulationCompletedToken{};
		winrt::event_token _bottomThumbKeyDownToken{};
		winrt::event_token _bottomThumbKeyUpToken{};

		// Left
		winrt::event_token _leftThumbManipulationDeltaToken{};
		winrt::event_token _leftThumbManipulationCompletedToken{};
		winrt::event_token _leftThumbKeyDownToken{};
		winrt::event_token _leftThumbKeyUpToken{};

		// Right
		winrt::event_token _rightThumbManipulationDeltaToken{};
		winrt::event_token _rightThumbManipulationCompletedToken{};
		winrt::event_token _rightThumbKeyDownToken{};
		winrt::event_token _rightThumbKeyUpToken{};

		// UpperLeft
		winrt::event_token _upperLeftThumbManipulationDeltaToken{};
		winrt::event_token _upperLeftThumbManipulationCompletedToken{};
		winrt::event_token _upperLeftThumbKeyDownToken{};
		winrt::event_token _upperLeftThumbKeyUpToken{};

		// UpperRight
		winrt::event_token _upperRightThumbManipulationDeltaToken{};
		winrt::event_token _upperRightThumbManipulationCompletedToken{};
		winrt::event_token _upperRightThumbKeyDownToken{};
		winrt::event_token _upperRightThumbKeyUpToken{};

		// LowerLeft
		winrt::event_token _lowerLeftThumbManipulationDeltaToken{};
		winrt::event_token _lowerLeftThumbManipulationCompletedToken{};
		winrt::event_token _lowerLeftThumbKeyDownToken{};
		winrt::event_token _lowerLeftThumbKeyUpToken{};

		// LowerRight
		winrt::event_token _lowerRightThumbManipulationDeltaToken{};
		winrt::event_token _lowerRightThumbManipulationCompletedToken{};
		winrt::event_token _lowerRightThumbKeyDownToken{};
		winrt::event_token _lowerRightThumbKeyUpToken{};

		winrt::event_token _imageCanvasSizeChangedToken{};
		winrt::event_token _sourceImageManipulationDeltaToken{};
	};
}

namespace winrt::XamlToolkit::WinUI::Controls::factory_implementation
{
	struct ImageCropper : ImageCropperT<ImageCropper, implementation::ImageCropper>
	{
	};
}
