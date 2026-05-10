#include "stdafx.h"
#include "DialogPool.h"
#include <QApplication>
#include <memory>
#include <QObject>
#include "Utilities/AgxDefinitions.h"
#include "Widgets/FilteredDropDownDialog.h"
#include <QMap>
#include <windows.h>


AgxEventFilterProxyModel::AgxEventFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
	setDynamicSortFilter(true);
	setSortCaseSensitivity(Qt::CaseInsensitive);
	sort(0);
}

void AgxEventFilterProxyModel::setFilterSource(FilteredDropDownDialog& dialog)
{
	_type = _type; //using as a way to prevent accidentally making this const as it messes up the connection for some reason.
	dialog.GetFilterBox()->connect(dialog.GetFilterBox(), &QComboBox::currentTextChanged, this, &AgxEventFilterProxyModel::FilterChange);
}

void AgxEventFilterProxyModel::FilterChange(const QString& str)
{
	beginFilterChange();
	auto resp = AgxEventTypeMap.contains(str) ? AgxEventTypeMap[str] : AgxEventType::UNDEFINED;
	_type = resp;
	endFilterChange();
}

bool AgxEventFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
	
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	AgxEventType checkType = sourceModel()->data(index, Qt::UserRole + 1).value<AgxEventInfo>().eventType;
	
	if (_type == checkType) return true;
	switch (_type)
	{
	case AgxEventType::Graph_Event:
	case AgxEventType::Game_Event:
		return checkType == AgxEventType::Both;
	case AgxEventType::Both:
		return checkType == AgxEventType::Graph_Event || checkType == AgxEventType::Game_Event;
	case AgxEventType::UNDEFINED:
		return true;
	default:
		return false;
	}
}

bool AgxEventFilterProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	QVariant leftData = sourceModel()->data(left, sortRole());
	QVariant rightData = sourceModel()->data(right, sortRole());

	if (leftData.userType() == QMetaType::QString && rightData.userType() == QMetaType::QString) {

		if (leftData.toString() == "<none>" || leftData.toString() == tr("<none>"))
			return true;

		if (leftData.toString() == "<custom>" || leftData.toString() == tr("<custom>"))
		{
			if (rightData.toString() == "<none>" || rightData.toString() == tr("<none>"))
				return false;

			return true;
		}

		return leftData.toString().compare(rightData.toString(), Qt::CaseInsensitive) < 0;
	}

	return QSortFilterProxyModel::lessThan(left, right);
}

