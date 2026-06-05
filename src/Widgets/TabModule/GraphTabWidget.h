// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once

#include <qsplitter.h>
#include <QFileInfo>
#include "ITabWidget.h"
#include "Widgets/AgxGraphicsView.h"

class GraphTabWidget : public ITabWidget
{
public:
    Q_OBJECT

public:
    explicit GraphTabWidget(QWidget* parent = nullptr);
    ~GraphTabWidget() override;

    [[nodiscard]] AgxGraphicsView* graph() const;
    void setGraph(AgxGraphicsView* content);

    void addRightItem(QWidget* item) const;
    void showRightItem(QWidget* item, bool moveToEnd = true) const;
    void hideRightItem(QWidget* item) const;
    [[nodiscard]] bool rightPanelVisible() const;

    void setLeftItem(QWidget* item);
    [[nodiscard]] bool leftPanelVisible() const;

public slots:
    void onNodeEstablished(AgxNodeId nodeId);
    void onSetRightPanelVisible(bool setVisible);
    void onSetLeftPanelVisible(bool setVisible) const;

public:
    void buildMenus(CALUMIMotion* parent) override;

    [[nodiscard]] QString tabTitle() const override;
    [[nodiscard]] QColor tabTitleColor() const override;

public slots:
    void onShowMenus() const override;
    void onHideMenus() const override;

    bool onLoadGraphFile(const QFileInfo& fileInfo);

private:
    QVBoxLayout* m_topLayout = Q_NULLPTR;
    QSplitter* m_centralLayout = Q_NULLPTR;
    AgxGraphicsView* m_graph = Q_NULLPTR;

    QWidget* m_leftAreaParent = Q_NULLPTR;
    QVBoxLayout* m_leftAreaParentLayout = Q_NULLPTR;
    QScrollArea* m_rightScrollArea = Q_NULLPTR;
    QWidget* m_rightWidget = Q_NULLPTR;
    QVBoxLayout* m_rightScrollLayout = Q_NULLPTR;
    bool m_allowRightPanelVisible = true;
    int m_localRightWidth = 0;

    QPushButton* m_leftCloseButton = Q_NULLPTR;

    QMap<AgxNodeId, QPointer<QWidget>> m_rightItems;
    QPointer<QWidget> m_leftItem = Q_NULLPTR;
};
