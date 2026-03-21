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

	if (allowBlank) 
		ui.MainComboBox->addItem(_noneString);

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
		ui.MainComboBox->addItem(_customVarString);
		ui.MainComboBox->addItem(_staticVarString);

		auto validator = new QIntValidator(this);

		if(!negativeAllowed)
			validator->setBottom(0);
		
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this, validator]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
			{
				ui.customEntry->setValidator(validator);
				ui.customEntry->setText("");
				this->ui.customEntry->setEnabled(true);
			}
			else if(this->ui.MainComboBox->currentText() == _customVarString)
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

void MultiVariableDialog::SetCustomBoolean(bool allowCustom, bool allowBlank)
{
	_varType = AgxVarType::Boolean;
	ui.customBoolean->setEnabled(false);
	ui.customBoolean->setVisible(allowCustom);

	if (allowBlank) 
		ui.MainComboBox->addItem(_noneString);

	connect(ui.customBoolean, &QPushButton::pressed, this, &MultiVariableDialog::ToggleBoolean);

	if (allowCustom)
	{
		ui.MainComboBox->addItem(_customVarString);
		ui.MainComboBox->addItem(_staticVarString);

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

			if (this->ui.MainComboBox->currentText() == _customVarString)
			{
				this->ui.customEntry->setVisible(true);
				this->ui.customBoolean->setVisible(false);
			}
			else
			{
				this->ui.customEntry->setVisible(false);
				this->ui.customBoolean->setVisible(true);
			}

			});
	}
}

void MultiVariableDialog::SetCustomVector(bool allowCustom, bool allowBlank, bool negativeAllowed)
{
	_varType = AgxVarType::Vector;
	ui.vectorEntryBox->setEnabled(false);
	ui.vectorEntryBox->setVisible(allowCustom);

	if (allowBlank) 
		ui.MainComboBox->addItem(_noneString);

	if (allowCustom) 
	{
		ui.MainComboBox->addItem(_customVarString);
		ui.MainComboBox->addItem(_staticVarString);
		
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this]() {
			if (this->ui.MainComboBox->currentText() == _staticVarString)
				this->ui.vectorEntryBox->setEnabled(true);
			else
				this->ui.vectorEntryBox->setEnabled(false);

			if (this->ui.MainComboBox->currentText() == _customVarString)
			{
				SetStaticVectorVisibile(false);
				this->ui.customEntry->setVisible(true);
			}
			else
			{
				SetStaticVectorVisibile(true);
				this->ui.customEntry->setVisible(false);
			}

			});

		auto validator = new QDoubleValidator(this);
		
		validator->setDecimals(5);
		
		if (!negativeAllowed) 
			validator->setBottom(0);
		
		ui.xVal->setValidator(validator);
		ui.yVal->setValidator(validator);
		ui.zVal->setValidator(validator);
	}
}

void MultiVariableDialog::SetCustomString(bool allowCustom, bool allowBlank)
{
	_varType = AgxVarType::String;
	ui.customEntry->setEnabled(false);
	ui.customEntry->setVisible(allowCustom);

	if (allowBlank)
		ui.MainComboBox->addItem(_noneString);

	if (allowCustom)
	{
		ui.MainComboBox->addItem(_customVarString);
		connect(ui.MainComboBox, &QComboBox::currentTextChanged, this, [this]() {
			if (this->ui.MainComboBox->currentText() == _customVarString)
			{
				ui.customEntry->setText("");
				ui.customEntry->setEnabled(true);
			}
			else
			{
				ui.customEntry->setEnabled(false);
				ui.customEntry->setText("");
			}
			});
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
			if (!ok)
				value = 0;

			return QString::number(value);
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
			return FormatVector(ui.xVal->text(), ui.yVal->text(), ui.zVal->text());
		
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

	ui.customEntry->setText("");
	SetStaticVector("", "", "");

	if (vector.isEmpty())
	{
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}

	auto idxCustom = ui.MainComboBox->findText(_customVarString);
	auto idxDrop = ui.MainComboBox->findText(vector);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		return true;
	}

	QStringList nums = vector.split(';');
	nums.removeAll("");
	

	if (nums.size() >= 3)
	{
		ui.MainComboBox->setCurrentText(_staticVarString);
		SetStaticVector(nums.at(0), nums.at(1), nums.at(2));
		return true;
	}
	
	if (idxCustom >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxCustom);
		ui.customEntry->setText(vector);
		return true;
	}

	ui.MainComboBox->setCurrentIndex(0);
	return false;
}

