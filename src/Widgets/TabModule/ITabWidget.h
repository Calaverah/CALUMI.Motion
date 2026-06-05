// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once

#include <QWidget>
#include <CALUMIMotion.h>

class ITabWidget : public QWidget
{
Q_OBJECT

public:
    explicit ITabWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual void buildMenus(CALUMIMotion* parent) = 0;

    [[nodiscard]] virtual QString tabTitle() const = 0;
    [[nodiscard]] virtual QColor tabTitleColor() const = 0;

public slots:
    virtual void onShowMenus() const = 0;
    virtual void onHideMenus() const = 0;

Q_SIGNALS:
    void statusUpdate(float loadPercentage, const QString& message = QString());

protected:
    QList<QPointer<QAction>> m_actions;

    static QMenu* ParentFile(const CALUMIMotion& parent) { return parent.ui.menuFile; }
    static QMenu* ParentEdit(const CALUMIMotion& parent) { return parent.ui.menuEdit; }
    static QMenu* ParentView(const CALUMIMotion& parent) { return parent.ui.menuView; }
    static QMenu* ParentHelp(const CALUMIMotion& parent) { return parent.ui.menuHelp; }
};