//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QColor>

#include "AgxStyle.h"



class AgxConnectionStyle : public AgxStyle
{
public:
    AgxConnectionStyle();

    AgxConnectionStyle(QString jsonText);

    ~AgxConnectionStyle() = default;

public:
    static void setConnectionStyle(QString jsonText);

public:
    void loadJson(QJsonObject const& json) override;

    QJsonObject toJson() const override;

public:
    QColor constructionColor() const;
    QColor normalColor() const;
    QColor normalColor(QString typeId) const;
    QColor selectedColor() const;
    QColor selectedHaloColor() const;
    QColor hoveredColor() const;

    float lineWidth() const;
    float constructionLineWidth() const;
    float pointDiameter() const;

    bool useDataDefinedColors() const;

private:
    QColor ConstructionColor = QColor(Qt::gray);
    QColor NormalColor = QColor(Qt::darkCyan);
    QColor SelectedColor = QColor(100, 100, 100);
    QColor SelectedHaloColor = QColor(Qt::cyan);
    QColor HoveredColor = QColor(Qt::lightGray);

    float LineWidth = 3.0f;
    float ConstructionLineWidth = 2.0f;
    float PointDiameter = 10.0f;

    bool UseDataDefinedColors = false;
};