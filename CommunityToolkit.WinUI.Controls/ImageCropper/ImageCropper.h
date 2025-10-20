#pragma once

#include "ImageCropper.g.h"
#include "ImageCropperThumb.h"
#include <wil/wistd_type_traits.h>
#include <wil/cppwinrt_authoring.h>
#include <winrt/Microsoft.Graphics.Canvas.Geometry.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	using namespace winrt;
	using namespace winrt::Microsoft::UI::Xaml;
	using namespace winrt::Microsoft::UI::Xaml::Controls;
	using namespace winrt::Microsoft::UI::Xaml::Shapes;
	using namespace winrt::Microsoft::UI::Xaml::Media;
	using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
	using namespace winrt::Microsoft::UI::Xaml::Media::Animation;
	using namespace winrt::Windows::Storage::Streams;
	using namespace winrt::Windows::Foundation;
	using namespace winrt::Windows::Graphics::Imaging;
	using namespace winrt::CommunityToolkit::WinUI::Controls;
	using namespace Microsoft::Graphics::Canvas;
	using namespace Microsoft::Graphics::Canvas::Geometry;
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

		static Rect ToRect(Size size)
		{
			return Rect(0, 0, size.Width, size.Height);
		}

		static Rect ToRect(Point point, Point end)
		{
			return RectHelper::FromPoints(point, end);
		}

		ImageCropper();

		void OnApplyTemplate();

		Size MeasureOverride(Size availableSize);

		winrt::Windows::Foundation::IAsyncAction LoadImageFromFile(
			winrt::Windows::Storage::StorageFile const& imageFile);

		winrt::Windows::Foundation::IAsyncAction SaveAsync(
			IRandomAccessStream const& stream,
			BitmapFileFormat bitmapFileFormat,
			bool keepRectangularOutput = false);

		void Reset();

		bool TrySetCroppedRegion(Rect rect);

		wil::single_threaded_rw_property<double> MinCroppedPixelLength{ 40 };

		wil::single_threaded_rw_property<double> MinSelectedLength{ 40 };

		wil::single_threaded_property<Rect> CroppedRegion{ _currentCroppedRect };

		static void OnSourceChanged(DependencyObject const& d, DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();
			if (auto bitmap = e.NewValue().try_as<WriteableBitmap>())
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
			DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
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
			DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
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
			DependencyObject const& d, [[maybe_unused]] DependencyPropertyChangedEventArgs const& e)
		{
			auto target = winrt::get_self<ImageCropper>(d.as<class_type>())->get_strong();
			target->UpdateThumbsVisibility();
		}

		WriteableBitmap Source() { return winrt::unbox_value<WriteableBitmap>(GetValue(SourceProperty)); }
		void Source(WriteableBitmap const& value) { SetValue(SourceProperty, value); }

		IReference<double> AspectRatio() { return winrt::unbox_value<IReference<double>>(GetValue(AspectRatioProperty)); }
		void AspectRatio(IReference<double> value) { SetValue(AspectRatioProperty, value); }

		CropShape CropShape() { return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::CropShape>(GetValue(CropShapeProperty)); }
		void CropShape(winrt::CommunityToolkit::WinUI::Controls::CropShape value) { SetValue(CropShapeProperty, winrt::box_value(value)); }

		Brush Mask() { return winrt::unbox_value<Brush>(GetValue(MaskProperty)); }
		void Mask(Brush const& value) { SetValue(MaskProperty, value); }

		Brush Overlay() { return winrt::unbox_value<Brush>(GetValue(OverlayProperty)); }
		void Overlay(Brush const& value) { SetValue(OverlayProperty, value); }

		winrt::Microsoft::UI::Xaml::Style PrimaryThumbStyle() { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Style>(GetValue(PrimaryThumbStyleProperty)); }
		void PrimaryThumbStyle(winrt::Microsoft::UI::Xaml::Style const& value) { SetValue(PrimaryThumbStyleProperty, value); }

		winrt::Microsoft::UI::Xaml::Style SecondaryThumbStyle() { return winrt::unbox_value<winrt::Microsoft::UI::Xaml::Style>(GetValue(SecondaryThumbStyleProperty)); }
		void SecondaryThumbStyle(winrt::Microsoft::UI::Xaml::Style const& value) { SetValue(SecondaryThumbStyleProperty, value); }

		winrt::CommunityToolkit::WinUI::Controls::ThumbPlacement ThumbPlacement() { return winrt::unbox_value<winrt::CommunityToolkit::WinUI::Controls::ThumbPlacement>(GetValue(ThumbPlacementProperty)); }
		void ThumbPlacement(winrt::CommunityToolkit::WinUI::Controls::ThumbPlacement value) { SetValue(ThumbPlacementProperty, winrt::box_value(value)); }

		static inline const wil::single_threaded_property<DependencyProperty> AspectRatioProperty =
			DependencyProperty::Register(L"AspectRatio", winrt::xaml_typename<IReference<double>>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnAspectRatioChanged));

		static inline const wil::single_threaded_property<DependencyProperty> SourceProperty =
			DependencyProperty::Register(L"Source", winrt::xaml_typename<WriteableBitmap>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr, OnSourceChanged));

		static inline const wil::single_threaded_property<DependencyProperty> CropShapeProperty =
			DependencyProperty::Register(L"CropShape", winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::CropShape>(), winrt::xaml_typename<class_type>(), PropertyMetadata(winrt::box_value(CropShape::
				Rectangular), OnCropShapeChanged));

		static inline const wil::single_threaded_property<DependencyProperty> MaskProperty =
			DependencyProperty::Register(L"Mask", winrt::xaml_typename<Brush>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<DependencyProperty> OverlayProperty =
			DependencyProperty::Register(L"Overlay", winrt::xaml_typename<Brush>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<DependencyProperty> PrimaryThumbStyleProperty =
			DependencyProperty::Register(L"PrimaryThumbStyle", winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<DependencyProperty> SecondaryThumbStyleProperty =
			DependencyProperty::Register(L"SecondaryThumbStyle", winrt::xaml_typename<winrt::Microsoft::UI::Xaml::Style>(), winrt::xaml_typename<class_type>(), PropertyMetadata(nullptr));

		static inline const wil::single_threaded_property<DependencyProperty> ThumbPlacementProperty =
			DependencyProperty::Register(L"ThumbPlacement", winrt::xaml_typename<winrt::CommunityToolkit::WinUI::Controls::ThumbPlacement>(), winrt::xaml_typename<class_type>(), PropertyMetadata({ winrt::box_value(ThumbPlacement::All), &ImageCropper::OnThumbPlacementChanged
				}));

	private:
		CompositeTransform _imageTransform;
		CompositeTransform _inverseImageTransform;
		GeometryGroup _maskAreaGeometryGroup;

		Grid _layoutGrid{ nullptr };
		Canvas _imageCanvas{ nullptr };
		Image _sourceImage{ nullptr };
		Path _maskAreaPath{ nullptr };
		Path _overlayAreaPath{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _topThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _bottomThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _leftThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _rightThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _upperLeftThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _upperRightThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _lowerLeftThumb{ nullptr };
		winrt::CommunityToolkit::WinUI::Controls::ImageCropperThumb _lowerRightThumb{ nullptr };

		// Selection area
		float _startX;
		float _startY;
		float _endX;
		float _endY;

		Rect _currentCroppedRect;
		Rect _restrictedCropRect;
		Rect _restrictedSelectRect;
		RectangleGeometry _outerGeometry;
		Media::Geometry _innerGeometry{ nullptr };
		Media::Geometry _overlayGeometry{ nullptr };
		TimeSpan _animationDuration = 300ms;

		Rect CanvasRect()
		{
			auto width = _imageCanvas ? _imageCanvas.ActualWidth() : 0;
			auto height = _imageCanvas ? _imageCanvas.ActualHeight() : 0;
			return Rect(0, 0, static_cast<float>(width), static_cast<float>(height));
		}

		bool KeepAspectRatio() { return ActualAspectRatio() > 0; }

		double ActualAspectRatio();

		Size MinCropSize();

		Size MinSelectSize();

		void HookUpEvents();

		void UnhookEvents();

		void InitImageLayout(bool animate = false);

		bool TryUpdateImageLayout(bool animate = false);

		bool TryUpdateImageLayoutWithViewport(Rect viewport, Rect viewportImageRect, bool animate = false);

		void UpdateCroppedRect(ThumbPosition position, Point diffPos);

		void UpdateSelectionThumbs(bool animate = false);

		void UpdateSelectionThumbs(Point startPoint, Point endPoint, bool animate = false);

		void UpdateCropShape();

		void UpdateMaskArea(bool animate = false);

		bool TryUpdateAspectRatio();

		void UpdateThumbsVisibility();

		Point SelectionAreaCenter();

		static winrt::Windows::Foundation::IAsyncAction CropImageAsync(WriteableBitmap const& writeableBitmap, IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat);

		static winrt::Windows::Foundation::IAsyncAction CropImageWithShapeAsync(WriteableBitmap const& writeableBitmap, IRandomAccessStream const& stream, Rect croppedRect, BitmapFileFormat bitmapFileFormat, winrt::CommunityToolkit::WinUI::Controls::CropShape cropShape);

		static CanvasGeometry CreateClipGeometry(ICanvasResourceCreator resourceCreator, winrt::CommunityToolkit::WinUI::Controls::CropShape cropShape, Size croppedSize);

		static winrt::guid GetEncoderId(BitmapFileFormat bitmapFileFormat);

		static Point GetSafePoint(Rect targetRect, Point point);

		static bool IsSafePoint(Rect targetRect, Point point);

		static bool IsSafeRect(Point startPoint, Point endPoint, Size minSize);

		static Rect GetSafeRect(Point startPoint, Point endPoint, Size minSize, ThumbPosition position);

		static Rect GetUniformRect(Rect targetRect, double aspectRatio);

		static bool IsValidRect(Rect targetRect);

		static Point GetSafeSizeChangeWhenKeepAspectRatio(Rect targetRect, ThumbPosition thumbPosition, Rect selectedRect, Point originSizeChange, double aspectRatio);

		static bool CanContains(Rect targetRect, Rect testRect);

		static bool TryGetContainedRect(Rect targetRect, Rect& testRect);

		static bool IsCornerThumb(ThumbPosition thumbPosition);

		void ImageCropperThumb_KeyDown(winrt::Windows::Foundation::IInspectable const& sender, KeyRoutedEventArgs const& e);

		void ImageCropperThumb_KeyUp(winrt::Windows::Foundation::IInspectable const& sender, KeyRoutedEventArgs const& e);

		void ImageCropperThumb_ManipulationCompleted(winrt::Windows::Foundation::IInspectable const& sender, ManipulationCompletedRoutedEventArgs const& e);

		void ImageCropperThumb_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

		void SourceImage_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e);

		void ImageCanvas_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, SizeChangedEventArgs const& e);

		static void AnimateUIElementOffset(Point to, TimeSpan duration, UIElement const& target);

		static void AnimateUIElementScale(double to, TimeSpan duration, UIElement const& target);

		static DoubleAnimation CreateDoubleAnimation(double to, TimeSpan duration, DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation);

		static PointAnimation CreatePointAnimation(Point to, TimeSpan duration, DependencyObject const& target, std::wstring_view propertyName, bool enableDependentAnimation);

		static ObjectAnimationUsingKeyFrames CreateRectangleAnimation(Rect to, TimeSpan duration, RectangleGeometry rectangle, bool enableDependentAnimation);

		static std::vector<DiscreteObjectKeyFrame> GetRectKeyframes(Rect from, Rect to, TimeSpan duration);

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

namespace winrt::CommunityToolkit::WinUI::Controls::factory_implementation
{
	struct ImageCropper : ImageCropperT<ImageCropper, implementation::ImageCropper>
	{
	};
}
