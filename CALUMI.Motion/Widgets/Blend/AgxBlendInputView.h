#pragma once

#include <QTableView>
#include <Widgets/IAgxEmbedSceneData.h>

class AgxGraphicsScene;
class AgxBlendInputModel;

class AgxBlendInputView  : public QTableView, public IAgxEmbedSceneData
{
	Q_OBJECT

public:
	AgxBlendInputView(QWidget* parent = nullptr);
	AgxBlendInputView(AgxBlendInputModel* model, QWidget* parent = nullptr);

	void SetBasePath(const QStringList& path);

public slots:
	void OnContextMenuRequested(const QPoint& pos);

private:
	QStringList _bastPath;

};

