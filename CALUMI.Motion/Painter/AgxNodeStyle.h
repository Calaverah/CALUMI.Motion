#pragma once

#include "AgxStyle.h"
#include <QColor>

class AgxNodeStyle : public AgxStyle
{
public:
    AgxNodeStyle();

    AgxNodeStyle(QString jsonText);

    AgxNodeStyle(const QJsonObject& json);

    virtual ~AgxNodeStyle() = default;

public:
    static void setNodeStyle(QString jsonText);

public:
    void loadJson(const QJsonObject& json) override;

    QJsonObject toJson() const override;

public:
    QColor NormalBoundaryColor = QColor(255, 255, 255);
    QColor SelectedBoundaryColor = QColor(255, 165, 0);
    QColor GradientColor0 = QColor(70, 70, 70);
    QColor GradientColor1 = QColor(70, 70, 70);
    QColor GradientColor2 = QColor(70, 70, 70);
    QColor GradientColor3 = QColor(70, 70, 70);
    QColor AlternateStateColor = QColor(Qt::cyan);
    //QColor ShadowColor;
    //bool ShadowEnabled = false;
    QColor FontColor = QColor(Qt::white);
    QColor FontColorFaded = QColor(Qt::gray);

    QColor ConnectionPointColor = QColor(169, 169, 169);
    QColor FilledConnectionPointColor = QColor(Qt::cyan);

    QColor WarningColor = QColor(255, 179, 0);
    QColor ErrorColor = QColor(211, 47, 47);
    QColor ToolTipIconColor = QColor(Qt::white);

    float PenWidth = 1.2f;
    float HoveredPenWidth = 2.25f;

    float ConnectionPointDiameter = 8.0f;

    float Opacity = 0.86f;
};