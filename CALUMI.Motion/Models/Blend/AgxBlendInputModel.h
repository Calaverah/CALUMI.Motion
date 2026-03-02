#pragma once

#include <QAbstractTableModel>
#include <Widgets/IAgxEmbedSceneData.h>
#include "Utilities/AgxDefinitions.h"
//#include "Utilities/UndoRedoCommands.h"

class AgxGraphicsScene;
class AgxPort;

class AgxBlendInputModel  : public QStandardItemModel
{
	Q_OBJECT

public:
	AgxBlendInputModel(AgxPort* id, QObject *parent = nullptr);

	QString getRangeValue(int row);
	QString getWeightValue(int row);

	size_t getDataRowCount() const;

private:
	void modifyRangeValue(int row, QString value);
	void modifyWeightValue(int row, QString value);
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

