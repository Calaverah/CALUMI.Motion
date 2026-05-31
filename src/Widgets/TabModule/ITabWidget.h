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
    virtual void clearMenus()
    {
        for (const auto action : m_actions)
        {
            if (action)
            {
                action->deleteLater();
            }
        }
        m_actions.clear();
    }

public slots:
    virtual void onShowMenus() const = 0;
    virtual void onHideMenus() const = 0;

Q_SIGNALS:
    void statusUpdate(float loadPercentage, const QString& message = QString());

protected:
    QList<QAction*> m_actions;
};