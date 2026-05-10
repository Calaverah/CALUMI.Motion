//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QColor>
#include <QRandomGenerator>
#pragma warning(pop)

inline QColor generateRandomQColor() {

    QRandomGenerator* generator = QRandomGenerator::global();

    int r = generator->bounded(256); 
    int g = generator->bounded(256); 
    int b = generator->bounded(256); 

    QColor output(r, g, b);
    //if (output == QColor(Qt::magenta))
    //    return generateRandomQColor();
    qreal sat = output.hsvSaturation() + 150 > 255 ? 255 : output.hsvSaturation() + 150;
    output.setHsv(output.hsvHue(), sat, 255);
    return output;
}