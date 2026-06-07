//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "SettingsDialog.h"
#include "../../Utilities/Settings/SettingsRegistry.h"
#include "Application/CALUMIMotionApplication.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
	//,_symbolColorBox(new AgxColorBox())
{
	ui.setupUi(this);

	ui.languageComboBox->insertItems(0, LanguageStrings);

	ui.themeLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression(R"([^\\/:*?"<>|]+$)"),this));

	RefreshUiValues();
}

void SettingsDialog::changeEvent(QEvent * event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}
	QWidget::changeEvent(event);
}

void SettingsDialog::accept()
{
	ApplySettings();

	QDialog::accept();
}

void SettingsDialog::RefreshUiValues() const
{
	auto& ref = SettingsRegistry::GetInstance();

	//Repopulate Localized Lists
	ui.showConsoleLogComboBox_3->clear();
	ui.showConsoleLogComboBox_3->insertItems(0, m_startupVisibilityStrings);

	//General
	ui.convertFunctionsCheckBox->setChecked(ref.GetConvertFunctionsSetting());
	ui.autohideGraphSidebarCheckBox->setChecked(ref.GetGraphSidebarAutoHide());

	//Console Log
	ui.showConsoleLogComboBox_3->setCurrentIndex(static_cast<int>(ref.GetConsoleVisibilityPreference()));
	ui.saveConsoleGeometryCheckBox_3->setChecked(ref.UseSavedConsoledGeometry());

	//SFBGS
	ui.sfbgs_GraphDataPathLineEdit->setText(ref.GetRelativeDataPath(AgxGameType::SFBGS));

	//Language
	ui.languageComboBox->setCurrentText(LanguageCodeToString(ref.GetLanguage()));

	//Visuals
	ui.equationSymbolColorWidget->setColor(ref.GetSymbolColor());
	ui.themeLineEdit->setText(ref.GetThemeFileName());
}

void SettingsDialog::ApplySettings() const
{
	auto& ref = SettingsRegistry::GetInstance();

	//General
	ref.SetConvertFunctionsSetting(ui.convertFunctionsCheckBox->isChecked());
	ref.SetGraphSidebarAutoHide(ui.autohideGraphSidebarCheckBox->isChecked());

	//Console Log
	ref.SetConsoleVisibilityPreference(ui.showConsoleLogComboBox_3->currentIndex());
	ref.SetUseSavedConsoledGeometry(ui.saveConsoleGeometryCheckBox_3->isChecked());

	//SFBGS
	ref.SaveRelativeDataPath(AgxGameType::SFBGS, ui.sfbgs_GraphDataPathLineEdit->text());

	//Langauge
	ref.SetLanguage(LanguageCodeFromString(ui.languageComboBox->currentText()));

	//Visuals
	ref.SetSymbolColor(ui.equationSymbolColorWidget->getColor());
	ref.SetThemeFileName(ui.themeLineEdit->text());

	//Finalize
	ref.SyncSettings();

	//Refresh View
	RefreshUiValues();

	Q_EMIT calApp->settingsUpdated(); // NOLINT(*-pro-type-static-cast-downcast)
}

