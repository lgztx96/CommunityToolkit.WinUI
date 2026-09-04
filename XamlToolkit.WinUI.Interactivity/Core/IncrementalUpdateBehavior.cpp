#include "pch.h"
#include "winrt_module_imports.h"
#include "IncrementalUpdateBehavior.h"
#if __has_include("IncrementalUpdateBehavior.g.cpp")
#include "IncrementalUpdateBehavior.g.cpp"
#endif
#
namespace winrt::XamlToolkit::WinUI::Interactivity::implementation
{
	const wil::single_threaded_property<winrt::DependencyProperty> IncrementalUpdateBehavior::PhaseProperty =
		winrt::DependencyProperty::Register(
			L"Phase",
			winrt::xaml_typename<int>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(winrt::box_value(1), &IncrementalUpdateBehavior::OnPhaseChanged));

	const wil::single_threaded_property<winrt::DependencyProperty> IncrementalUpdateBehavior::IncrementalUpdaterProperty =
		winrt::DependencyProperty::RegisterAttached(
			L"IncrementalUpdater",
			winrt::xaml_typename<winrt::IInspectable>(),
			winrt::xaml_typename<class_type>(),
			winrt::PropertyMetadata(nullptr, &IncrementalUpdateBehavior::OnIncrementalUpdaterChanged));

	int IncrementalUpdateBehavior::Phase() const
	{
		return winrt::unbox_value<int>(GetValue(PhaseProperty()));
	}

	void IncrementalUpdateBehavior::Phase(int value)
	{
		SetValue(PhaseProperty(), winrt::box_value(value));
	}

	void IncrementalUpdateBehavior::OnPhaseChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		const auto behavior = winrt::get_self<IncrementalUpdateBehavior>(sender.as<class_type>())->get_strong();

		winrt::com_ptr<IncrementalUpdater> incrementalUpdater = behavior->FindUpdater();
		winrt::FrameworkElement frameworkElement = behavior->AssociatedObject();

