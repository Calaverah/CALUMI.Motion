//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <QDialog>
#include "ui_FilteredDropDownDialog.h"
#include "Utilities/AgxDefinitions.h"


class CALUMIMotion;

class AgxEventFilterProxyModel;

class FilteredDropDownDialog : public QDialog
{
	Q_OBJECT

public:
	FilteredDropDownDialog(QWidget* parent = nullptr);
	~FilteredDropDownDialog();

public:
	QComboBox* GetMainComboBox() const;
	QComboBox* GetFilterBox() const;
	QString GetText() const;
	void SetCustomHolder(const QString& str = "");


public slots:
	void OnFilterChange(const QString& str);
	
signals:
	void FilterChangedSignal(const QString& str);

protected:
	void changeEvent(QEvent* event) override;

private:
	Ui::FilteredDropDownDialogClass ui;

	QString _customHolder;
	QString _noneString = QObject::tr("<none>");
	QString _customString = QObject::tr("<custom>");
};

