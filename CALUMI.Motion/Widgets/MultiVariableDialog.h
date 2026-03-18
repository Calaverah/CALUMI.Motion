//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QDialog>
#include "ui_MultiVariableDialog.h"
#include "Utilities/AgxDefinitions.h"
#include "Widgets/AgxGraphicsView.h"

class CALUMIMotion;

class MultiVariableDialog : public QDialog
{
	Q_OBJECT

public:
	MultiVariableDialog(QWidget* parent = nullptr);
	~MultiVariableDialog();

	void SetCustomFloat(bool allowCustom = true, bool allowBlank = true, bool negativeAllowed = true);
	void SetCustomInteger(bool allowCustom = true, bool allowBlank = true, bool negativeAllowed = true);
	void SetCustomBoolean(bool allowCustom = true, bool allowBlank = true);
	void SetCustomVector(bool allowCustom = true, bool allowBlank = true, bool negativeAllowed = true);

	void DisableComboBox();

	QComboBox* GetComboBox();
	QString GetValue() const;

	bool SetInitialVector(const QString& vector);
	bool SetInitialBoolen(const QString& boolean);
	bool SetInitialInteger(const QString& integer);
	bool SetInitialFloat(const QString& floatpt);


public Q_SLOTS:
	void ToggleBoolean();

protected:
	void changeEvent(QEvent* event) override;

private:
	QString _trueString = QObject::tr("True");
	QString _falseString = QObject::tr("False");
	QString _customString = QObject::tr("<custom>");
	QString _noneString = QObject::tr("<none>");

	Ui::MultiVariableDialogClass ui;

	bool _boolState = false;
	AgxVarType _varType = AgxVarType::String; //default so that basic lists can be used


};

