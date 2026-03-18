//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

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

