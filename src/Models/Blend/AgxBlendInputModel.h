//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include "Utilities/AgxDefinitions.h"
#include <QStandardItemModel>

class AgxGraphicsScene;
class AgxPort;

class AgxBlendInputModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	explicit AgxBlendInputModel(AgxPort* port, QObject *parent = nullptr);

	QString getRangeValue(int row) const;
	QString getWeightValue(int row) const;

	[[nodiscard]] int getDataRowCount() const;

private:
	void modifyRangeValue(int row, const QString& value) const;
	void modifyWeightValue(int row, const QString& value) const;
	//void addDataRow(int row);
	void addDataRow(int row, const QString& rng = "0.0", const QString& wgt = "0.0");
	void removeDataRow(int row);

	//overrides
public:
	//We override to use undo/redo instead
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;

signals:
	void DataChangeRequest(const QString& row, const QString& type, const QString& value);

private:
	AgxPort* _port;

public:
friend class SFBGSNode;
friend class AgxPort_SFBGS;
friend class AgxBlendInputView;
};

