// ReSharper disable CppTooWideScopeInitStatement
#include "stdafx.h"
#include "DialogPool.h"
#include <QObject>
#include "Utilities/AgxDefinitions.h"
#include "Widgets/FilteredDropDownDialog.h"
#include <QMap>

AgxEventFilterProxyModel::AgxEventFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    QSortFilterProxyModel::sort(0);
}

//Cannot be const method!!
void AgxEventFilterProxyModel::setFilterSource(const FilteredDropDownDialog& dialog)
{
    dialog.GetFilterBox()->connect(dialog.GetFilterBox(), &QComboBox::currentTextChanged, this,
                                   &AgxEventFilterProxyModel::FilterChange);
}

void AgxEventFilterProxyModel::FilterChange(const QString& str)
{
    beginFilterChange();
    const auto resp = AgxEventTypeMap.contains(str) ? AgxEventTypeMap[str] : AgxEventType::UNDEFINED;
    m_type = resp;
    endFilterChange();
}

bool AgxEventFilterProxyModel::filterAcceptsRow(const int sourceRow, const QModelIndex& sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    const AgxEventType checkType = sourceModel()->data(index, Qt::UserRole + 1).value<AgxEventInfo>().eventType;

    if (m_type == checkType)
        return true;

    switch (m_type)
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
    const QVariant leftData = sourceModel()->data(left, sortRole());
    const QVariant rightData = sourceModel()->data(right, sortRole());

    if (leftData.userType() == QMetaType::QString && rightData.userType() == QMetaType::QString)
    {
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
        m_eventModel = new QStandardItemModel();

        for (auto entry = AgxEventVars->cbegin(); entry != AgxEventVars->cend(); ++entry)
        {
            // ReSharper disable once CppDFAMemoryLeak
            const auto item = new QStandardItem(entry.key());
            QVariant var;
            var.setValue(entry.value());
            item->setData(var, Qt::UserRole + 1);
            m_eventModel->appendRow(item);
        }

        m_EventEntryDialog = new FilteredDropDownDialog();


        // ReSharper disable once CppDFAMemoryLeak
        const auto _eventModelFilter = new AgxEventFilterProxyModel(m_EventEntryDialog);
        _eventModelFilter->setSourceModel(m_eventModel);


        _eventModelFilter->setFilterSource(*m_EventEntryDialog);
        m_EventEntryDialog->GetMainComboBox()->setModel(_eventModelFilter);


        m_EventEntryDialog->setWindowTitle("Select Event");

        m_EventEntryDialog->GetFilterBox()->addItem("All");
        m_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Graph_Event));
        m_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Game_Event));
        m_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Both));
        m_EventEntryDialog->GetFilterBox()->addItem(AgxEventTypeMap.key(AgxEventType::Special));
    }

    {
        m_FloatEntryDialog = new MultiVariableDialog();
        m_FloatEntryDialog->setWindowTitle("Select Float Type Variable");
        m_FloatEntryDialog->SetCustomFloat();
        m_FloatEntryDialog->GetComboBox()->addItems(AgxFloatVars);
    }

    {
        m_IntegerEntryDialog = new MultiVariableDialog();
        m_IntegerEntryDialog->setWindowTitle("Select Integer Type Variable");
        m_IntegerEntryDialog->SetCustomInteger();
        m_IntegerEntryDialog->GetComboBox()->addItems(AgxIntegerVars);
    }

    {
        m_BooleanEntryDialog = new MultiVariableDialog();
        m_BooleanEntryDialog->setWindowTitle("Select Boolean Type Variable");
        m_BooleanEntryDialog->SetCustomBoolean();
        m_BooleanEntryDialog->GetComboBox()->addItems(AgxBooleanVars);
    }

    {
        m_VectorEntryDialog = new MultiVariableDialog();
        m_VectorEntryDialog->setWindowTitle("Select Vector Type Variable");
        m_VectorEntryDialog->SetCustomVector();
        m_VectorEntryDialog->GetComboBox()->addItems(AgxVectorVars);
    }

    {
        m_PrefixDialog = new MultiVariableDialog();
        m_PrefixDialog->setWindowTitle("Select Prefix");
        m_PrefixDialog->SetCustomString();
        m_PrefixDialog->GetComboBox()->addItems(AgxAnimPrefixes);
    }

    {
        m_SuffixDialog = new MultiVariableDialog();
        m_SuffixDialog->setWindowTitle("Select Suffix");
        m_SuffixDialog->SetCustomString();
        m_SuffixDialog->GetComboBox()->addItems(AgxAnimSuffixes);
    }
    {
        m_ActionVarDialog = new MultiVariableDialog();
        m_ActionVarDialog->setWindowTitle("Select Action");
        m_ActionVarDialog->SetCustomString();
        m_ActionVarDialog->GetComboBox()->addItems(AgxActionVars);
    }
    {
        m_StateVarDialog = new MultiVariableDialog();
        m_StateVarDialog->setWindowTitle("Select State");
        m_StateVarDialog->SetCustomString();
        m_StateVarDialog->GetComboBox()->addItems(AgxStateVars);
    }
    {
        m_SyncDialog = new MultiVariableDialog();
        m_SyncDialog->setWindowTitle("Select Sync System");
        m_SyncDialog->SetCustomString();
        m_SyncDialog->GetComboBox()->addItems(AgxSyncs);
    }
}

