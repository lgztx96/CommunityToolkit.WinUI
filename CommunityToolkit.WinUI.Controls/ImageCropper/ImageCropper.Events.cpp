#include "pch.h"
#include "ImageCropper.h"

using namespace winrt;
using namespace Windows::UI::Core;
using namespace Microsoft::UI::Input;
using namespace Microsoft::UI::Xaml;

namespace winrt::CommunityToolkit::WinUI::Controls::implementation
{
	void ImageCropper::ImageCropperThumb_KeyDown(winrt::Windows::Foundation::IInspectable const& sender, KeyRoutedEventArgs const& e)
	{
		auto changed = false;
		Point diffPos{};
		if (e.Key() == VirtualKey::Left)
		{
			diffPos.X--;
			auto upKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Up);
			auto downKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Down);

			if (upKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.Y--;
			}

			if (downKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.Y++;
			}

			changed = true;
		}
		else if (e.Key() == VirtualKey::Right)
		{
			diffPos.X++;

			auto upKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Up);
			auto downKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Down);

			if (upKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.Y--;
			}

			if (downKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.Y++;
			}

			changed = true;
		}
		else if (e.Key() == VirtualKey::Up)
		{
			diffPos.Y--;

			auto leftKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Left);
			auto rightKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Right);

			if (leftKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.X--;
			}

			if (rightKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.X++;
			}

			changed = true;
		}
		else if (e.Key() == VirtualKey::Down)
		{
			diffPos.Y++;

			auto leftKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Left);
			auto rightKeyState = InputKeyboardSource::GetKeyStateForCurrentThread(VirtualKey::Right);

			if (leftKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.X--;
			}

			if (rightKeyState == CoreVirtualKeyStates::Down)
			{
				diffPos.X++;
			}

			changed = true;
		}

		if (changed)
		{
			auto imageCropperThumb = sender.as<Controls::ImageCropperThumb>();
			UpdateCroppedRect(imageCropperThumb.Position(), diffPos);
		}
	}

	void ImageCropper::ImageCropperThumb_KeyUp([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] KeyRoutedEventArgs const& e)
	{
		auto selectedRect = ToRect(Point(_startX, _startY), Point(_endX, _endY));
		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		if (croppedRect.Width > MinCropSize().Width && croppedRect.Height > MinCropSize().Height)
		{
			croppedRect = RectHelper::Intersect(croppedRect, _restrictedCropRect);
			_currentCroppedRect = croppedRect;
		}

		if (TryUpdateImageLayout(true))
		{
			UpdateSelectionThumbs(true);
			UpdateMaskArea(true);
		}
	}

	void ImageCropper::ImageCropperThumb_ManipulationCompleted([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] ManipulationCompletedRoutedEventArgs const& e)
	{
		auto selectedRect = ToRect(
			Point(static_cast<float>(_startX), static_cast<float>(_startY)),
			Point(static_cast<float>(_endX), static_cast<float>(_endY)));

		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		if (croppedRect.Width > MinCropSize().Width && croppedRect.Height > MinCropSize().Height)
		{
			croppedRect = RectHelper::Intersect(croppedRect, _restrictedCropRect);
			_currentCroppedRect = croppedRect;
		}

		if (TryUpdateImageLayout(true))
		{
			UpdateSelectionThumbs(true);
			UpdateMaskArea(true);
		}
	}

	void ImageCropper::ImageCropperThumb_ManipulationDelta(winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e)
	{
		auto imageCropperThumb = sender.as<Controls::ImageCropperThumb>();
		auto currentPointerX = imageCropperThumb.X() + e.Position().X + e.Delta().Translation.X - (imageCropperThumb.ActualWidth() / 2);
		auto currentPointerY = imageCropperThumb.Y() + e.Position().Y + e.Delta().Translation.Y - (imageCropperThumb.ActualHeight() / 2);
		auto currentPointerPosition = Point(static_cast<float>(currentPointerX), static_cast<float>(currentPointerY));
		auto safePosition = GetSafePoint(_restrictedSelectRect, currentPointerPosition);
		auto safeX = safePosition.X - imageCropperThumb.X();
		auto safeY = safePosition.Y - imageCropperThumb.Y();
		auto safeDiffPoint = Point(static_cast<float>(safeX), static_cast<float>(safeY));
		UpdateCroppedRect(imageCropperThumb.Position(), safeDiffPoint);
	}

	void ImageCropper::SourceImage_ManipulationDelta([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, ManipulationDeltaRoutedEventArgs const& e)
	{
		auto offsetX = -e.Delta().Translation.X;
		auto offsetY = -e.Delta().Translation.Y;
		if (offsetX > 0)
		{
			offsetX = std::min<float>(offsetX, _restrictedSelectRect.X + _restrictedSelectRect.Width - _endX);
		}
		else
		{
			offsetX = std::max<float>(offsetX, _restrictedSelectRect.X - _startX);
		}

		if (offsetY > 0)
		{
			offsetY = std::min<float>(offsetY, _restrictedSelectRect.Y + _restrictedSelectRect.Height - _endY);
		}
		else
		{
			offsetY = std::max<float>(offsetY, _restrictedSelectRect.Y - _startY);
		}

		auto selectedRect = ToRect(Point(_startX, _startY), Point(_endX, _endY));
		selectedRect.X += offsetX;
		selectedRect.Y += offsetY;
		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		croppedRect = RectHelper::Intersect(croppedRect, _restrictedCropRect);
		_currentCroppedRect = croppedRect;

		if (TryUpdateImageLayout())
		{
			UpdateSelectionThumbs();
			UpdateMaskArea();
		}
	}

	void ImageCropper::ImageCanvas_SizeChanged([[maybe_unused]] winrt::Windows::Foundation::IInspectable const& sender, [[maybe_unused]] SizeChangedEventArgs const& e)
	{
		if (Source() == nullptr)
		{
			return;
		}

		if (TryUpdateImageLayout())
		{
			UpdateSelectionThumbs();
		}

		if (TryUpdateAspectRatio())
		{
			UpdateSelectionThumbs();
			UpdateMaskArea();
		}
	}
}