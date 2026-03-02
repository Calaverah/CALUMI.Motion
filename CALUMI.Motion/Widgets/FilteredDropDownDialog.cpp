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
	if (ui.MainComboBox->currentText() == _noneString) return "";

	return ui.MainComboBox->currentText();
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


