//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QDialog>
#include "ui_TextEditDialog.h"

class TextEditDialog : public QDialog
{
	Q_OBJECT

public:
	TextEditDialog(QWidget *parent = nullptr);
	TextEditDialog(QWidget* parent, const QString& iniText, const QFont& iniFont);
	TextEditDialog(QWidget* parent, const QString& iniText, int iniFontSize);
	~TextEditDialog();

public:
	QString GetText() const;
	int GetFontSize() const;
	QFont GetFont() const;

protected:
	void changeEvent(QEvent* event) override;

private:
	Ui::TextEditDialogClass ui;
};

