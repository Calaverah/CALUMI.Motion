//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)
#include <QDialog>
#include "ui_StringEntryPopup.h"
#pragma warning(pop)


class StringEntryPopup : public QDialog
{
	Q_OBJECT

public:
	StringEntryPopup(QWidget *parent = nullptr);
	~StringEntryPopup();

	QString getInputText() const;

public slots:
	void AttemptAccept();
	void ResetError();

protected:
	void changeEvent(QEvent* event) override;

private:
	Ui::StringEntryPopupClass ui;
	QRegularExpressionValidator validator;
};

