// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once

#include <qsplitter.h>

#include "ITabWidget.h"

class RigTabWidget : public ITabWidget {
    Q_OBJECT

public:
    explicit RigTabWidget(QWidget* parent = nullptr);
    ~RigTabWidget() override = default;

    void buildMenus(CALUMIMotion* parent) override {}

    [[nodiscard]] QString tabTitle() const override { return "RigTab"; }

    [[nodiscard]] QColor tabTitleColor() const override { return Qt::magenta;}

    void onShowMenus() const override {}

    void onHideMenus() const override {}

private:
    QSplitter* m_mainSplitter = Q_NULLPTR;
    QScrollArea* m_topScrollArea = Q_NULLPTR;
    QScrollArea* m_middleScrollArea = Q_NULLPTR;
    QScrollArea* m_bottomScrollArea = Q_NULLPTR;

};