DialogPool_SFBGS::~DialogPool_SFBGS()
{
    //deletion will be handled for us either by QT or windows releasing memory

    qDebug() << "SFBGS Dialog Pool Destroyed\n";
    if (m_EventEntryDialog)
    {
        qDebug() << "_EventEntryDialog Attempt\n";
        //delete _EventEntryDialog;
        m_EventEntryDialog = nullptr;
        qDebug() << "_EventEntryDialog Destroyed\n";
    }
    if (m_FloatEntryDialog)
    {
        qDebug() << "_FloatEntryDialog Attempt\n";
        //delete _FloatEntryDialog;
        m_FloatEntryDialog = nullptr;
        qDebug() << "_FloatEntryDialog Destroyed\n";
    }
    if (m_IntegerEntryDialog)
    {
        qDebug() << "_IntegerEntryDialog Attempt\n";
        //delete _IntegerEntryDialog;
        m_IntegerEntryDialog = nullptr;
        qDebug() << "_IntegerEntryDialog Destroyed\n";
    }
    if (m_BooleanEntryDialog)
    {
        qDebug() << "_BooleanEntryDialog Attempt\n";
        //delete _BooleanEntryDialog;
        m_BooleanEntryDialog = nullptr;
        qDebug() << "_BooleanEntryDialog Destroyed\n";
    }
    if (m_VectorEntryDialog)
    {
        qDebug() << "_VectorEntryDialog Attempt\n";
        //delete _VectorEntryDialog;
        m_VectorEntryDialog = nullptr;
        qDebug() << "_VectorEntryDialog Destroyed\n";
    }
    if (m_PrefixDialog)
    {
        qDebug() << "_PrefixDialog Attempt\n";
        //delete _PrefixDialog;
        m_PrefixDialog = nullptr;
        qDebug() << "_PrefixDialog Destroyed\n";
    }
    if (m_SuffixDialog)
    {
        qDebug() << "_SuffixDialog Attempt\n";
        //delete _SuffixDialog;
        m_SuffixDialog = nullptr;
        qDebug() << "_SuffixDialog Destroyed\n";
    }
    if (m_ActionVarDialog)
    {
        qDebug() << "_ActionVarDialog Attempt\n";
        //delete _ActionVarDialog;
        m_ActionVarDialog = nullptr;
        qDebug() << "_ActionVarDialog Destroyed\n";
    }
    if (m_StateVarDialog)
    {
        qDebug() << "_StateVarDialog Attempt\n";
        //delete _StateVarDialog;
        m_StateVarDialog = nullptr;
        qDebug() << "_StateVarDialog Destroyed\n";
    }
    if (m_SyncDialog)
    {
        qDebug() << "_SyncDialog Attempt\n";
        //delete _SyncDialog;
        m_SyncDialog = nullptr;
        qDebug() << "_SyncDialog Destroyed\n";
    }
}

FilteredDropDownDialog* DialogPool_SFBGS::GetEventEntryDialog(const QString& str, AgxEventType iType) const
{
    const int idx = static_cast<int>(iType);

    m_EventEntryDialog->GetFilterBox()->setCurrentIndex(idx);
    m_EventEntryDialog->SetCustomHolder(str);

    const auto idxDrop = m_EventEntryDialog->GetMainComboBox()->findText(str);
    auto idxCustom = m_EventEntryDialog->GetMainComboBox()->findText("<custom>");

    //until we work out the translations during runtimes for events, this will have to be done to prevent any awkward bugs
    if (idxCustom < 0)
        idxCustom = m_EventEntryDialog->GetMainComboBox()->findText(QObject::tr("<custom>"));

    m_EventEntryDialog->GetMainComboBox()->setCurrentIndex(0);

    if (!str.isEmpty())
    {
        if (idxDrop >= 0)
        {
            m_EventEntryDialog->GetMainComboBox()->setCurrentIndex(idxDrop);
        }
        else if (idxCustom >= 0)
        {
            m_EventEntryDialog->GetMainComboBox()->setCurrentIndex(idxCustom);
            //_EventEntryDialog->SetCustomHolder(str);
        }
    }

    return m_EventEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetFloatVariableEntryDialog(const QString& str) const
{
    m_FloatEntryDialog->SetInitialFloat(str);

    return m_FloatEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetIntegerVariableEntryDialog(const QString& str) const
{
    m_IntegerEntryDialog->SetInitialInteger(str);

    return m_IntegerEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetBooleanVariableEntryDialog(const QString& str) const
{
    m_BooleanEntryDialog->SetInitialBoolen(str);

    return m_BooleanEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetVectorVariableEntryDialog(const QString& str) const
{
    m_VectorEntryDialog->SetInitialVector(str);

    return m_VectorEntryDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetPrefixDialog(const QString& str) const
{
    m_PrefixDialog->SetInitialString(str);

    return m_PrefixDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetSuffixDialog(const QString& str) const
{
    m_SuffixDialog->SetInitialString(str);

    return m_SuffixDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetActionVariableDialog(const QString& str) const
{
    m_ActionVarDialog->SetInitialString(str);

    return m_ActionVarDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetStateVariableDialog(const QString& str) const
{
    m_StateVarDialog->SetInitialString(str);

    return m_StateVarDialog;
}

MultiVariableDialog* DialogPool_SFBGS::GetSyncVariableDialog(const QString& str) const
{
    m_SyncDialog->SetInitialString(str);

    return m_SyncDialog;
}

BitfieldWidgetDialog* DialogPool_SFBGS::GetAnimationFlagDialog(const size_t initialValue, QWidget* parent)
{
    const auto output = new BitfieldWidgetDialog(parent);

    output->setWindowTitle("Select Animation Flags");
    output->AddCheckBoxes(AgxAnimationFlags::GetStringList());
    output->SetInitialValues(initialValue);

    return output;
}
