//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "TextEditDialog.h"

TextEditDialog::TextEditDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.spinBox, &QSpinBox::valueChanged, this, [this](int size)
		{
			QFont f = ui.textEdit->font();
			f.setPointSize(size);
			ui.textEdit->setFont(f);
		});
	connect(ui.fontComboBox, &QFontComboBox::currentFontChanged, this, [this](const QFont& f) 
		{
			QFont ff = f;
			ff.setPointSize(ui.textEdit->font().pointSize());
			ui.textEdit->setFont(ff);
		});
}

TextEditDialog::TextEditDialog(QWidget* parent, const QString& iniText, const QFont& iniFont) : TextEditDialog(parent)
{
	blockSignals(true);
	ui.spinBox->setValue(iniFont.pointSize());
	ui.fontComboBox->setCurrentFont(iniFont);
	ui.textEdit->setFont(iniFont);
	ui.textEdit->setText(iniText);
	blockSignals(false);
}

TextEditDialog::TextEditDialog(QWidget* parent, const QString& iniText, int iniFontSize) : TextEditDialog(parent)
{
	QFont f = ui.textEdit->font();
	if (iniFontSize > 3)
	{
		f.setPointSize(iniFontSize);
	}
	blockSignals(true);
	ui.spinBox->setValue(f.pointSize());
	ui.fontComboBox->setCurrentFont(f);
	ui.textEdit->setFont(f);
	ui.textEdit->setText(iniText);
	blockSignals(false);
}

TextEditDialog::~TextEditDialog()
{}

int TextEditDialog::GetFontSize() const
{
	return ui.textEdit->font().pointSize();
}
QFont TextEditDialog::GetFont() const
{
	return ui.textEdit->font();
}

void TextEditDialog::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}

	QWidget::changeEvent(event);
}

QString TextEditDialog::GetText() const
{
	return ui.textEdit->toPlainText();
}