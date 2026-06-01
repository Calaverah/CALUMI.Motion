// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once

#include <qsplitter.h>

#include "ITabWidget.h"
#include "Widgets/AgxGraphicsView.h"

class GraphTabWidget : public ITabWidget
{
public:
    Q_OBJECT

public:
    explicit GraphTabWidget(AgxGraphicsView* content, QWidget* parent = nullptr);
    ~GraphTabWidget() override;

    [[nodiscard]] AgxGraphicsView* graph() const;

    void addRightItem(QWidget* item) const;
    void showRightItem(QWidget* item, bool moveToEnd = true) const;
    void hideRightItem(QWidget* item) const;
    bool rightPanelVisible() const;

    void setLeftItem(QWidget* item);
    bool leftPanelVisible() const;

public slots:
    void onNodeEstablished(AgxNodeId nodeId);
    void onSetRightPanelVisible(bool setVisible);
    void onSetLeftPanelVisible(bool setVisible) const;

public:
    void buildMenus(CALUMIMotion* parent) override;
    void onShowMenus() const override;
    void onHideMenus() const override;

private:
    QVBoxLayout* m_topLayout;
    QSplitter* m_centralLayout;
    AgxGraphicsView* m_graph;

    QWidget* m_leftAreaParent;
    QVBoxLayout* m_leftAreaParentLayout;
    QScrollArea* m_rightScrollArea;
    QWidget* m_rightWidget;
    QVBoxLayout* m_rightScrollLayout;
    bool m_allowRightPanelVisible = true;
    int m_localRightWidth = 0;

    QPushButton* m_leftCloseButton = Q_NULLPTR;

    QMap<AgxNodeId, QPointer<QWidget>> m_rightItems;
    QPointer<QWidget> m_leftItem = Q_NULLPTR;
};
