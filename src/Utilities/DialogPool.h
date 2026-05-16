#pragma once

#pragma warning(push,0)
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
	explicit AgxEventFilterProxyModel(QObject* parent = nullptr);

	//void setFilter(const AgxEventType& type);

	//Cannot be const method
	void setFilterSource(const FilteredDropDownDialog& dialog);

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

	FilteredDropDownDialog* GetEventEntryDialog(const QString &str = "", AgxEventType iType = AgxEventType::UNDEFINED) const;
	MultiVariableDialog* GetFloatVariableEntryDialog(const QString &str = "") const;
	MultiVariableDialog* GetIntegerVariableEntryDialog(const QString &str = "") const;
	MultiVariableDialog* GetBooleanVariableEntryDialog(const QString &str = "") const;
	MultiVariableDialog* GetVectorVariableEntryDialog(const QString& str = "") const;
	MultiVariableDialog* GetPrefixDialog(const QString& str = "") const;
	MultiVariableDialog* GetSuffixDialog(const QString& str = "") const;
	MultiVariableDialog* GetActionVariableDialog(const QString& str = "") const;
	MultiVariableDialog* GetStateVariableDialog(const QString& str = "") const;
	MultiVariableDialog* GetSyncVariableDialog(const QString& str = "") const;
	BitfieldWidgetDialog* GetAnimationFlagDialog(size_t initialValue = 0) const;
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