bool MultiVariableDialog::SetInitialBoolen(const QString& boolean)
{
	ui.customEntry->setText("");
	ui.customEntry->setVisible(false);
	ui.customBoolean->setVisible(true);

	if (boolean.isEmpty())
	{
		_boolState = false;
		ui.customBoolean->setText("---");
		ui.customBoolean->setEnabled(false);
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}

	auto idxCustom = ui.MainComboBox->findText(_customVarString);
	auto idxDrop = ui.MainComboBox->findText(boolean);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		return true;
	}

	if (boolean.compare("True", Qt::CaseInsensitive) == 0)
	{
		ui.MainComboBox->setCurrentText(_staticVarString);
		SetBoolState(true);
		return true;
	}

	if (boolean.compare("False", Qt::CaseInsensitive) == 0)
	{
		ui.MainComboBox->setCurrentText(_staticVarString);
		SetBoolState(false);
		return true;
	}

	if (idxCustom >= 0)
	{
		ui.customEntry->setText(boolean);
		ui.customEntry->setVisible(true);
		ui.customBoolean->setVisible(false);
		ui.MainComboBox->setCurrentIndex(idxCustom);
		return true;
	}

	SetBoolState(false);
	return false;
}

bool MultiVariableDialog::SetInitialInteger(const QString& integer)
{
	ui.customEntry->setText("");

	if (integer.isEmpty())
	{
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}
	
	QStringList strList = integer.split('.');

	int idxCustom = ui.MainComboBox->findText(_customVarString);
	int idxDrop = ui.MainComboBox->findText(integer);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		return true;
	}

	for (auto& entry : strList)
	{
		bool ok;
		int value = entry.toInt(&ok);
		if (ok)
		{
			ui.MainComboBox->setCurrentText(_staticVarString);
			ui.customEntry->setText(QString::number(value));
			return true;
		}
	}

	if (idxCustom >= 0)
	{
		ui.customEntry->setValidator(nullptr);
		ui.customEntry->setText(integer);
		ui.MainComboBox->setCurrentIndex(idxCustom);
		return true;
	}

	ui.MainComboBox->setCurrentIndex(0);
	return false;
}

bool MultiVariableDialog::SetInitialFloat(const QString& floatpt)
{
	ui.customEntry->setText("");

	if (floatpt.isEmpty())
	{
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}

	int idxCustom = ui.MainComboBox->findText(_customVarString);
	int idxDrop = ui.MainComboBox->findText(floatpt);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		return true;
	}
	
	bool ok;

	float value = floatpt.toDouble(&ok);
	
	if (ok)
	{
		ui.MainComboBox->setCurrentText(_staticVarString);
		QString buffer = QString::number(value, 'f', 5);
		ui.customEntry->setText(CleanUpDecimals(buffer, 5));
		
		return true;
	}

	if (idxCustom >= 0)
	{
		ui.customEntry->setValidator(nullptr);
		ui.customEntry->setText(floatpt);
		ui.MainComboBox->setCurrentIndex(idxCustom);

		return true;
	}

	ui.MainComboBox->setCurrentIndex(0);
	return false;
}

bool MultiVariableDialog::SetInitialString(const QString& string)
{
	ui.customEntry->setText("");

	if (string.isEmpty())
	{
		ui.MainComboBox->setCurrentIndex(0);
		return false;
	}

	auto idxCustom = ui.MainComboBox->findText(_customVarString);
	auto idxDrop = ui.MainComboBox->findText(string);

	if (idxDrop >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxDrop);
		return true;
	}

	if (idxCustom >= 0)
	{
		ui.MainComboBox->setCurrentIndex(idxCustom);
		ui.customEntry->setText(string);
		return true;
	}

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

void MultiVariableDialog::SetBoolState(bool state)
{
	//translated text
	if (state)
		ui.customBoolean->setText(_trueString);
	else
		ui.customBoolean->setText(_falseString);

	_boolState = state;
}

void MultiVariableDialog::SetStaticVectorVisibile(bool state)
{
	ui.vectorEntryBox->setVisible(state);
}

void MultiVariableDialog::SetStaticVectorEnabled(bool state)
{
	ui.vectorEntryBox->setEnabled(state);
}

void MultiVariableDialog::SetStaticVector(QString x, QString y, QString z)
{
	if (x.isEmpty())
	{
		ui.xVal->setText("");
	} 
	else
	{
		bool ok = false;
		double val = x.toDouble(&ok);
		if (!ok)
			val = 0;

		ui.xVal->setText(CleanUpDecimals(QString::number(val, 'f', 5)));
	}

	if (y.isEmpty())
	{
		ui.yVal->setText("");
	} 
	else
	{
		bool ok = false;
		double val = y.toDouble(&ok);
		if (!ok)
			val = 0;

		ui.yVal->setText(CleanUpDecimals(QString::number(val, 'f', 5)));
	}

	if (z.isEmpty())
	{
		ui.zVal->setText("");
	} 
	else
	{
		bool ok = false;
		double val = z.toDouble(&ok);
		if (!ok)
			val = 0;

		ui.zVal->setText(CleanUpDecimals(QString::number(val, 'f', 5)));
	}
}

void MultiVariableDialog::ToggleBoolean()
{
	_boolState = !_boolState;
	QString text = _boolState ? _trueString : _falseString;
	ui.customBoolean->setText(text);
}

