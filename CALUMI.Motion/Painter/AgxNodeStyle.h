#pragma once

#include "AgxStyle.h"
#include <QColor>

class AgxNodeStyle : public AgxStyle
{
public:
    AgxNodeStyle();

    AgxNodeStyle(QString jsonText);

    AgxNodeStyle(QJsonObject const& json);

    virtual ~AgxNodeStyle() = default;

public:
    static void setNodeStyle(QString jsonText);

public:
    void loadJson(QJsonObject const& json) override;

    QJsonObject toJson() const override;

public:
    QColor NormalBoundaryColor;
    QColor SelectedBoundaryColor;
    QColor GradientColor0;
    QColor GradientColor1;
    QColor GradientColor2;
    QColor GradientColor3;
    QColor ShadowColor;
    bool ShadowEnabled;
    QColor FontColor;
    QColor FontColorFaded;

    QColor ConnectionPointColor;
    QColor FilledConnectionPointColor;

    QColor WarningColor;
    QColor ErrorColor;
    QColor ToolTipIconColor;

    float PenWidth;
    float HoveredPenWidth;

    float ConnectionPointDiameter;

    float Opacity;
};