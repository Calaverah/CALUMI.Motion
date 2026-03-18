//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QDialog>
#include "ui_SettingsDialog.h"
#include <Widgets/ColorBox/AgxColorBox.h>

class SettingsRegistry;

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = nullptr);
	~SettingsDialog();

public slots:
	void ApplySettings();

protected:
	void changeEvent(QEvent* event) override;
	void accept() override;

private:
	void RefreshUiValues() const;

private:
	Ui::SettingsDialogClass ui;

	QStringList _StartupVisibilityStrings = {
		QObject::tr("Never"),
		QObject::tr("Remember"),
		QObject::tr("Always")
	};

	//AgxColorBox* _symbolColorBox;
};

