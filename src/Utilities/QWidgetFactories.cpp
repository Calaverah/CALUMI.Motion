//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "QWidgetFactories.h"

void SetTransparentBackground(QWidget* widget)
{
    if (!widget)
        return;

    auto pal = widget->palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    widget->setPalette(pal);
    widget->setAutoFillBackground(true);
}
