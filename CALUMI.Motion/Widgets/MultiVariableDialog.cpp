//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "MultiVariableDialog.h"
#include "CALUMIMotion.h"
#include "Utilities/UndoRedoCommands.h"

MultiVariableDialog::MultiVariableDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.customBoolean->setVisible(false);
	ui.customEntry->setVisible(false);
	ui.vectorEntryBox->setVisible(false);

}

MultiVariableDialog::~MultiVariableDialog()
{
}


void MultiVariableDialog::SetCustomFloat(bool allowCustom, bool allowBlank, bool negativeAllowed)
{
	_varType = AgxVarType::Float;
	ui.customEntry->setEnabled(false);
	ui.customEntry->setVisible(allowCustom);

	if (allowBlank) ui.MainComboBox->addItem(_noneString);

	if (allowCustom)
	{
		ui.MainComboBox->addItem(_customVarString);
		ui.MainComboBox->addItem(_staticVarString);

		auto validator = new QDoubleValidator(this);
		
		validator->setDecimals(5);
		
		if (!negativeAllowed) 
			validator->setBottom(0);
		

		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this, validator]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
			{
				ui.customEntry->setValidator(validator);
				ui.customEntry->setText("");
				this->ui.customEntry->setEnabled(true);
			}
			else if (this->ui.MainComboBox->currentText() == _customVarString)
			{
				ui.customEntry->setValidator(nullptr);
				ui.customEntry->setText("");
				this->ui.customEntry->setEnabled(true);
			}
			else
				this->ui.customEntry->setEnabled(false);

			});
		
	}
}

void MultiVariableDialog::SetCustomInteger(bool allowCustom, bool allowBlank, bool negativeAllowed)
{
	_varType = AgxVarType::Integer;
	ui.customEntry->setEnabled(false);
	ui.customEntry->setVisible(allowCustom);

	if (allowBlank) 
		ui.MainComboBox->addItem(_noneString);

	if (allowCustom)
	{
		if (allowCustom) ui.MainComboBox->addItem(_staticVarString);
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
				this->ui.customEntry->setEnabled(true);
			else
				this->ui.customEntry->setEnabled(false);

			});

		auto validator = new QIntValidator(this);
		if(!negativeAllowed)validator->setBottom(0);
		ui.customEntry->setValidator(validator);
	}
}

void MultiVariableDialog::SetCustomBoolean(bool allowCustom, bool allowBlank)
{
	_varType = AgxVarType::Boolean;
	ui.customBoolean->setEnabled(false);
	ui.customBoolean->setVisible(allowCustom);

	if (allowBlank) ui.MainComboBox->addItem(_noneString);

	connect(ui.customBoolean, &QPushButton::pressed, this, &MultiVariableDialog::ToggleBoolean);

	if (allowCustom)
	{
		if (allowCustom) ui.MainComboBox->addItem(_staticVarString);
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
			{
				this->ui.customBoolean->setEnabled(true);
				QString text = _boolState ? _trueString : _falseString;
				ui.customBoolean->setText(text);
			}
			else
			{
				this->ui.customBoolean->setEnabled(false);
				this->ui.customBoolean->setText("---");
			}

			});
	}
}

void MultiVariableDialog::SetCustomVector(bool allowCustom, bool allowBlank, bool negativeAllowed)
{
	_varType = AgxVarType::Vector;
	ui.vectorEntryBox->setEnabled(false);
	ui.vectorEntryBox->setVisible(allowCustom);

	if (allowBlank) ui.MainComboBox->addItem(_noneString);

	if (allowCustom) 
	{
		ui.MainComboBox->addItem(_staticVarString);
		
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
				this->ui.vectorEntryBox->setEnabled(true);
			else
				this->ui.vectorEntryBox->setEnabled(false);

			});

		connect(ui.buttonBox, &QDialogButtonBox::accepted, this, [this]() {
			if (ui.xVal->text().isEmpty()) ui.xVal->setText("0");
			if (ui.yVal->text().isEmpty()) ui.yVal->setText("0");
			if (ui.zVal->text().isEmpty()) ui.zVal->setText("0");
			});

		auto validator = new QDoubleValidator(this);
		validator->setDecimals(5);
		if (!negativeAllowed) validator->setBottom(0);
		ui.xVal->setValidator(validator);
		ui.yVal->setValidator(validator);
		ui.zVal->setValidator(validator);
	}
}

void MultiVariableDialog::DisableComboBox()
{
	ui.MainComboBox->setCurrentText(_staticVarString);
	ui.vectorEntryBox->setEnabled(true);
	ui.MainComboBox->setEditable(false);
	ui.MainComboBox->setVisible(false);
}

