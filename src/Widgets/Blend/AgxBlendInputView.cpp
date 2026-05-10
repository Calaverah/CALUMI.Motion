//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxBlendInputView.h"
#include "Utilities/UndoRedoCommands.h"
#include "Models/Blend/AgxBlendInputModel.h"
#include "Models/AgxGraphicsScene.h"
#include <Utilities/AgxJsonHelper.h>

AgxBlendInputView::AgxBlendInputView(QWidget* parent) : QTableView(parent) {
	setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, &QTableView::customContextMenuRequested, this, &AgxBlendInputView::OnContextMenuRequested);
}

AgxBlendInputView::AgxBlendInputView(AgxBlendInputModel* model, QWidget* parent) : AgxBlendInputView(parent)
{
	setModel(model);
	resizeRowsToContents();
	setSizeAdjustPolicy(QAbstractItemView::AdjustToContents);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	//horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setMinimumWidth(200);
	setSelectionMode(QAbstractItemView::NoSelection);
	horizontalHeader()->setVisible(true);

	connect(model, &AgxBlendInputModel::DataChangeRequest, this, [this](const QString& row, const QString& type, const QString& value) {
				QStringList path = _bastPath;
				path.append(row);
				path.append(type);
				SendInsertPropertySheetDataCommand(QStringListToQJsonObject(path, value));
			});
}

void AgxBlendInputView::SetBasePath(const QStringList& path)
{
	_bastPath = path;
}

void AgxBlendInputView::OnContextMenuRequested(const QPoint& pos) {

	bool valid = indexAt(pos).isValid();
	size_t index;

	if (!valid) {
		if (auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
			index = modelRef->getDataRowCount();
		}
	} else {
		index = indexAt(pos).row();
	}

	QMenu* menu = new QMenu(this);

	if(valid) {
		QAction* removeRowAction = menu->addAction("Remove Row");
		connect(removeRowAction, &QAction::triggered, this, [this, index]() {
					if (auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
						if (_scene && modelRef->_port) {
							QJsonObject data;
							data["command-text"] = "Remove Blend Point";
							data["command-undo"] = "blend-input-insert";
							data["command-redo"] = "blend-input-remove";
							data["command-payload"] = std::to_string(index).c_str();
							_scene->undoStack().push(new AgxPortCommandPayloadCommand(_scene,_nodeId,AgxPortType::In,modelRef->_port->GetPortIndex(), data));
						}
					}
				});
	}

	QString insertString = valid ? "Insert Row Above" : "Insert Row";
	QAction* insertRowAboveAction = menu->addAction(insertString);
	connect(insertRowAboveAction, &QAction::triggered, this, [this, index]() {
				if (auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
					if (_scene && modelRef->_port) {
						QJsonObject data;
						data["command-text"] = "Insert Blend Point";
						data["command-undo"] = "blend-input-remove";
						data["command-redo"] = "blend-input-insert";
						data["command-payload"] = std::to_string(index).c_str();
						_scene->undoStack().push(new AgxPortCommandPayloadCommand(_scene, _nodeId, AgxPortType::In, modelRef->_port->GetPortIndex(), data));
					}
				}
			});

	if(valid) {
		QAction* insertRowBelowAction = menu->addAction("Insert Row Below");
		connect(insertRowBelowAction, &QAction::triggered, this, [this, index]() {
					if (auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
						if (_scene && modelRef->_port) {
							QJsonObject data;
							data["command-text"] = "Remove Blend Point";
							data["command-undo"] = "blend-input-remove";
							data["command-redo"] = "blend-input-insert";
							data["command-payload"] = std::to_string(index+1).c_str();
							_scene->undoStack().push(new AgxPortCommandPayloadCommand(_scene, _nodeId, AgxPortType::In, modelRef->_port->GetPortIndex(), data));
						}
					}
				});
	}

	menu->popup(viewport()->mapToGlobal(pos));

	connect(menu, &QMenu::aboutToHide, [this, menu]() {
				clearSelection();
			});

	menu->setAttribute(Qt::WA_DeleteOnClose);

}

