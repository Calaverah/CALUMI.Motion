#pragma once

#pragma warning(push,0)
#include <QDialog>
#include <QInputDialog>
#include <QObject>
#include <QSortFilterProxyModel>
#pragma warning(pop)
#include "Widgets/FilteredDropDownDialog.h"
#include "Widgets/MultiVariableDialog.h"
#include "Utilities/AgxDefinitions.h"
#include "Widgets/BitfieldWidgetDialog.h"


class AgxEventFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	AgxEventFilterProxyModel(QObject* parent = nullptr);

	//void setFilter(const AgxEventType& type);
	void setFilterSource(FilteredDropDownDialog& dialog);

public Q_SLOTS:
	void FilterChange(const QString& str);

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
	bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
	AgxEventType _type = AgxEventType::UNDEFINED;
};



class DialogPool_SFBGS
{

private:
	DialogPool_SFBGS();

	~DialogPool_SFBGS();

public:
	DialogPool_SFBGS(const DialogPool_SFBGS&) = delete;
	DialogPool_SFBGS(DialogPool_SFBGS&&) = delete;

	DialogPool_SFBGS& operator=(const DialogPool_SFBGS&) = delete;
	DialogPool_SFBGS& operator=(DialogPool_SFBGS&&) = delete;

	static DialogPool_SFBGS& GetInstance() {
		static DialogPool_SFBGS instance;
		return instance;
	}

	FilteredDropDownDialog* GetEventEntryDialog(QString str = "", AgxEventType iType = AgxEventType::UNDEFINED);
	MultiVariableDialog* GetFloatVariableEntryDialog(QString str = "");
	MultiVariableDialog* GetIntegerVariableEntryDialog(QString str = "");
	MultiVariableDialog* GetBooleanVariableEntryDialog(QString str = "");
	MultiVariableDialog* GetVectorVariableEntryDialog(QString str = "");
	MultiVariableDialog* GetPrefixDialog(QString str = "");
	MultiVariableDialog* GetSuffixDialog(QString str = "");
	MultiVariableDialog* GetActionVariableDialog(QString str = "");
	MultiVariableDialog* GetStateVariableDialog(QString str = "");
	MultiVariableDialog* GetSyncVariableDialog(QString str = "");
	BitfieldWidgetDialog* GetAnimationFlagDialog(size_t initialValue = 0);
	//MultiVariableDialog* GetAnimationTriggersDialog(QString str = "");

private:
	FilteredDropDownDialog* _EventEntryDialog;
	MultiVariableDialog* _FloatEntryDialog;
	MultiVariableDialog* _IntegerEntryDialog;
	MultiVariableDialog* _BooleanEntryDialog;
	MultiVariableDialog* _VectorEntryDialog;

	MultiVariableDialog* _PrefixDialog;
	MultiVariableDialog* _SuffixDialog;
	MultiVariableDialog* _StateVarDialog;
	MultiVariableDialog* _ActionVarDialog;
	MultiVariableDialog* _SyncDialog;

	BitfieldWidgetDialog* _AnimationFlagsDialog;

	//MultiVariableDialog* _AnimTriggersDialog;

	QStandardItemModel* _eventModel;
	
};