		if (incrementalUpdater && frameworkElement)
		{
			incrementalUpdater->UncachePhaseElement(frameworkElement, winrt::unbox_value<int>(args.OldValue()));
			incrementalUpdater->CachePhaseElement(frameworkElement, winrt::unbox_value<int>(args.NewValue()));
		}
	}

	winrt::com_ptr<IncrementalUpdater> IncrementalUpdateBehavior::GetIncrementalUpdater(winrt::DependencyObject const& dependencyObject)
	{
		return dependencyObject.GetValue(IncrementalUpdateBehavior::IncrementalUpdaterProperty()).try_as<IncrementalUpdater>();
	}

	void IncrementalUpdateBehavior::SetIncrementalUpdater(winrt::DependencyObject const& dependencyObject, winrt::com_ptr<IncrementalUpdater> const& value)
	{
		dependencyObject.SetValue(IncrementalUpdateBehavior::IncrementalUpdaterProperty(), *value);
	}

	void IncrementalUpdateBehavior::OnIncrementalUpdaterChanged(winrt::DependencyObject const& sender, winrt::DependencyPropertyChangedEventArgs const& args)
	{
		if (const auto oldValue = args.OldValue())
		{
			const auto incrementalUpdater = winrt::get_self<IncrementalUpdater>(oldValue);
			incrementalUpdater->Detach();
		}
		if (const auto newValue = args.NewValue())
		{
			const auto incrementalUpdater = winrt::get_self<IncrementalUpdater>(newValue);
			incrementalUpdater->Attach(sender);
		}
	}

	void IncrementalUpdateBehavior::OnAssociatedObjectLoaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		winrt::com_ptr<IncrementalUpdater> incrementalUpdater = FindUpdater();

		if (incrementalUpdater)
		{
			incrementalUpdater->CachePhaseElement(AssociatedObject(), Phase());
		}
	}

	void IncrementalUpdateBehavior::OnAssociatedObjectUnloaded([[maybe_unused]] winrt::IInspectable const& sender, [[maybe_unused]] winrt::RoutedEventArgs const& e)
	{
		if (_updater)
		{
			_updater->UncachePhaseElement(AssociatedObject(), Phase());
		}

		_updater = nullptr;
	}

	winrt::com_ptr<IncrementalUpdater> IncrementalUpdateBehavior::FindUpdater()
	{
		if (_updater)
		{
			return _updater;
		}

		winrt::DependencyObject ancestor = AssociatedObject();
		while (ancestor)
		{
			winrt::DependencyObject parent = winrt::VisualTreeHelper::GetParent(ancestor);

			if (winrt::ListViewBase listView = parent.try_as<winrt::ListViewBase>())
			{
				winrt::com_ptr<IncrementalUpdater> currentUpdater = IncrementalUpdateBehavior::GetIncrementalUpdater(listView);
				if (!currentUpdater)
				{
					currentUpdater = winrt::make_self<IncrementalUpdater>();

					IncrementalUpdateBehavior::SetIncrementalUpdater(listView, currentUpdater);
				}
				return currentUpdater;
			}
			ancestor = parent;
		}

		return nullptr;
	}

	void IncrementalUpdateBehavior::OnAttached()
	{
		Behavior::OnAttached();
		const auto object = AssociatedObject();
		if (!object)
		{
			const auto actualType = winrt::get_class_name(object);
			std::wstring_view expectedType = winrt::name_of<winrt::FrameworkElement>();
			const auto message = ResourceHelper::Format(ResourceHelper::InvalidAssociatedObjectExceptionMessage(), actualType, expectedType);
			throw winrt::hresult_error(E_FAIL, message);
		}

		_associatedObjectLoadedRevoker = object.Loaded(winrt::auto_revoke, { this, &IncrementalUpdateBehavior::OnAssociatedObjectLoaded });
		_associatedObjectUnloadedRevoker = object.Unloaded(winrt::auto_revoke, { this, &IncrementalUpdateBehavior::OnAssociatedObjectUnloaded });
	}

	void IncrementalUpdateBehavior::OnDetaching()
	{
		Behavior::OnDetaching();
		_associatedObjectLoadedRevoker.revoke();
		_associatedObjectUnloadedRevoker.revoke();
		// no need to perform the work that Unloaded would have done - that's just housekeeping on the cache, which is now going away
	}

	void IncrementalUpdater::PhasedElementRecord::FreezeAndHide()
	{
		if (_isFrozen)
		{
			return;
		}

		_isFrozen = true;
		_localOpacity = _frameworkElement.ReadLocalValue(winrt::UIElement::OpacityProperty());
		_localDataContext = _frameworkElement.ReadLocalValue(winrt::FrameworkElement::DataContextProperty());
		_frameworkElement.Opacity(0.0);
		_frameworkElement.DataContext(_frameworkElement.DataContext());
	}

	void IncrementalUpdater::PhasedElementRecord::ThawAndShow()
	{
		if (!_isFrozen)
		{
			return;
		}

		if (_localOpacity != winrt::DependencyProperty::UnsetValue())
		{
			_frameworkElement.SetValue(winrt::UIElement::OpacityProperty(), _localOpacity);
		}
		else
		{
			_frameworkElement.ClearValue(winrt::UIElement::OpacityProperty());
		}

		if (_localDataContext != DependencyProperty::UnsetValue())
		{
			_frameworkElement.SetValue(winrt::FrameworkElement::DataContextProperty(), _localDataContext);
		}
		else
		{
			_frameworkElement.ClearValue(winrt::FrameworkElement::DataContextProperty());
		}

		_isFrozen = false;
	}

	void IncrementalUpdater::OnContainerContentChanging([[maybe_unused]] winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& e)
	{
		winrt::UIElement contentTemplateRoot = e.ItemContainer().ContentTemplateRoot();

		if (const auto iter = _elementCache.find(contentTemplateRoot); iter != _elementCache.end())
		{
			const auto& elementCacheRecord = iter->second;
			if (!e.InRecycleQueue())
			{
				for (auto& phaseRecord : elementCacheRecord->ElementsByPhase())
				{
					for (auto& phasedElementRecord : phaseRecord)
					{
						phasedElementRecord.FreezeAndHide();
					}
				}

				if (elementCacheRecord->Phases().size() > 0)
				{
					e.RegisterUpdateCallback(static_cast<uint32_t>(elementCacheRecord->Phases()[0]), { this, &IncrementalUpdater::OnContainerContentChangingCallback });
				}

				// set the DataContext manually since we inhibit default operation by setting e.Handled=true	
				contentTemplateRoot.as<winrt::FrameworkElement>().DataContext(e.Item());
			}
			else
			{
				// clear the DataContext manually since we inhibit default operation by setting e.Handled=true
				contentTemplateRoot.ClearValue(winrt::FrameworkElement::DataContextProperty());

				for (auto& phaseRecord : elementCacheRecord->ElementsByPhase())
				{
					for (auto& phasedElementRecord : phaseRecord)
					{
						phasedElementRecord.ThawAndShow();
					}
				}
			}
		}
	}

	void IncrementalUpdater::OnContainerContentChangingCallback([[maybe_unused]] winrt::ListViewBase const& sender, winrt::ContainerContentChangingEventArgs const& e)
	{
		winrt::UIElement contentTemplateRoot = e.ItemContainer().ContentTemplateRoot();

		if (const auto iter = _elementCache.find(contentTemplateRoot); iter != _elementCache.end())
		{
			const auto& elementCacheRecord = iter->second;
			auto& phases = elementCacheRecord->Phases();

			const auto it = std::lower_bound(phases.begin(), phases.end(), (int)e.Phase());

			int phaseIndex;
			if (it != phases.end() && *it == (int)e.Phase())
			{
				phaseIndex = static_cast<int>(std::distance(phases.begin(), it));

				for (auto& phasedElementRecord : elementCacheRecord->ElementsByPhase()[phaseIndex])
				{
					phasedElementRecord.ThawAndShow();
				}

				phaseIndex++;
			}
			else
			{
				// don't know why this phase was not found, but by BinarySearch rules, ~phaseIndex is the place
				// where it would be inserted, thus the item there has the next higher number.
				phaseIndex = static_cast<int>(std::distance(phases.begin(), it));
			}

			const auto recordSize = elementCacheRecord->Phases().size();
			if (phaseIndex < static_cast<int>(recordSize))
			{
				e.RegisterUpdateCallback(static_cast<uint32_t>(elementCacheRecord->Phases()[phaseIndex]), { this, &IncrementalUpdater::OnContainerContentChangingCallback });
			}
		}
	}

	winrt::UIElement IncrementalUpdater::FindContentTemplateRoot(winrt::FrameworkElement const& phaseElement)
	{
		winrt::DependencyObject ancestor = phaseElement;
		while (ancestor)
		{
			winrt::DependencyObject parent = winrt::VisualTreeHelper::GetParent(ancestor);
			
			if (const auto item = parent.try_as<winrt::SelectorItem>())
			{
				return item.ContentTemplateRoot();
			}
			ancestor = parent;
		}

		return nullptr;
	}

	void IncrementalUpdater::CachePhaseElement(winrt::FrameworkElement const& phaseElement, int phase)
	{
		if (phase < 0)
		{
			throw winrt::hresult_invalid_argument(L"phase");
		}

		if (phase == 0)
		{
			return;
		}

		winrt::UIElement contentTemplateRoot = IncrementalUpdater::FindContentTemplateRoot(phaseElement);
		if (contentTemplateRoot)
		{
			// get the cache for this element
			ElementCacheRecord* elementCacheRecord;
			const auto iter = _elementCache.find(contentTemplateRoot);
			if (iter == _elementCache.end())
			{
				elementCacheRecord = new ElementCacheRecord();
				_elementCache.emplace(contentTemplateRoot, elementCacheRecord);
			}
			else {
				elementCacheRecord = iter->second.get();
			}

			int phaseIndex;
			auto& phases = elementCacheRecord->Phases();
			// get the cache for this phase
			{
				const auto it = std::lower_bound(phases.begin(), phases.end(), phase);
				phaseIndex = static_cast<int>(it - phases.begin());

				if (it == phases.end() || *it != phase)
				{
					// not found - insert
					phases.insert(phases.begin() + phaseIndex, phase);
					auto& elementsByPhase = elementCacheRecord->ElementsByPhase();
					elementsByPhase.emplace(elementsByPhase.begin() + phaseIndex);
				}
			}

			std::vector<PhasedElementRecord>& phasedElementRecords = elementCacheRecord->ElementsByPhase()[phaseIndex];

			// first see if the element is already there
			for (size_t i = 0; i < phasedElementRecords.size(); i++)
			{
				if (phasedElementRecords[i].FrameworkElement() == phaseElement)
				{
					return;
				}
			}

			// insert the element
			phasedElementRecords.emplace_back(phaseElement);
		}
	}

	void IncrementalUpdater::UncachePhaseElement(winrt::FrameworkElement const& phaseElement, int phase)
	{
		if (phase <= 0)
		{
			return;
		}

		winrt::UIElement contentTemplateRoot = IncrementalUpdater::FindContentTemplateRoot(phaseElement);
		if (contentTemplateRoot)
		{
			// get the cache for this element
			if (auto iter = _elementCache.find(contentTemplateRoot); iter != _elementCache.end())
			{
				const auto& elementCacheRecord = iter->second;
				// get the cache for this phase
				auto& phases = elementCacheRecord->Phases();

				const auto it = std::lower_bound(phases.begin(), phases.end(), phase);

				const int phaseIndex = (it != phases.end() && *it == phase)
					? static_cast<int>(std::distance(phases.begin(), it))
					: -1;

				if (phaseIndex >= 0)
				{
					// remove the element: the linear search here is not spectacular but the list should be very short
					std::vector<PhasedElementRecord>& phasedElementRecords = elementCacheRecord->ElementsByPhase()[phaseIndex];

					for (size_t i = 0; i < phasedElementRecords.size(); i++)
					{
						if (phasedElementRecords[i].FrameworkElement() == phaseElement)
						{
							phasedElementRecords[i].ThawAndShow();

							phasedElementRecords.erase(phasedElementRecords.begin() + i);

							if (phasedElementRecords.size() == 0)
							{
								elementCacheRecord->Phases().erase(elementCacheRecord->Phases().begin() + phaseIndex);
								elementCacheRecord->ElementsByPhase().erase(elementCacheRecord->ElementsByPhase().begin() + phaseIndex);
							}
						}
					}
				}
			}
		}
	}

	void IncrementalUpdater::Attach(winrt::DependencyObject const& dependencyObject)
	{
		_associatedListViewBase = dependencyObject.try_as<winrt::ListViewBase>();

		if (_associatedListViewBase)
		{
			_containerContentChangingRevoker = _associatedListViewBase.ContainerContentChanging(winrt::auto_revoke, { this, &IncrementalUpdater::OnContainerContentChanging });
		}
	}

	void IncrementalUpdater::Detach()
	{
		if (_associatedListViewBase)
		{
			_containerContentChangingRevoker.revoke();
		}
		_associatedListViewBase = nullptr;
	}
}
