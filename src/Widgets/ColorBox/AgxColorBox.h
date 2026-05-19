//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QWidget>

class AgxColorBox  : public QWidget
{
	Q_OBJECT

public:
	explicit AgxColorBox(QWidget *parent = nullptr);
	~AgxColorBox() override;

	void setColor(QColor color);
	QColor getColor() const;

protected:
	void mousePressEvent(QMouseEvent* event) override;
};

