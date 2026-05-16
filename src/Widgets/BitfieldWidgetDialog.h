//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QWidget>
#include <QCheckBox>
#include <QDialog>
#pragma warning(pop)


class BitfieldWidgetDialog : public QDialog
{
	Q_OBJECT
public:
	explicit BitfieldWidgetDialog(QWidget* parent = nullptr);
	~BitfieldWidgetDialog() override = default;

	QCheckBox* AddCheckBox(const QString& title);
	QList<QCheckBox*> AddCheckBoxes(const QStringList& list);

	void SetInitialValues(size_t value = 0) const;

	QList<bool> GetValuesAsList() const;
	size_t GetValues() const;

private:
	QDialogButtonBox* _buttonBox;

	QVBoxLayout* _vLayout;
	QWidget* _scrollAreaWidget;
	QVBoxLayout* _scrollAreaLayout;

	QList<QCheckBox*> _checkBoxes;
};

