//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "FilteredDropDownDialog.h"
#include "Utilities/DialogPool.h"
#include "Utilities/UndoRedoCommands.h"
#include "CALUMIMotion.h"
#include "TextEditDialog.h"

FilteredDropDownDialog::FilteredDropDownDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.FilterComboBox, &QComboBox::textActivated, this, &FilteredDropDownDialog::OnFilterChange);
}

FilteredDropDownDialog::~FilteredDropDownDialog()
{
}

QComboBox* FilteredDropDownDialog::GetMainComboBox() const
{
	return ui.MainComboBox;
}

QComboBox* FilteredDropDownDialog::GetFilterBox() const
{
	return ui.FilterComboBox;
}

QString FilteredDropDownDialog::GetText() const
{
	//Since some entries might not yet have translation, like Events, we include both versions of the custom and none checks for now
	if (ui.MainComboBox->currentText() == _noneString || ui.MainComboBox->currentText() == "<none>") 
		return "";
		

	if (ui.MainComboBox->currentText() == _customString || ui.MainComboBox->currentText() == "<custom>")
	{
		bool ok = false;
		QString output = QInputDialog::getText(nullptr, "Custom Value", "Entry", QLineEdit::Normal, _customHolder, &ok);

		if (!ok)
			output = _customHolder;

		return output;
	}

	return ui.MainComboBox->currentText();
}
void FilteredDropDownDialog::SetCustomHolder(const QString& str)
{
	_customHolder = str;
}
void FilteredDropDownDialog::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}
	QWidget::changeEvent(event);
}
void FilteredDropDownDialog::OnFilterChange(const QString& str)
{
	Q_EMIT FilterChangedSignal(str);
}


