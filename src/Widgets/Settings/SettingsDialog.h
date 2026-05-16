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
	explicit SettingsDialog(QWidget *parent = nullptr);
	~SettingsDialog() override = default;

public slots:
	void ApplySettings() const;

protected:
	void changeEvent(QEvent* event) override;
public:
	void accept() override;

private:
	void RefreshUiValues() const;

	Ui::SettingsDialogClass ui{};

	QStringList m_startupVisibilityStrings = {
		QObject::tr("Never"),
		QObject::tr("Remember"),
		QObject::tr("Always")
	};
};

