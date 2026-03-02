#pragma once

#pragma warning(push,0)
#include <QWidget>
#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
#include <Utilities/AgxDictionary.h>


class AgxLineEdit;
class AgxPropertyBlockData;


class ModifiedPushButton : public QPushButton
{
	Q_OBJECT

public:
	using QPushButton::QPushButton;

protected:
	bool block = false;
	void mousePressEvent(QMouseEvent* event) override;

signals:
	void CustomPressSignal();

protected:
	void ResetBlock();
};


class AgxPropertyBlockWidget : public QWidget
{
	Q_OBJECT
public:
	AgxPropertyBlockWidget(TermRef ref, AgxPropertyBlockData& dataRef, QWidget* parent = nullptr);

public Q_SLOTS:
	void OnRowAdded(int index);
	void OnRowRemoved(int index);

signals:
	void LanguageChanged();

protected:
	void ShowContextMenu(const QPoint& pos);

private:
	void ConstructRow(int index);
	void ClearRows(int index);

	void ForceRefresh();

	void SetUpCustomDropDown(AgxLineEdit* line, const QStringList& list, const QStringList& keyPath);

protected:
	void showEvent(QShowEvent* event) override;

private:
	AgxPropertyBlockData* _dataRef;

	QGridLayout* _grid;
	QLabel* _label;

	QAction _addRowAction;
};

