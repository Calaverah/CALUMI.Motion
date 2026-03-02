#include "stdafx.h"
#include "SettingsDialog.h"
#include "Utilities/SettingsRegistry.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.languageComboBox->insertItems(0, LanguageStrings);
	RefreshUiValues();
}

SettingsDialog::~SettingsDialog()
{}

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
	ui.showConsoleLogComboBox_3->insertItems(0, _StartupVisibilityStrings);
	ui.showPropertiesSidebarComboBox->clear();
	ui.showPropertiesSidebarComboBox->insertItems(0, _StartupVisibilityStrings);
	

	//General
	ui.convertFunctionsCheckBox->setChecked(ref.GetConvertFunctionsSetting());
	ui.showPropertiesSidebarComboBox->setCurrentIndex(static_cast<int>(ref.GetPropertySidebarVisibilityPreference()));

	//Console Log
	ui.showConsoleLogComboBox_3->setCurrentIndex(static_cast<int>(ref.GetConsoleVisibilityPreference()));
	ui.saveConsoleGeometryCheckBox_3->setChecked(ref.UseSavedConsoledGeometry());

	//SFBGS
	ui.sfbgs_GraphDataPathLineEdit->setText(ref.GetRelativeDataPath(AgxGameType::SFBGS));

	//Language
	ui.languageComboBox->setCurrentText(LanguageCodeToString(ref.GetLanguage()));
}

void SettingsDialog::ApplySettings()
{
	auto& ref = SettingsRegistry::GetInstance();

	//General
	ref.SetConvertFunctionsSetting(ui.convertFunctionsCheckBox->isChecked());
	ref.SetPropertySidebarVisibilityPreference(ui.showPropertiesSidebarComboBox->currentIndex());

	//Console Log
	ref.SetConsoleVisibilityPreference(ui.showConsoleLogComboBox_3->currentIndex());
	ref.SetUseSavedConsoledGeometry(ui.saveConsoleGeometryCheckBox_3->isChecked());

	//SFBGS
	ref.SaveRelativeDataPath(AgxGameType::SFBGS, ui.sfbgs_GraphDataPathLineEdit->text());

	//Langauge
	ref.SetLanguage(LanguageCodeFromString(ui.languageComboBox->currentText()));

	//Finalize
	ref.SyncSettings();

	//Refresh View
	RefreshUiValues();
}