QComboBox* MultiVariableDialog::GetComboBox()
{
	return ui.MainComboBox;
}

QString MultiVariableDialog::GetValue() const
{
	QString text = ui.MainComboBox->currentText();
	if (text == _noneString) return "";
	if (text == _staticVarString)
	{
		switch (_varType)
		{
		case AgxVarType::Integer:
		{
			QString entry = ui.customEntry->text();
			bool ok;
			int value = entry.toInt(&ok);
			if (ok)
			{
				return(std::to_string(value).c_str());
			}
			return ui.customEntry->text();
		}
		case AgxVarType::Float:
		{
			//this helps filter out scientific notation which we don't know will suffice in the engine
			bool ok;
			double value = ui.customEntry->text().toDouble(&ok);
			if (!ok)
				value = 0;

			return CleanUpDecimals(QString::number(value,'f',5), 5);
		}
		case AgxVarType::Boolean:
		{
			if (_boolState)
				return "True";
			else
				return "False";
		}
		break;
		case AgxVarType::Vector:
			return ui.xVal->text() + ";" + ui.yVal->text() + ";" + ui.zVal->text();
		default:
			return "ERROR";
		}
	}
	if (text == _customVarString)
	{
		return ui.customEntry->text();
	}
	return text;
}

bool MultiVariableDialog::SetInitialVector(const QString& vector)
{
	QStringList nums = vector.split(';');
	nums.removeAll("");
	QVector<float> floats;
	for (auto& entry : nums)
	{
		bool ok;
		float value = entry.trimmed().toFloat(&ok);
		if (ok)
		{
			floats.append(value);
		}
	}

	if (floats.size() == 3)
	{
		char bufferx[256];
		sprintf_s(bufferx, "%.5g", floats.at(0));
		char buffery[256];
		sprintf_s(buffery, "%.5g", floats.at(1));
		char bufferz[256];
		sprintf_s(bufferz, "%.5g", floats.at(2));

		ui.xVal->setText(bufferx);
		ui.yVal->setText(buffery);
		ui.zVal->setText(bufferz);
		return true;
	}

	ui.xVal->setText("0");
	ui.yVal->setText("0");
	ui.zVal->setText("0");
	return false;
}

bool MultiVariableDialog::SetInitialBoolen(const QString& boolean)
{
	if (boolean.compare("True", Qt::CaseInsensitive) == 0)
	{
		ui.customBoolean->setText(_trueString);
		_boolState = true;
		return true;
	}

	if (boolean.compare("False", Qt::CaseInsensitive) == 0)
	{
		ui.customBoolean->setText(_falseString);
		_boolState = false;
		return true;
	}
	_boolState = false;
	return false;
}

bool MultiVariableDialog::SetInitialInteger(const QString& integer)
{
	QStringList strList = integer.split('.');

	for (auto& entry : strList)
	{
		bool ok;
		int value = entry.toInt(&ok);
		if (ok)
		{
			ui.customEntry->setText(std::to_string(value).c_str());
			return true;
		}
	}
	ui.customEntry->setText("");
	return false;
}

bool MultiVariableDialog::SetInitialFloat(const QString& floatpt)
{
	if (floatpt.isEmpty())
	{
		ui.customEntry->setText("");
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}

	int idxCustom = ui.MainComboBox->findText(_customVarString);
	int idxDrop = ui.MainComboBox->findText(floatpt);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		ui.customEntry->setText("");
		return true;
	}
	
	bool ok;

	float value = floatpt.toDouble(&ok);
	
	if (ok)
	{
		QString buffer = QString::number(value, 'f', 5);
		ui.customEntry->setText(CleanUpDecimals(buffer, 5));
		ui.MainComboBox->setCurrentText(_staticVarString);
		
		return true;
	}

	if (idxCustom >= 0)
	{
		ui.customEntry->setValidator(nullptr);
		ui.customEntry->setText(floatpt);
		ui.MainComboBox->setCurrentIndex(idxCustom);

		return true;
	}

	ui.customEntry->setText("");
	ui.MainComboBox->setCurrentIndex(0);
	return false;
}

void MultiVariableDialog::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange) {
		ui.retranslateUi(this);
	}
	QWidget::changeEvent(event);
}

void MultiVariableDialog::ToggleBoolean()
{
	_boolState = !_boolState;
	QString text = _boolState ? _trueString : _falseString;
	ui.customBoolean->setText(text);
}

