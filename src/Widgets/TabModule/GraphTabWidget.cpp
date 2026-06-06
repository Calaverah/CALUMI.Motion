// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFAMemoryLeak
#include "GraphTabWidget.h"

#include <QClipboard>
#include <QMimeData>
#include <QTimer>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <Widgets/Dialog/AgxProgressDialog.h>
#include <QFutureWatcher>

#include "Utilities/QWidgetFactories.h"
#include "Utilities/Settings/SettingsRegistry.h"
#include "CALUMIMotion.h"
#include "Application/CALUMIMotionApplication.h"
#include "Utilities/UndoRedoCommands.h"
#include "Utilities/AgxFormat.h"
#include "Widgets/SFBGS/SFBGS_GraphPropertiesDialogWidget.h"

#include <oclero/qlementine/icons/Icons16.hpp>

GraphTabWidget::GraphTabWidget(QWidget* parent) : ITabWidget(parent) {}

GraphTabWidget::~GraphTabWidget()
{
	SettingsRegistry::GetInstance().SetGraphSidebarWidth(m_localRightWidth);
}

AgxGraphicsView* GraphTabWidget::graph() const
{
    return m_graph;
}

void GraphTabWidget::setGraph(AgxGraphicsView* content)
{
	if (!content)
        return;

	m_graph = content;

    connect(&m_graph->agxNodeScene()->agxGraphModel(), &AgxGraphModel::nodeCreated, this, &GraphTabWidget::onNodeEstablished);
	connect(m_graph->agxNodeScene(), &AgxGraphicsScene::nodeGOSelected, this, [this](const AgxNodeId& id)
		{
			if (m_rightItems.contains(id))
			{
				//We use a timer to allow any hide events to occur first
				QTimer::singleShot(5, this, [this, id]
				{
					showRightItem(m_rightItems[id]);
				});

			}

			if (SettingsRegistry::GetInstance().GetGraphSidebarAutoHide() && !rightPanelVisible())
				onSetRightPanelVisible(true);

		});
	connect(m_graph->agxNodeScene(), &AgxGraphicsScene::nodeGODeselected, this, [this](const AgxNodeId& id)
		{
			if (const auto itm = m_rightItems[id])
				itm->setVisible(false);

			if (SettingsRegistry::GetInstance().GetGraphSidebarAutoHide() && rightPanelVisible() && m_rightWidget->childrenRect().isEmpty())
				onSetRightPanelVisible(false);
		});

	connect(&m_graph->agxNodeScene()->agxGraphModel(), &AgxGraphModel::nodeDeleted, this, [this](const AgxNodeId id)
		{
			if (m_rightItems.contains(id))
			{
				const auto obj = m_rightItems.take(id);
			   obj->deleteLater();
			}
		});


	m_localRightWidth = SettingsRegistry::GetInstance().GetGraphSidebarWidth();

    m_topLayout = new QVBoxLayout();
	m_centralLayout = new QSplitter(Qt::Horizontal);
	m_centralLayout->setHandleWidth(0);

	m_leftAreaParent = new QWidget();
	m_leftAreaParentLayout = new QVBoxLayout();
	m_leftAreaParent->setLayout(m_leftAreaParentLayout);
	m_leftAreaParent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_leftAreaParent->setContentsMargins(0, 0, 0, 0);
	m_leftAreaParentLayout->setContentsMargins(0, 0, 0, 0);
	m_rightScrollArea = new QScrollArea();
	m_rightScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	m_rightScrollArea->viewport()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	m_rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_rightScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_rightScrollArea->setWidgetResizable(true);

	Q_EMIT statusUpdate(0.05f, "Building Right Panel");

	//ResetSideBar_Right();
	m_rightScrollLayout = new QVBoxLayout();
	m_rightWidget = new QWidget();
	m_rightWidget->setLayout(m_rightScrollLayout);
	m_rightWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	m_rightScrollArea->setWidget(m_rightWidget);

	SetTransparentBackground(m_rightWidget);
	SetTransparentBackground(m_rightScrollArea);

	m_rightScrollLayout->addStretch(1);

	Q_EMIT statusUpdate(0.15f, "Creating Central Layout");

	m_centralLayout->addWidget(m_leftAreaParent);
	m_centralLayout->addWidget(m_graph);
	m_centralLayout->addWidget(m_rightScrollArea);

	m_centralLayout->setStretchFactor(0, 0);
	m_centralLayout->setStretchFactor(1, 2);
	m_centralLayout->setStretchFactor(2, 0);

	m_centralLayout->setSizes({0,m_centralLayout->width(),0});

	Q_EMIT statusUpdate(0.20, "Building Left Panel");

	setLeftItem(Q_NULLPTR);

	//Visibility
	onSetLeftPanelVisible(false);
	onSetRightPanelVisible(
		!SettingsRegistry::GetInstance().GetGraphSidebarAutoHide() ||
		!m_graph->agxNodeScene()->selectedNodes().empty()
		);

	m_leftCloseButton = new QPushButton("Close");
	m_leftAreaParentLayout->addWidget(m_leftCloseButton);
	connect(m_leftCloseButton, &QPushButton::pressed, this, [this]
	{
		this->onSetLeftPanelVisible(false);
	});

	Q_EMIT statusUpdate(0.25, "Reparenting Sidebar Items");

	m_leftAreaParent->setVisible(false);

	m_topLayout->addWidget(m_centralLayout);
	setLayout(m_topLayout);

	auto nodeIds = m_graph->agxNodeScene()->agxGraphModel().allNodeIds();
	const size_t contentCount = nodeIds.size();
	size_t progress = 0;

	for (const auto& existingNodeId : nodeIds)
	{
		onNodeEstablished(existingNodeId);
		Q_EMIT statusUpdate(static_cast<float>(0.25 + 0.75 * static_cast<double>(progress) / static_cast<double>(contentCount)));
		progress++;
	}
}

