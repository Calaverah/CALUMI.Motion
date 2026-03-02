#include "stdafx.h"
#include "AgxLineEdit.h"

AgxLineEdit::AgxLineEdit(QWidget* parent) : QLineEdit(parent)
{
	connect(this, &AgxLineEdit::textChanged, this, &AgxLineEdit::RefreshTooltip);
	setContextMenuPolicy(Qt::NoContextMenu);
}

AgxLineEdit::~AgxLineEdit()
{
	
}

void AgxLineEdit::RefreshTooltip(const QString& str)
{
	if (str == "")
	{
		setToolTip("");
	} else
	{
		setToolTip(_placeHolderText);
	}
}

void AgxLineEdit::SetUpDoubleClickEdit()
{
	_placeHolderText = "Double Click To Edit";
	setPlaceholderText(_placeHolderText);
	connect(this, &AgxLineEdit::DoubleClicked, this, [this]() { deselect(); });
}

void AgxLineEdit::SetUpSingleClickEdit()
{
	_placeHolderText = "Click To Edit";
	setPlaceholderText(_placeHolderText);
	connect(this, &AgxLineEdit::Clicked, this, [this]() { deselect(); });
}

void AgxLineEdit::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		event->ignore();
		return;
	}
	QLineEdit::mousePressEvent(event);
	Q_EMIT Clicked();
}
void AgxLineEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		event->ignore();
		return;
	}
	QLineEdit::mouseDoubleClickEvent(event);
	Q_EMIT DoubleClicked();
}