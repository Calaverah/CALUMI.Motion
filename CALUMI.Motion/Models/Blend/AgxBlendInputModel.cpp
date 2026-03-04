#include "stdafx.h"
#include "AgxBlendInputModel.h"
#include "Models/AgxGraphicsScene.h"
#include "Utilities/UndoRedoCommands.h"
#include <Models/AgxPort.h>

AgxBlendInputModel::AgxBlendInputModel(AgxPort* port, QObject* parent) : QStandardItemModel(parent), _port(port)
{
	setColumnCount(2);
	setHeaderData(0, Qt::Horizontal, "Range");
	setHeaderData(1, Qt::Horizontal, "Weight");
}

QString AgxBlendInputModel::getRangeValue(int row)
{
	return item(row,0)->data(Qt::DisplayRole).toString();
}

QString AgxBlendInputModel::getWeightValue(int row)
{
	return item(row, 1)->data(Qt::DisplayRole).toString();
}

int AgxBlendInputModel::getDataRowCount() const
{
	return rowCount();
}

void AgxBlendInputModel::modifyRangeValue(int row, QString value)
{
	item(row, 0)->setData(value, Qt::DisplayRole);
}

void AgxBlendInputModel::modifyWeightValue(int row, QString value)
{
	item(row, 1)->setData(value, Qt::DisplayRole);
}

//void AgxBlendInputModel::addDataRow(int row)
//{
//	insertRow(row);
//
//	setItem(row, 0, new QStandardItem("0.0"));
//	setItem(row, 1, new QStandardItem("0.0"));
//}

void AgxBlendInputModel::addDataRow(int row, const QString& rng, const QString& wgt)
{
	insertRow(row);

	setItem(row, 0, new QStandardItem(rng));
	setItem(row, 1, new QStandardItem(wgt));

}

void AgxBlendInputModel::removeDataRow(int row)
{
	removeRow(row);
}

bool AgxBlendInputModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (role == Qt::EditRole) {
		auto old = data(index, Qt::EditRole);
		if (old != value.toString()) {
			QString type = index.column() == 0 ? "range" : "weight";
			Q_EMIT DataChangeRequest(std::to_string(index.row()).c_str(), type, value.toString());
		}
	}

	return false;
}
