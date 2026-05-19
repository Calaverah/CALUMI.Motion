//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScopeInitStatement
#include "stdafx.h"
#include "AgxColorBox.h"

AgxColorBox::AgxColorBox(QWidget *parent)
	: QWidget(parent)
{
	setAutoFillBackground(true);
	setFixedSize(16, 16);
}

AgxColorBox::~AgxColorBox() = default;

void AgxColorBox::setColor(const QColor color)
{
	auto pal = palette();
	pal.setColor(QPalette::ColorRole::Window, color);
	setPalette(pal);
}

QColor AgxColorBox::getColor() const
{
	const auto pal = palette();
	return pal.color(QPalette::Window);
}

void AgxColorBox::mousePressEvent(QMouseEvent * event)
{
	const auto result = QColorDialog::getColor(getColor(), this, "Select Color");

	if (result.isValid())
		setColor(result);
}

