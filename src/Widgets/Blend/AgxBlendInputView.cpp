//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFAMemoryLeak
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
	QTableView::setModel(model);
	resizeRowsToContents();
	setSizeAdjustPolicy(AdjustToContents);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setMinimumWidth(200);
	setSelectionMode(NoSelection);
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

	const bool valid = indexAt(pos).isValid();
	size_t index;

	if (!valid) {
		if (const auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
			index = modelRef->getDataRowCount();
		}
		else
			index = model()->rowCount();
	}
	else
	{
		index = indexAt(pos).row();
	}

	const auto menu = new QMenu(this);

	if(valid) {
		const QAction* removeRowAction = menu->addAction("Remove Row");
		connect(removeRowAction, &QAction::triggered, this, [this, index] {
					if (const auto modelRef = dynamic_cast<AgxBlendInputModel*>(model()))
					{
						if (m_scene && modelRef->_port)
						{
							QJsonObject data;
							data["command-text"] = "Remove Blend Point";
							data["command-undo"] = "blend-input-insert";
							data["command-redo"] = "blend-input-remove";
							data["command-payload"] = std::to_string(index).c_str();
							m_scene->undoStack().push(new AgxPortCommandPayloadCommand(m_scene,m_nodeId,AgxPortType::In,modelRef->_port->getPortIndex(), data));
						}
					}
				});
	}

	const QString insertString = valid ? "Insert Row Above" : "Insert Row";
	const QAction* insertRowAboveAction = menu->addAction(insertString);
	connect(insertRowAboveAction, &QAction::triggered, this, [this, index] {
				if (const auto modelRef = dynamic_cast<AgxBlendInputModel*>(model())) {
					if (m_scene && modelRef->_port) {
						QJsonObject data;
						data["command-text"] = "Insert Blend Point";
						data["command-undo"] = "blend-input-remove";
						data["command-redo"] = "blend-input-insert";
						data["command-payload"] = std::to_string(index).c_str();
						m_scene->undoStack().push(new AgxPortCommandPayloadCommand(m_scene, m_nodeId, AgxPortType::In, modelRef->_port->getPortIndex(), data));
					}
				}
			});

	if(valid) {
		const QAction* insertRowBelowAction = menu->addAction("Insert Row Below");
		connect(insertRowBelowAction, &QAction::triggered, this, [this, index] {
					if (const auto modelRef = dynamic_cast<AgxBlendInputModel*>(model()))
					{
						if (m_scene && modelRef->_port)
						{
							QJsonObject data;
							data["command-text"] = "Remove Blend Point";
							data["command-undo"] = "blend-input-remove";
							data["command-redo"] = "blend-input-insert";
							data["command-payload"] = std::to_string(index+1).c_str();
							m_scene->undoStack().push(new AgxPortCommandPayloadCommand(m_scene, m_nodeId, AgxPortType::In, modelRef->_port->getPortIndex(), data));
						}
					}
				});
	}

	menu->popup(viewport()->mapToGlobal(pos));

	connect(menu, &QMenu::aboutToHide, [this]
		{
			clearSelection();
		});

	menu->setAttribute(Qt::WA_DeleteOnClose);

}

