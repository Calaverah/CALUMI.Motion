//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "CALUMITabModule.h"
#include "AgxGraphicsView.h"
#include "Models/AgxGraphicsScene.h"
#include <Interfaces/IButtonBox.h>
#include <Widgets/AgxSidebarContent.h>

CALUMITabModule::CALUMITabModule(QWidget* content, QFutureWatcher<void>* watcher, int current, int progressShare, QWidget* parent) : QWidget(parent), _mainWidget(content)
{
	if (watcher) {
		auto connection = connect(this, &CALUMITabModule::statusUpdate, watcher, [this, progressShare, current, watcher](float loadPercentage, const QString& message) {
			auto maxValue = watcher->progressMaximum();
			Q_EMIT watcher->progressValueChanged(current + static_cast<int>(loadPercentage * progressShare));
			if (!message.isEmpty()) Q_EMIT watcher->progressTextChanged(message);
								  });
		CALUMITabModuleConstructorHelper(content, parent);
		disconnect(connection);

	} 
	else 
	{
		CALUMITabModuleConstructorHelper(content, parent);
	}
}

void CALUMITabModule::CALUMITabModuleConstructorHelper(QWidget* content, QWidget* parent)
{
	_topLayout = new QVBoxLayout();
	_centralLayout = new QSplitter(Qt::Horizontal);
	_centralLayout->setHandleWidth(0);

	_leftAreaParent = new QWidget();
	_leftAreaParentLayout = new QVBoxLayout();
	_leftAreaParent->setLayout(_leftAreaParentLayout);
	_leftAreaParent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	_leftAreaParent->setContentsMargins(0, 0, 0, 0);
	_leftAreaParentLayout->setContentsMargins(0, 0, 0, 0);
	_rightScrollArea = new QScrollArea();
	_rightScrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	_rightScrollArea->viewport()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	_rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_rightScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_rightScrollArea->setWidgetResizable(true);

	Q_EMIT statusUpdate(0.05f, "Building Property Sidebar");
	//QApplication::processEvents();

	ResetSideBar_Right();

	Q_EMIT statusUpdate(0.15f, "Creating Tab Widgets");
	//QApplication::processEvents();

	_centralLayout->addWidget(_leftAreaParent);
	_centralLayout->addWidget(_mainWidget);
	_centralLayout->addWidget(_rightScrollArea);

	_centralLayout->setStretchFactor(0, 0);
	_centralLayout->setStretchFactor(1, 2);
	_centralLayout->setStretchFactor(2, 0);
	
	//auto size = _centralLayout->width();
	//auto centerSize = size - 271;
	//_centralLayout->setSizes({0,1000,271});
	_centralLayout->setSizes({0,1000,0});
	
	Q_EMIT statusUpdate(0.25, "Reparenting Sidebar Items");
	//QApplication::processEvents();

	_leftAreaParent->setVisible(false);

	//_topLayout->addWidget(_toolBar);
	_topLayout->addWidget(_centralLayout);
	setLayout(_topLayout);

	if (auto agxView = dynamic_cast<AgxGraphicsView*>(_mainWidget))
	{
		connect(agxView, &AgxGraphicsView::broadcastSidebarItem, this, &CALUMITabModule::AddSideBarItem_Right);

		auto nodeIds = agxView->pagxNodeScene()->agxGraphModel().allNodeIds();
		size_t contentCount = nodeIds.size();
		size_t progress = 0;

		for (auto existingNodeId : nodeIds) {
			auto sidebar = agxView->pagxNodeScene()->agxGraphModel().GetNodeSidebarContent(existingNodeId);
			if (auto iagx = dynamic_cast<AgxSidebarContent*>(sidebar)) {
				iagx->SetRefData(existingNodeId, agxView->pagxNodeScene());
			}
			if(sidebar){
			sidebar->setVisible(false);
			AddSideBarItem_Right(sidebar);
			}
			Q_EMIT statusUpdate(0.25 + (0.75 * progress) / contentCount);
			//QApplication::processEvents();
			progress++;
		}
	}


}

QWidget* CALUMITabModule::GetMainWidget() const
{
	return _mainWidget;
}

void CALUMITabModule::AddSideBarItem_Right(QWidget* item)
{	
	if (!item) return;

	if(item->parent()!=_rightScrollLayout)
	{
		_rightScrollLayout->insertWidget(0, item);

		if (auto sfbgsItem = dynamic_cast<AgxSidebarContent*>(item))
		{
			connect(sfbgsItem, &AgxSidebarContent::StateChanged, this, &CALUMITabModule::AdjustSize_Right);

		}

		connect(item, &QWidget::destroyed, this, [this](){
			QTimer::singleShot(1, this, &CALUMITabModule::AdjustSize_Right);
				});
	}		
}

void CALUMITabModule::AdjustSize_Right()
{
	_rightScrollLayout->update();
	_rightWidget->adjustSize();
	auto widthHint = _rightWidget->sizeHint().width() > 25 ? _rightWidget->sizeHint().width() + 10 : 0;
	_rightScrollArea->setFixedWidth(widthHint);

	auto s = _centralLayout->sizes();
	s[2] = _rightScrollArea->size().width();
	_centralLayout->setSizes(s);
}

void CALUMITabModule::SetSidebarVisibility_Right(bool show)
{
	_rightScrollArea->setVisible(show);
}

void CALUMITabModule::SetSideBarItem_Left(QWidget* item, bool show)
{
	ResetSideBar_Left();

	_leftAreaParentLayout->addWidget(item);
	_leftAreaParent->setFixedWidth(item->sizeHint().width());
	auto s = _centralLayout->sizes();
	s[0] = _leftAreaParent->sizeHint().width();
	_centralLayout->setSizes(s);

	if(auto bWidget = dynamic_cast<IButtonBox*>(item)) {
		auto button = bWidget->GetButtonBox()->button(QDialogButtonBox::Close);
		connect(button, &QPushButton::pressed, this, &CALUMITabModule::CloseSideBarItem_Left);
	}

	_leftAreaParent->setVisible(show);
}

void CALUMITabModule::CloseSideBarItem_Left()
{
	_leftAreaParent->setVisible(false);
}

bool CALUMITabModule::GetSideBarVisibility_Left() const {
	return _leftAreaParent->isVisible();
}

void CALUMITabModule::ResetSideBar_Left()
{
	for (int i = 0; i < _leftAreaParentLayout->count(); i++)
	{
		if (auto widget = _leftAreaParentLayout->itemAt(i)->widget())
		{
			widget->deleteLater();
		}
	}
}

void CALUMITabModule::ResetSideBar_Right()
{
	if (_rightWidget) _rightWidget->deleteLater();

	_rightScrollLayout = new QVBoxLayout();
	_rightWidget = new QWidget();
	_rightWidget->setLayout(_rightScrollLayout);
	_rightWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	_rightScrollArea->setWidget(_rightWidget);
	_rightWidget->setStyleSheet("QWidget {border-color: transparent;}");
	_rightScrollLayout->addStretch(1);
}