void GraphTabWidget::addRightItem(QWidget* item) const
{
	if (!item)
		return;

	if(item->parent() != m_rightScrollLayout)
	{
		const int idx = SettingsRegistry::GetInstance().GetGraphSelectedToTop() ? 0 : m_rightScrollLayout->count() - 1;
		m_rightScrollLayout->insertWidget(idx, item);
	}
}

void GraphTabWidget::showRightItem(QWidget* item, const bool moveToEnd) const
{
	if (!item)
		return;

	const bool isChild = item->parent() == m_rightWidget;
	const bool isShown = item->isVisible();

	if (isChild && isShown)
		return;

	if (!isShown)
		item->setVisible(true);

	if (moveToEnd && isChild)
	{
		m_rightScrollLayout->removeWidget(item);
		addRightItem(item);
	}
	else if (!isChild)
	{
		addRightItem(item);
	}
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void GraphTabWidget::hideRightItem(QWidget* item) const // NOLINT(*-convert-member-functions-to-static)
{
	item->setVisible(false);
}

bool GraphTabWidget::rightPanelVisible() const
{
	const auto sizes = m_centralLayout->sizes();
	return sizes.at(2) > 0;
}

void GraphTabWidget::setLeftItem(QWidget* item)
{
	if (!m_leftItem.isNull() || m_leftItem != Q_NULLPTR)
	{
		m_leftItem->deleteLater();
	}

	if (!item)
	{
		item = new QWidget(this);
		item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

	m_leftItem = item;
	m_leftAreaParentLayout->insertWidget(0, item);
}

bool GraphTabWidget::leftPanelVisible() const
{
	const auto sizes = m_centralLayout->sizes();

	return sizes.at(0) > 0 && m_leftAreaParent->isVisible();
}

void GraphTabWidget::onNodeEstablished(AgxNodeId nodeId)
{
	if (m_rightItems.contains(nodeId) && m_rightItems[nodeId] != Q_NULLPTR)
	{
		qWarning() << "GraphTabWidget::onNodeEstablished() Property Sheet Widget for NodeId (" << nodeId << ") already exists.";
		return;
	}

	if (const auto item = m_graph->agxNodeScene()->agxGraphModel().GetNodeSidebarContent(nodeId))
	{
		m_rightItems[nodeId] = item;

		if (const auto iAgx = dynamic_cast<AgxSidebarContent*>(item))
		{
			iAgx->SetRefData(nodeId, m_graph->agxNodeScene());
		}

		this->addRightItem(item);

		QTimer::singleShot(1, item, [this, nodeId, item]
		{
			item->setVisible(m_graph->agxNodeScene()->isNodeSelected(nodeId));
		});
	}

}

void GraphTabWidget::onSetRightPanelVisible(bool setVisible)
{
	if (!m_allowRightPanelVisible)
		setVisible = false;

	auto sizes = m_centralLayout->sizes();

	if (setVisible && sizes.at(2) == 0)
	{
		auto rightSize = m_localRightWidth;
		rightSize = rightSize == 0 ? qMax((m_centralLayout->width() - sizes.at(0)) / 3, 300) : rightSize;
		sizes[2] = rightSize;
	}
	else if (sizes.at(2) > 0)
	{
		m_localRightWidth = sizes.at(2);
		sizes[2] = 0;
	}

	sizes[1] = qMax(m_centralLayout->width() - sizes.at(0) - sizes.at(2), 0);
	m_centralLayout->setSizes(sizes);
}

void GraphTabWidget::onSetLeftPanelVisible(const bool setVisible) const
{
	auto sizes = m_centralLayout->sizes();
	m_leftAreaParent->setVisible(setVisible);

	if (setVisible && sizes.at(0) == 0)
	{
		const auto prefWidth = m_leftAreaParent->sizeHint().width();
		sizes[0] = prefWidth;
	}
	else if (sizes.at(0) > 0)
	{
		sizes[0] = 0;
	}

	sizes[1] = qMax(m_centralLayout->width() - sizes.at(0) - sizes.at(2), 0);
	m_centralLayout->setSizes(sizes);
}

void GraphTabWidget::buildMenus(CALUMIMotion* parent)
{
	if (!m_graph || !parent)
		return;

	const auto mView = ParentView(*parent);
	const auto mEdit = ParentEdit(*parent);
	// const auto mFile = ParentFile(*parent);
	// const auto mHelp = ParentHelp(*parent);

	//Center Menu
	{
		mView->addAction(m_graph->centerActionRef());
		m_actions.append(m_graph->centerActionRef());
	}

	//Sidebar Visibility
	{
		const auto sideVisAction = mView->addAction(tr("Disable Properties Panel"), QKeySequence(Qt::CTRL | Qt::Key_P));

		connect(sideVisAction, &QAction::triggered, this, [this, sideVisAction]
		{
			m_allowRightPanelVisible = !m_allowRightPanelVisible;
			if (m_allowRightPanelVisible)
			{
				sideVisAction->setText(tr("Disable Properties Panel"));
				onSetRightPanelVisible(
						!SettingsRegistry::GetInstance().GetGraphSidebarAutoHide() ||
						!m_graph->agxNodeScene()->selectedNodes().empty()
					);
			}
			else
			{
				sideVisAction->setText(tr("Enable Properties Panel"));
				onSetRightPanelVisible(false);
			}

		});

		connect(this, &QObject::destroyed, sideVisAction, &QAction::deleteLater);
		m_actions.append(sideVisAction);
	}

	//Show Node Group
	{
		const auto showNodeGroup = mView->addAction("Show Node Groups", QKeySequence(Qt::CTRL | Qt::Key_G));
		connect(showNodeGroup, &QAction::triggered, parent, &CALUMIMotion::ShowNodeGroupMenu);
		connect(this, &QObject::destroyed, showNodeGroup, &QAction::deleteLater);
		m_actions.append(showNodeGroup);
	}

	//Edit Menu - Undo Stack
	{
		mEdit->addAction(m_graph->undoActionRef());
		mEdit->addAction(m_graph->redoActionRef());
		m_actions.append(m_graph->undoActionRef());
		m_actions.append(m_graph->redoActionRef());

		auto undoStackMenu = mEdit->addMenu(tr("Undo Stack"));
		auto redoStackMenu = mEdit->addMenu(tr("Redo Stack"));
		m_actions.append(undoStackMenu->menuAction());
		m_actions.append(redoStackMenu->menuAction());

		connect(this, &QObject::destroyed, undoStackMenu, &QAction::deleteLater);
		connect(this, &QObject::destroyed, redoStackMenu, &QAction::deleteLater);


		connect(mEdit, &QMenu::aboutToShow, undoStackMenu, [this, undoStackMenu, mEdit]
		{
			for (int i = 0; i < m_graph->undoStackRef().index(); i++)
			{
				QString text = m_graph->undoStackRef().command(i)->text();
				QAction* menuItem = undoStackMenu->addAction(text);
				connect(menuItem, &QAction::triggered, m_graph, [this, i]
				{
					m_graph->undoStackRef().setIndex(i);
				});

				connect(mEdit, &QMenu::aboutToHide, menuItem, &QAction::deleteLater);

				if (i == m_graph->undoStackRef().index() - 1)
					menuItem->setIcon(QIcon::fromTheme("edit-undo"));
			}

			undoStackMenu->setEnabled(!undoStackMenu->actions().isEmpty());
		});

		connect(mEdit, &QMenu::aboutToShow, redoStackMenu, [this, redoStackMenu, mEdit]
		{
			for (int i = m_graph->undoStackRef().index(); i < m_graph->undoStackRef().count(); i++)
			{
				QString text = m_graph->undoStackRef().command(i)->text();
				QAction* menuItem = redoStackMenu->addAction(text);
				connect(menuItem, &QAction::triggered, m_graph, [this, i]
				{
					m_graph->undoStackRef().setIndex(i + 1);
				});

				connect(mEdit, &QMenu::aboutToHide, menuItem, &QAction::deleteLater);

				if (i == m_graph->undoStackRef().index())
					menuItem->setIcon(QIcon::fromTheme("edit-redo"));
			}

			redoStackMenu->setEnabled(!redoStackMenu->actions().isEmpty());
		});

		const auto sep = mEdit->addSeparator();
		m_actions.append(sep);
		connect(this, &QObject::destroyed, sep, &QAction::deleteLater);
	}

	//Edit Menu - CutCopyPasteDelete
	{
		const auto cutAction = m_graph->cutActionRef();
		const auto copyAction = m_graph->copyActionRef();
		const auto dupAction = m_graph->duplicateActionRef();
		const auto deleteAction = m_graph->deleteActionRef();
		const auto pasteAction = m_graph->pasteActionRef();

		mEdit->addAction(cutAction);
		mEdit->addAction(copyAction);
		mEdit->addAction(dupAction);
		mEdit->addAction(deleteAction);
		mEdit->addAction(pasteAction);
		const auto sep = mEdit->addSeparator();

		m_actions.append(cutAction);
		m_actions.append(copyAction);
		m_actions.append(dupAction);
		m_actions.append(deleteAction);
		m_actions.append(pasteAction);
		m_actions.append(sep);

		connect(this, &QAction::destroyed, sep, &QAction::deleteLater);

#define SELECTLOGIC if (m_graph->agxNodeScene()->selectedNodes().isEmpty())\
		{\
			cutAction->setEnabled(false);\
			copyAction->setEnabled(false);\
			dupAction->setEnabled(false);\
			deleteAction->setEnabled(false);\
		}\
		else\
		{\
			cutAction->setEnabled(true);\
			copyAction->setEnabled(true);\
			dupAction->setEnabled(true);\
			deleteAction->setEnabled(true);\
		}

#define SELECTSLOT [this, cutAction, copyAction, dupAction, deleteAction]\
		{\
		SELECTLOGIC\
		}

		connect(m_graph->agxNodeScene(), &AgxGraphicsScene::nodeGODeselected, this, SELECTSLOT);
		connect(m_graph->agxNodeScene(), &AgxGraphicsScene::nodeGOSelected, this, SELECTSLOT);
		SELECTLOGIC

#undef SELECTSLOT
#undef SELECTLOGIC

		connect(qApp->clipboard(), &QClipboard::dataChanged, this, [pasteAction]
		{
			if (qApp->clipboard()->mimeData()->hasFormat("application/qt-nodes-graph"))
				pasteAction->setEnabled(true);
			else
				pasteAction->setEnabled(false);
		});
	}

	//Edit Menu - Selection Filter
	{
		const auto selectAll = m_graph->selectAllActionRef();
		const auto selectAllNodes = m_graph->selectAllNodesActionRef();
		const auto selectAllConn = m_graph->selectAllConnectionsActionRef();
		const auto selectFilter = m_graph->selectionFilterMenu();

		mEdit->addAction(selectAll);
		mEdit->addAction(selectAllNodes);
		mEdit->addAction(selectAllConn);
		mEdit->addMenu(selectFilter);
		const auto sep = mEdit->addSeparator();

		m_actions.append(selectAll);
		m_actions.append(selectAllNodes);
		m_actions.append(selectAllConn);
		m_actions.append(selectFilter->menuAction());
		m_actions.append(sep);

		connect(this, &QAction::destroyed, sep, &QAction::deleteLater);
	}

	//Edit Menu - Graph Title
	{
		const auto newTitleAction = mEdit->addAction(tr("Edit Graph Title"));
		m_actions.append(newTitleAction);
		connect(this, &QObject::destroyed, newTitleAction, &QAction::deleteLater);
		connect(newTitleAction, &QAction::triggered, this, [this]
			{
				bool ok = false;
				QString result = QInputDialog::getText(this, tr("Input New Graph Title"),
									  tr("Graph Title:"), QLineEdit::Normal,
									  m_graph->agxNodeScene()->agxGraphModel().GetGraphTitle(), &ok);

				if (result.isEmpty() || !ok || m_graph->agxNodeScene()->agxGraphModel().GetGraphTitle().compare(result, Qt::CaseInsensitive) == 0)
					return;

				result = cleanFileName(result, false, true);
				m_graph->agxNodeScene()->undoStack().push(new AgxSetGraphTitleCommand(&m_graph->agxNodeScene()->agxGraphModel(), result));
			});
		const auto sep = mEdit->addSeparator();
		m_actions.append(sep);
		connect(this, &QObject::destroyed, sep, &QAction::deleteLater);
	}
}

void GraphTabWidget::onShowMenus() const
{
	for (const auto& action : m_actions)
	{
		if (action)
			action->setVisible(true);
	}
}

void GraphTabWidget::onHideMenus() const
{
	for (const auto& action : m_actions)
	{
		if (action)
			action->setVisible(false);
	}
}

bool GraphTabWidget::onLoadGraphFile(const QFileInfo& fileInfo)
{
	auto agxGraphModel = std::make_shared<AgxGraphModel>(AgxGameType::None);
	auto scene = std::make_shared<AgxGraphicsScene>(agxGraphModel);

	const auto progBar = new AgxProgressDialog(tr("Loading Graph File..."), "", 0, 1000, this);
	auto watcher = new QFutureWatcher<void>(this);
	connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
	connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
	connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
	connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
	connect(agxGraphModel.get(), &AgxGraphModel::statusUpdate, watcher, [watcher](const float loadPercentage, const QString& message) {
			Q_EMIT watcher->progressValueChanged(static_cast<int>(0.49 * loadPercentage * 1000));

			if (!message.isEmpty())
				Q_EMIT watcher->progressTextChanged(message);
	});
	progBar->show();

	QFile file(fileInfo.absoluteFilePath());
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		progBar->deleteLater();
		QMessageBox::critical(this, tr("Error"), tr("Could not open file: ") + fileInfo.fileName());
		return false;
	}

	const QByteArray byteArray = file.readAll();
	file.close();

	//Import as AGX "XML" Serialized File (Will need to check game type for future versions)
	if (fileInfo.suffix().compare("agx", Qt::CaseInsensitive) == 0)
	{
		bool ok = false;
		const auto result = QInputDialog::getItem(this, tr("Select Game Type"), tr("Game Type"), AgxGameTypeList(), 0, false, &ok);
		if (ok)
		{
			if (const auto gameType = AgxGameTypeFromString(result); gameType != AgxGameType::None)
			{
				agxGraphModel = std::make_shared<AgxGraphModel>(gameType);
				scene = std::make_shared<AgxGraphicsScene>(agxGraphModel);
			}
			else
				ok = false;
		}

		if (!ok)
		{
			progBar->deleteLater();
			return false;
		}

		pugi::xml_document doc;
		doc.load_string(byteArray.toStdString().c_str());

		if (!doc)
		{
			progBar->deleteLater();
			QMessageBox::critical(this, tr("Error"), tr("XML Parsing Failed"));
			return false;
		}

		if (!doc.child("root"))
		{
			if (const QMessageBox::StandardButton reply = QMessageBox::critical(nullptr, tr("File Content Warning"), tr("File is missing root and may not import properly..."), QMessageBox::Ok | QMessageBox::Abort); reply == QMessageBox::Abort)
			{
				progBar->deleteLater();
				return false;
			}
		}

		auto graphNode = doc.child("root");
		agxGraphModel->load(graphNode);
	}
	//Import as JSON Serialized File
	else if (fileInfo.suffix().compare("jagx", Qt::CaseInsensitive) == 0)
	{
		QJsonParseError parseError;
		const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);

		if (parseError.error != QJsonParseError::NoError) {
			progBar->deleteLater();
			QMessageBox::critical(this, tr("Error"), tr("Json Parsing Failed: ") + parseError.errorString());
			return false;
		}

		if (!doc.isObject())
		{
			progBar->deleteLater();
			return false;
		}

		const QJsonObject obj = doc.object();

		const auto gameTypeString = obj.value("game-type").toString();
		const auto gameType = AgxGameTypeFromString(gameTypeString);

		if (gameType == AgxGameType::None)
		{
			progBar->deleteLater();
			return false;
		}

		agxGraphModel = std::make_shared<AgxGraphModel>(gameType);
		scene = std::make_shared<AgxGraphicsScene>(agxGraphModel);
		agxGraphModel->load(obj);
	}
	else
	{
		progBar->deleteLater();
		return false;
	}

	Q_EMIT watcher->progressValueChanged(495);
	Q_EMIT watcher->progressTextChanged(tr("Processing Scene"));
	scene->update();

	Q_EMIT watcher->progressValueChanged(500);
	Q_EMIT watcher->progressTextChanged(tr("Loading View"));
	const auto newTabView = new AgxGraphicsView(scene.get());

	const auto connection = connect(this, &GraphTabWidget::statusUpdate, this, [watcher](const float loadPercentage, const QString& message)
	{
		Q_EMIT watcher->progressValueChanged(500 + static_cast<int>(loadPercentage * 475));

		if (!message.isEmpty())
			Q_EMIT watcher->progressTextChanged(message);
	});
	setGraph(newTabView);
	disconnect(connection);

	Q_EMIT watcher->progressValueChanged(975);
	Q_EMIT watcher->progressTextChanged(tr("Finalizing View"));

	if (const auto toolbar = newTabView->getToolBarLayout())
	{
		const auto propButton = new QPushButton();
		{
			const auto iconPath = oclero::qlementine::icons::iconPath(oclero::qlementine::icons::Icons16::Navigation_MenuBurger);
			const auto ico = GetColoredIconFromSVG(iconPath);
			propButton->setIcon(ico);
		}

		propButton->setFixedSize(QSize(48, 48));
		toolbar->addWidget(propButton);

		connect(propButton, &QPushButton::pressed, this, [this]
			{
				onSetLeftPanelVisible(!leftPanelVisible());
			});

		connect(agxGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [scene, this]
			{
				setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));
			});

		setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));

	}

	Q_EMIT watcher->progressValueChanged(1000);
	Q_EMIT watcher->progressTextChanged(tr("Finished!"));

	newTabView->setFocus();

	return true;
}

QString GraphTabWidget::tabTitle() const
{
	if (m_graph)
		return m_graph->agxNodeScene()->agxGraphModel().GetGraphTitle(false);

	return "untitled graph tab";
}

QColor GraphTabWidget::tabTitleColor() const
{
	if (m_graph)
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto root = m_graph->agxNodeScene()->agxGraphModel().rootGraphReference();

		if (root != &m_graph->agxNodeScene()->agxGraphModel())
			return qApp->palette().color(QPalette::PlaceholderText);
	}

	return qApp->palette().color(QPalette::Text);
}
