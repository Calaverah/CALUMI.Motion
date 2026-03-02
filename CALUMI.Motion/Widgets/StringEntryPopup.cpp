#include "stdafx.h"
#include "StringEntryPopup.h"
#include <windows.h>

StringEntryPopup::StringEntryPopup(QWidget *parent)
	: QDialog(parent), validator(QRegularExpression("[A-Za-z0-9_]{5,64}"))
{
	ui.setupUi(this);
	ui.lineEdit->setValidator(&validator);
	ui.label->setVisible(false);
}

StringEntryPopup::~StringEntryPopup()
{}

QString StringEntryPopup::getInputText() const
{
	return ui.lineEdit->text();
}

void StringEntryPopup::AttemptAccept()
{
	if (ui.lineEdit->hasAcceptableInput())
	{
		accept();
	}
	else
	{
		ui.lineEdit->setProperty("error", true);
		ui.label->setVisible(true);
		ui.lineEdit->style()->unpolish(ui.lineEdit);
		ui.lineEdit->style()->polish(ui.lineEdit);
		ui.lineEdit->repaint();
	}
}

void StringEntryPopup::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}

void StringEntryPopup::ResetError()
{
	if (ui.lineEdit->property("error").toBool())
	{
		if (ui.lineEdit->hasAcceptableInput())
		{
			ui.lineEdit->setProperty("error", false);
			ui.label->setVisible(false);
			ui.lineEdit->style()->unpolish(ui.lineEdit);
			ui.lineEdit->style()->polish(ui.lineEdit);
			ui.lineEdit->update();
		}
	}
}

