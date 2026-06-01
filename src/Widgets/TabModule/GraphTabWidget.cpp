// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFAMemoryLeak
#include "GraphTabWidget.h"

#include <QTimer>
#include <QVBoxLayout>

#include "Utilities/QWidgetFactories.h"
#include "Utilities/Settings/SettingsRegistry.h"
#include "CALUMIMotion.h"
#include "Application/CALUMIMotionApplication.h"

GraphTabWidget::GraphTabWidget(AgxGraphicsView* content, QWidget* parent) : ITabWidget(parent), m_graph(content)
{
    if (!content)
        return;

    connect(&content->agxNodeScene()->agxGraphModel(), &AgxGraphModel::nodeCreated, this, &GraphTabWidget::onNodeEstablished);
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
		!content->agxNodeScene()->selectedNodes().empty()
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
		Q_EMIT statusUpdate(0.25 + 0.75 * progress / contentCount);
		progress++;
	}
}

GraphTabWidget::~GraphTabWidget()
{
	SettingsRegistry::GetInstance().SetGraphSidebarWidth(m_localRightWidth);
}

AgxGraphicsView* GraphTabWidget::graph() const
{
    return m_graph;
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
void GraphTabWidget::hideRightItem(QWidget* item) const
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

		QTimer::singleShot(1, this, [this, nodeId, item]
		{
			item->setVisible(m_graph->agxNodeScene()->isNodeSelected(nodeId));
		});

		connect(&m_graph->agxNodeScene()->agxGraphModel(), &AgxGraphModel::nodeDeleted, this, [this, item](const AgxNodeId id)
		{
			this->m_rightItems.remove(id);
			item->deleteLater();
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
	// const auto mEdit = ParentEdit(*parent);
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
}

void GraphTabWidget::onShowMenus() const
{
	for (const auto& action : m_actions)
		action->setVisible(true);
}

void GraphTabWidget::onHideMenus() const
{
	for (const auto& action : m_actions)
		action->setVisible(false);
}
