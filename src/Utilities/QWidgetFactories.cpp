//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "QWidgetFactories.h"

#include <QFile>
#include <QPainter>
#include <QtSvg/QSvgRenderer>

#include "Painter/AgxPalette.h"

void SetTransparentBackground(QWidget* widget)
{
    if (!widget)
        return;

    // widget->setAttribute(Qt::WA_TranslucentBackground);
    // widget->setAutoFillBackground(false);

    auto pal = widget->palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    widget->setPalette(pal);
}


QIcon GetColoredIconFromSVG(const QString& path)
{
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly))
        return {};

    QByteArray content = file.readAll();
    content.replace("#000", AgxPalette::GetInstance().iconPalette().IconColor.name(QColor::HexRgb).toStdString().c_str());
    content.replace("black", AgxPalette::GetInstance().iconPalette().IconColor.name(QColor::HexRgb).toStdString().c_str());
    content.replace("#000000", AgxPalette::GetInstance().iconPalette().IconColor.name(QColor::HexRgb).toStdString().c_str());

    QSvgRenderer renderer(content);
    QPixmap pixmap(48,48);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    renderer.render(&painter);
    painter.end();

    return {pixmap};
}
