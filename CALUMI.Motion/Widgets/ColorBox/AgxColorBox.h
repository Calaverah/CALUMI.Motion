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

