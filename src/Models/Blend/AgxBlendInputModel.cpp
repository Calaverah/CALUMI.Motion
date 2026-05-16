//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxBlendInputModel.h"
#include "Models/AgxGraphicsScene.h"
#include <Models/AgxPort.h>
#include <QAbstractTableModel>

AgxBlendInputModel::AgxBlendInputModel(AgxPort* port, QObject* parent) : QStandardItemModel(parent), _port(port)
{
	setColumnCount(2);
	QStandardItemModel::setHeaderData(0, Qt::Horizontal, "Range");
	QStandardItemModel::setHeaderData(1, Qt::Horizontal, "Weight");
}

QString AgxBlendInputModel::getRangeValue(const int row) const
{
	return item(row,0)->data(Qt::DisplayRole).toString();
}

QString AgxBlendInputModel::getWeightValue(const int row) const
{
	return item(row, 1)->data(Qt::DisplayRole).toString();
}

int AgxBlendInputModel::getDataRowCount() const
{
	return rowCount();
}

void AgxBlendInputModel::modifyRangeValue(const int row, const QString& value) const
{
	item(row, 0)->setData(value, Qt::DisplayRole);
}

void AgxBlendInputModel::modifyWeightValue(const int row, const QString& value) const
{
	item(row, 1)->setData(value, Qt::DisplayRole);
}

void AgxBlendInputModel::addDataRow(const int row, const QString& rng, const QString& wgt)
{
	insertRow(row);

	setItem(row, 0, new QStandardItem(rng));
	setItem(row, 1, new QStandardItem(wgt));

}

void AgxBlendInputModel::removeDataRow(const int row)
{
	removeRow(row);
}

bool AgxBlendInputModel::setData(const QModelIndex& index, const QVariant& value, const int role)
{
	if (role == Qt::EditRole)
	{
		if (const auto old = data(index, Qt::EditRole); old != value.toString())
		{
			const QString type = index.column() == 0 ? "range" : "weight";
			Q_EMIT DataChangeRequest(std::to_string(index.row()).c_str(), type, value.toString());
		}
	}

	return false;
}
