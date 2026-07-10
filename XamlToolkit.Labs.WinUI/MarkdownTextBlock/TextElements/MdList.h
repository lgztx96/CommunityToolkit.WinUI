// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
#pragma once

#include "../Extension.h"
#include "IAddChild.h"
#include "MdFlowDocument.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Documents.h>
#endif

namespace winrt
{
	using namespace Windows::Foundation;
	using namespace Microsoft::UI::Xaml;
	using namespace Microsoft::UI::Xaml::Controls;

}

namespace winrt::XamlToolkit::Labs::WinUI::TextElements
{
	enum class BulletType
	{
		Circle,
		Number,
		LowerAlpha,
		UpperAlpha,
		LowerRoman,
		UpperRoman
	};

	class MdList final : public IAddChild
	{
	private:
		winrt::Paragraph _paragraph;
		winrt::InlineUIContainer _container;
		winrt::StackPanel _stackPanel;
		BulletType _bulletType;
		bool _isOrdered;
		int _startIndex = 1;
		int _index = 1;
		static constexpr auto _dot = L"• ";
		static constexpr auto UnorderedListDot = L"• ";
		wchar_t _delimiter = '.';

	public:
		winrt::TextElement TextElement() const override
		{
			return _paragraph;
		}

		MdList([[maybe_unused]] int isTight, wchar_t delimiter) : _isOrdered(false), _delimiter(delimiter)
		{

		}

		MdList(uint32_t start, [[maybe_unused]] int isTight, wchar_t delimiter) : _startIndex(start), _isOrdered(false), _delimiter(delimiter)
		{
			_bulletType = BulletType::Circle; // ToBulletType(bulletType);
			_index = _startIndex;
		}

		void Enter() override 
		{
			_stackPanel.Orientation(winrt::Orientation::Vertical);
			_container.Child(_stackPanel);
			_paragraph.Inlines().Append(_container);
		}

		void Leave()  override
		{

		}

		void AddChild(IAddChild* child) override
		{
			winrt::Grid grid;
			winrt::ColumnDefinition column;
			column.Width(winrt::GridLength(1, winrt::GridUnitType::Auto));
			grid.ColumnDefinitions().Append(column);
			column = winrt::ColumnDefinition();
			column.Width(winrt::GridLength(1, winrt::GridUnitType::Star));
			grid.ColumnDefinitions().Append(column);
			winrt::hstring bullet;
			if (_isOrdered)
			{
				switch (_bulletType)
				{
				case BulletType::Number:
					bullet = winrt::format(L"{}{} ", _index, _delimiter);
					break;
				case BulletType::LowerAlpha:
					bullet = winrt::format(L"{}. ", Extensions::ToAlphabetical(_index));
					break;
				case BulletType::UpperAlpha:
					bullet = winrt::format(L"{}. ", Extensions::ToAlphabetical(_index, true));
					break;
				case BulletType::LowerRoman:
					bullet = winrt::format(L"{} ", Extensions::ToRomanNumerals(_index, true));
					break;
				case BulletType::UpperRoman:
					bullet = winrt::format(L"{} ", Extensions::ToRomanNumerals(_index));
					break;
				case BulletType::Circle:
				default:
					bullet = _dot;
				};
				_index++;
			}
			else
			{
				bullet = _dot;
			}

			winrt::TextBlock textBlock;
			textBlock.Text(bullet);
			textBlock.SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(0));
			textBlock.VerticalAlignment(winrt::VerticalAlignment::Top);
			grid.Children().Append(textBlock);
			MdFlowDocument flowDoc;
			flowDoc.AddChild(child);

			flowDoc.RichTextBlock().SetValue(winrt::Grid::ColumnProperty(), winrt::box_value(1));
			flowDoc.RichTextBlock().Padding(winrt::Thickness(0));
			flowDoc.RichTextBlock().VerticalAlignment(winrt::VerticalAlignment::Top);
			grid.Children().Append(flowDoc.RichTextBlock());

			_stackPanel.Children().Append(grid);
		}

		static winrt::hstring GetBulletString(bool isOrdered, BulletType bulletType, int index)
		{
			if (isOrdered)
			{
				switch (bulletType)
				{
				case BulletType::Number:
					return winrt::format(L"{}. ", index);
					break;
				case BulletType::LowerAlpha:
					return winrt::format(L"{}. ", Extensions::ToAlphabetical(index));
					break;
				case BulletType::UpperAlpha:
					return winrt::format(L"{}. ", Extensions::ToAlphabetical(index, true));
					break;
				case BulletType::LowerRoman:
					return winrt::format(L"{} ", Extensions::ToRomanNumerals(index, true));
					break;
				case BulletType::UpperRoman:
					return winrt::format(L"{} ", Extensions::ToRomanNumerals(index));
					break;
				case BulletType::Circle:
					return _dot;
				default:
					return _dot;
				};
			}
			else
			{
				return UnorderedListDot;
			}
		}

		static BulletType ToBulletType(wchar_t bullet)
		{
			/// Gets or sets the type of the bullet (e.g: '1', 'a', 'A', 'i', 'I').
			switch (bullet)
			{
			case L'1':
				return BulletType::Number;
			case L'a':
				return BulletType::LowerAlpha;
			case L'A':
				return BulletType::UpperAlpha;
			case L'i':
				return BulletType::LowerRoman;
			case L'I':
				return BulletType::UpperRoman;
			default:
				return BulletType::Circle;
			}
		}
	};
}

