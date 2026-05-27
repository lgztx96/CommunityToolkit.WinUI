#include "pch.h"
#include "winrt_module_imports.h"
#include "RichSuggestToken.h"
#if __has_include("RichSuggestToken.g.cpp")
#include "RichSuggestToken.g.cpp"
#endif

namespace winrt::XamlToolkit::WinUI::Controls::implementation
{
	int RichSuggestToken::Position() const
	{
		return _range ? _range.GetIndex(winrt::Microsoft::UI::Text::TextRangeUnit::Character) - 1 : 0;
	}

	void RichSuggestToken::UpdateTextRange(winrt::Microsoft::UI::Text::ITextRange const& range)
	{
		bool rangeStartChanged = RangeStart != range.StartPosition();
		bool rangeEndChanged = RangeEnd != range.EndPosition();
		bool positionChanged = _range == nullptr || rangeStartChanged;
		_range = range.GetClone();
		RangeStart = _range.StartPosition();
		RangeEnd = _range.EndPosition();

		if (rangeStartChanged)
		{
			RaisePropertyChanged(L"RangeStart");
		}

		if (rangeEndChanged)
		{
			RaisePropertyChanged(L"RangeEnd");
		}

		if (positionChanged)
		{
			RaisePropertyChanged(L"Position");
		}
	}

	winrt::hstring RichSuggestToken::ToString() const
	{
		return winrt::format(L"HYPERLINK \"{}\"\u200B{}\u200B", winrt::to_hstring(Id), DisplayText());
	}
}