DialogPool_SFBGS::DialogPool_SFBGS()
{
	qInfo() << "SFBGS Dialog Pool Created";

	{
		_eventModel = new QStandardItemModel();

		for (auto entry = AgxEventVars->cbegin(); entry != AgxEventVars->cend(); entry++)
		{
			QStandardItem* item = new QStandardItem(entry.key());
			QVariant var;
			var.setValue(entry.value());
			item->setData(var, Qt::UserRole + 1);
			_eventModel->appendRow(item);
		}

		_EventEntryDialog = new FilteredDropDownDialog();


		AgxEventFilterProxyModel* _eventModelFilter = new AgxEventFilterProxyModel(_EventEntryDialog);
		_eventModelFilter->setSourceModel(_eventModel);

		
		_eventModelFilter->setFilterSource(*_EventEntryDialog);
		_EventEntryDialog->GetMainComboBox()->setModel(_eventModelFilter);


		_EventEntryDialog->setWindowTitle("Select Event");

		_EventEntryDialog->GetFilterBox()->addItem("All");
		_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Graph_Event));
		_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Game_Event));
		_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Both));
		_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Special));
	}

	{
		_FloatEntryDialog = new MultiVariableDialog();
		_FloatEntryDialog->setWindowTitle("Select Float Type Variable");
		_FloatEntryDialog->SetCustomFloat();
		_FloatEntryDialog->GetComboBox()->addItems(AgxFloatVars);
	}

	{
		_IntegerEntryDialog = new MultiVariableDialog();
		_IntegerEntryDialog->setWindowTitle("Select Integer Type Variable");
		_IntegerEntryDialog->SetCustomInteger();
		_IntegerEntryDialog->GetComboBox()->addItems(AgxIntegerVars);
	}

	{
		_BooleanEntryDialog = new MultiVariableDialog();
		_BooleanEntryDialog->setWindowTitle("Select Boolean Type Variable");
		_BooleanEntryDialog->SetCustomBoolean();
		_BooleanEntryDialog->GetComboBox()->addItems(AgxBooleanVars);
	}

	{
		_VectorEntryDialog = new MultiVariableDialog();
		_VectorEntryDialog->setWindowTitle("Select Vector Type Variable");
		_VectorEntryDialog->SetCustomVector();
		_VectorEntryDialog->GetComboBox()->addItems(AgxVectorVars);
	}

	{
		_PrefixDialog = new MultiVariableDialog();
		_PrefixDialog->setWindowTitle("Select Prefix");
		_PrefixDialog->SetCustomString();
		_PrefixDialog->GetComboBox()->addItems(AgxAnimPrefixes);
	}

	{
		_SuffixDialog = new MultiVariableDialog();
		_SuffixDialog->setWindowTitle("Select Suffix");
		_SuffixDialog->SetCustomString();
		_SuffixDialog->GetComboBox()->addItems(AgxAnimSuffixes);
	}
	{
		_ActionVarDialog = new MultiVariableDialog();
		_ActionVarDialog->setWindowTitle("Select Action");
		_ActionVarDialog->SetCustomString();
		_ActionVarDialog->GetComboBox()->addItems(AgxActionVars);
	}
	{
		_StateVarDialog = new MultiVariableDialog();
		_StateVarDialog->setWindowTitle("Select State");
		_StateVarDialog->SetCustomString();
		_StateVarDialog->GetComboBox()->addItems(AgxStateVars);
	}
	{
		_SyncDialog = new MultiVariableDialog();
		_SyncDialog->setWindowTitle("Select Sync System");
		_SyncDialog->SetCustomString();
		_SyncDialog->GetComboBox()->addItems(AgxSyncs);
	}

	{
		_AnimationFlagsDialog = new BitfieldWidgetDialog();
		_AnimationFlagsDialog->setWindowTitle("Select Animation Flags");
		_AnimationFlagsDialog->AddCheckBoxes(AgxAnimationFlags::GetStringList());
	}
}

DialogPool_SFBGS::~DialogPool_SFBGS()
{
	//deletion will be handled for us either by QT or windows releasing memory

	OutputDebugStringA("SFBGS Dialog Pool Destroyed\n");
	if (_EventEntryDialog)
	{
		OutputDebugStringA("_EventEntryDialog Attempt\n");
		//delete _EventEntryDialog;
		_EventEntryDialog = nullptr;
		OutputDebugStringA("_EventEntryDialog Destroyed\n");
	}
	if (_FloatEntryDialog)
	{
		OutputDebugStringA("_FloatEntryDialog Attempt\n");
		//delete _FloatEntryDialog;
		_FloatEntryDialog = nullptr;
		OutputDebugStringA("_FloatEntryDialog Destroyed\n");
	}
	if (_IntegerEntryDialog)
	{
		OutputDebugStringA("_IntegerEntryDialog Attempt\n");
		//delete _IntegerEntryDialog;
		_IntegerEntryDialog = nullptr;
		OutputDebugStringA("_IntegerEntryDialog Destroyed\n");
	}
	if (_BooleanEntryDialog)
	{
		OutputDebugStringA("_BooleanEntryDialog Attempt\n");
		//delete _BooleanEntryDialog;
		_BooleanEntryDialog = nullptr;
		OutputDebugStringA("_BooleanEntryDialog Destroyed\n");
	}
	if (_VectorEntryDialog)
	{
		OutputDebugStringA("_VectorEntryDialog Attempt\n");
		//delete _VectorEntryDialog;
		_VectorEntryDialog = nullptr;
		OutputDebugStringA("_VectorEntryDialog Destroyed\n");
	}
	if (_PrefixDialog)
	{
		OutputDebugStringA("_PrefixDialog Attempt\n");
		//delete _PrefixDialog;
		_PrefixDialog = nullptr;
		OutputDebugStringA("_PrefixDialog Destroyed\n");
	}
	if (_SuffixDialog)
	{
		OutputDebugStringA("_SuffixDialog Attempt\n");
		//delete _SuffixDialog;
		_SuffixDialog = nullptr;
		OutputDebugStringA("_SuffixDialog Destroyed\n");
	}
	if (_ActionVarDialog)
	{
		OutputDebugStringA("_ActionVarDialog Attempt\n");
		//delete _ActionVarDialog;
		_ActionVarDialog = nullptr;
		OutputDebugStringA("_ActionVarDialog Destroyed\n");
	}
	if (_StateVarDialog)
	{
		OutputDebugStringA("_StateVarDialog Attempt\n");
		//delete _StateVarDialog;
		_StateVarDialog = nullptr;
		OutputDebugStringA("_StateVarDialog Destroyed\n");
	}
	if (_SyncDialog)
	{
		OutputDebugStringA("_SyncDialog Attempt\n");
		//delete _SyncDialog;
		_SyncDialog = nullptr;
		OutputDebugStringA("_SyncDialog Destroyed\n");
	}
	if (_AnimationFlagsDialog)
	{
		OutputDebugStringA("_AnimationFlagsDialog Attempt\n");
		//delete _AnimationFlagsDialog;
		_AnimationFlagsDialog = nullptr;
		OutputDebugStringA("_AnimationFlagsDialog Destroyed\n");
	}
}

