//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QWidget>

class AgxColorBox  : public QWidget
{
	Q_OBJECT

public:
	AgxColorBox(QWidget *parent = nullptr);
	~AgxColorBox();

	void setColor(QColor color);
	QColor getColor() const;

protected:
	void mousePressEvent(QMouseEvent* event) override;
};

