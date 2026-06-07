// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#include "RigTabWidget.h"

RigTabWidget::RigTabWidget(QWidget* parent) : ITabWidget(parent)
{
    m_mainSplitter = new QSplitter(this);
    m_mainSplitter->setOrientation(Qt::Horizontal);

    m_topScrollArea = new QScrollArea(this);
    m_middleScrollArea = new QScrollArea(this);
    m_bottomScrollArea = new QScrollArea(this);

    m_mainSplitter->addWidget(m_topScrollArea);
    m_mainSplitter->addWidget(m_middleScrollArea);
    m_mainSplitter->addWidget(m_bottomScrollArea);

    const QList sizes = {1,1,1};
    m_mainSplitter->setSizes(sizes);
}
