#include "pch.h"
#include "winrt_module_imports.h"
#ifdef __INTELLISENSE__
#include <algorithm>
#endif
#include "ImageCropper.h"

namespace winrt
{
	using namespace Windows::System;
	using namespace Windows::Foundation;
	using namespace Windows::UI::Core;
	using namespace Microsoft::UI::Input;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Input;
	using namespace XamlToolkit::WinUI::Controls;
}

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	void ImageCropper::ImageCropperThumb_KeyDown(winrt::IInspectable const& sender, winrt::KeyRoutedEventArgs const& e)
	{
		auto changed = false;
		winrt::Point diffPos{};
		if (e.Key() == VirtualKey::Left)
		{
			diffPos.X--;
			auto upKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Up);
			auto downKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Down);

			if (upKeyState == winrt::CoreVirtualKeyStates::Down)
			{
				diffPos.Y--;
			}

			if (downKeyState == winrt::CoreVirtualKeyStates::Down)
			{
				diffPos.Y++;
			}

			changed = true;
		}
		else if (e.Key() == winrt::VirtualKey::Right)
		{
			diffPos.X++;

			auto upKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Up);
			auto downKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Down);

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
		else if (e.Key() == winrt::VirtualKey::Up)
		{
			diffPos.Y--;

			auto leftKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Left);
			auto rightKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Right);

			if (leftKeyState == winrt::CoreVirtualKeyStates::Down)
			{
				diffPos.X--;
			}

			if (rightKeyState == winrt::CoreVirtualKeyStates::Down)
			{
				diffPos.X++;
			}

			changed = true;
		}
		else if (e.Key() == winrt::VirtualKey::Down)
		{
			diffPos.Y++;

			auto leftKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Left);
			auto rightKeyState = winrt::InputKeyboardSource::GetKeyStateForCurrentThread(winrt::VirtualKey::Right);

			if (leftKeyState == winrt::CoreVirtualKeyStates::Down)
			{
				diffPos.X--;
			}

			if (rightKeyState == winrt::CoreVirtualKeyStates::Down)
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

	void ImageCropper::ImageCropperThumb_KeyUp([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::KeyRoutedEventArgs const& e)
	{
		auto selectedRect = ToRect(winrt::Point(_startX, _startY), winrt::Point(_endX, _endY));
		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		if (croppedRect.Width > MinCropSize().Width && croppedRect.Height > MinCropSize().Height)
		{
			croppedRect = winrt::RectHelper::Intersect(croppedRect, _restrictedCropRect);
			_currentCroppedRect = croppedRect;
		}

		if (TryUpdateImageLayout(true))
		{
			UpdateSelectionThumbs(true);
			UpdateMaskArea(true);
		}
	}

	void ImageCropper::ImageCropperThumb_ManipulationCompleted([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::ManipulationCompletedRoutedEventArgs const& e)
	{
		auto selectedRect = ToRect(
			winrt::Point(static_cast<float>(_startX), static_cast<float>(_startY)),
			winrt::Point(static_cast<float>(_endX), static_cast<float>(_endY)));

		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		if (croppedRect.Width > MinCropSize().Width && croppedRect.Height > MinCropSize().Height)
		{
			croppedRect = winrt::RectHelper::Intersect(croppedRect, _restrictedCropRect);
			_currentCroppedRect = croppedRect;
		}

		if (TryUpdateImageLayout(true))
		{
			UpdateSelectionThumbs(true);
			UpdateMaskArea(true);
		}
	}

	void ImageCropper::ImageCropperThumb_ManipulationDelta(winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e)
	{
		auto imageCropperThumb = sender.as<Controls::ImageCropperThumb>();
		auto currentPointerX = imageCropperThumb.X() + e.Position().X + e.Delta().Translation.X - (imageCropperThumb.ActualWidth() / 2);
		auto currentPointerY = imageCropperThumb.Y() + e.Position().Y + e.Delta().Translation.Y - (imageCropperThumb.ActualHeight() / 2);
		auto currentPointerPosition = winrt::Point(static_cast<float>(currentPointerX), static_cast<float>(currentPointerY));
		auto safePosition = GetSafePoint(_restrictedSelectRect, currentPointerPosition);
		auto safeX = safePosition.X - imageCropperThumb.X();
		auto safeY = safePosition.Y - imageCropperThumb.Y();
		auto safeDiffPoint = winrt::Point(static_cast<float>(safeX), static_cast<float>(safeY));
		UpdateCroppedRect(imageCropperThumb.Position(), safeDiffPoint);
	}

	void ImageCropper::SourceImage_ManipulationDelta([[maybe_unused]] winrt::IInspectable const& sender, winrt::ManipulationDeltaRoutedEventArgs const& e)
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

		auto selectedRect = ToRect(winrt::Point(_startX, _startY), winrt::Point(_endX, _endY));
		selectedRect.X += offsetX;
		selectedRect.Y += offsetY;
		auto croppedRect = _inverseImageTransform.TransformBounds(selectedRect);
		croppedRect = winrt::RectHelper::Intersect(croppedRect, _restrictedCropRect);
		_currentCroppedRect = croppedRect;

		if (TryUpdateImageLayout())
		{
			UpdateSelectionThumbs();
			UpdateMaskArea();
		}
	}

	void ImageCropper::ImageCanvas_SizeChanged([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::SizeChangedEventArgs const& e)
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
