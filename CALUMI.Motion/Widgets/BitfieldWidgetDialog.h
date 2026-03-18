//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QWidget>
#pragma warning(pop)


class BitfieldWidgetDialog : public QDialog
{
	Q_OBJECT
public:
	BitfieldWidgetDialog(QWidget* parent = nullptr);
	~BitfieldWidgetDialog() = default;

	QCheckBox* AddCheckBox(const QString& title);
	QList<QCheckBox*> AddCheckBoxes(const QStringList& list);

	void SetInitialValues(size_t value = 0);

	QList<bool> GetValuesAsList() const;
	size_t GetValues() const;

private:
	QDialogButtonBox* _buttonBox;

	QVBoxLayout* _vLayout;
	QWidget* _scrollAreaWidget;
	QVBoxLayout* _scrollAreaLayout;

	QList<QCheckBox*> _checkBoxes;
};

