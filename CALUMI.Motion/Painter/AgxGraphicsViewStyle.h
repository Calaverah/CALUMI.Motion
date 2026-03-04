#pragma once

#include "AgxStyle.h"

class AgxGraphicsViewStyle : public AgxStyle
{
public:
    AgxGraphicsViewStyle();

    AgxGraphicsViewStyle(QString jsonText);

    ~AgxGraphicsViewStyle() = default;

public:
    static void setStyle(QString jsonText);

private:
    void loadJson(QJsonObject const& json) override;

    QJsonObject toJson() const override;

public:
    QColor BackgroundColor = QColor(53, 53, 53);
    QColor FineGridColor = QColor(60, 60, 60);
    QColor CoarseGridColor = QColor(25, 25, 25);
};