FilteredDropDownDialog* DialogPool_SFBGS::GetEventEntryDialog(QString str, AgxEventType iType)
{
	int idx = static_cast<int>(iType);

	_EventEntryDialog->GetFilterBox()->setCurrentIndex(idx);
	_EventEntryDialog->SetCustomHolder(str);

	auto idxDrop = _EventEntryDialog->GetMainComboBox()->findText(str);
	auto idxCustom = _EventEntryDialog->GetMainComboBox()->findText("<custom>");

	//until we work out the translations during runtimes for events, this will have to be done to prevent any awkward bugs
	if (idxCustom < 0)
		idxCustom = _EventEntryDialog->GetMainComboBox()->findText(QObject::tr("<custom>"));

	_EventEntryDialog->GetMainComboBox()->setCurrentIndex(0);
	
	if (!str.isEmpty())
	{
		if (idxDrop >= 0)
		{
			_EventEntryDialog->GetMainComboBox()->setCurrentIndex(idxDrop);
		}
		else if (idxCustom >= 0)
		{
			_EventEntryDialog->GetMainComboBox()->setCurrentIndex(idxCustom);
			//_EventEntryDialog->SetCustomHolder(str);
		} 
	}

	return _EventEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetFloatVariableEntryDialog(QString str)
{
	_FloatEntryDialog->SetInitialFloat(str);

	return _FloatEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetIntegerVariableEntryDialog(QString str)
{
	_IntegerEntryDialog->SetInitialInteger(str);

	return _IntegerEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetBooleanVariableEntryDialog(QString str)
{
	_BooleanEntryDialog->SetInitialBoolen(str);

	return _BooleanEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetVectorVariableEntryDialog(QString str)
{
	_VectorEntryDialog->SetInitialVector(str);

	return _VectorEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetPrefixDialog(QString str)
{
	_PrefixDialog->SetInitialString(str);

	return _PrefixDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetSuffixDialog(QString str)
{
	_SuffixDialog->SetInitialString(str);

	return _SuffixDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetActionVariableDialog(QString str)
{
	_ActionVarDialog->SetInitialString(str);

	return _ActionVarDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetStateVariableDialog(QString str)
{
	_StateVarDialog->SetInitialString(str);

	return _StateVarDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetSyncVariableDialog(QString str)
{
	_SyncDialog->SetInitialString(str);

	return _SyncDialog;
}

BitfieldWidgetDialog* DialogPool_SFBGS::GetAnimationFlagDialog(size_t initialValue)
{
	_AnimationFlagsDialog->SetInitialValues(initialValue);
	return _AnimationFlagsDialog;
}