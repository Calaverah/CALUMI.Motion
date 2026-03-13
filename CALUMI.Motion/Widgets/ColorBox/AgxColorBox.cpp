#include "stdafx.h"
#include "AgxColorBox.h"

AgxColorBox::AgxColorBox(QWidget *parent)
	: QWidget(parent)
{
	setAutoFillBackground(true);
	setFixedSize(16, 16);
}

AgxColorBox::~AgxColorBox()
{}

void AgxColorBox::setColor(QColor color)
{
	auto pal = palette();
	pal.setColor(QPalette::ColorRole::Window, color);
	setPalette(pal);
}

QColor AgxColorBox::getColor() const
{
	auto pal = palette();
	return pal.color(QPalette::Window);
}

void AgxColorBox::mousePressEvent(QMouseEvent * event)
{
	auto result = QColorDialog::getColor(getColor(), this, "Select Color");
	if (result.isValid())
		setColor(result);
}

