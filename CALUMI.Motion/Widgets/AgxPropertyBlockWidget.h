//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include <qgridlayout.h>
#include <qlabel.h>

#pragma warning(push,0)
#include <QWidget>
#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
#include <Utilities/AgxDictionary.h>


class AgxLineEdit;
class AgxLineEditContainer;
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
	AgxPropertyBlockWidget(TermRef ref, AgxPropertyBlockData& dataRef, uint8_t wrappedRowItemCount = 3, QWidget* parent = nullptr);

public Q_SLOTS:
	void OnRowAdded(int index);
	void OnRowRemoved(int index);

signals:
	void LanguageChanged();
	void RowsChanged();

protected:
	void ShowContextMenu(const QPoint& pos);

private:
	void ConstructRow(int index);
	void ClearRows(int index);

	void ForceRefresh();

	void SetUpCustomDropDown(AgxLineEditContainer* line, const QList<TermRef>& list, const QStringList& keyPath);

protected:
	void showEvent(QShowEvent* event) override;

private:
	AgxPropertyBlockData* _dataRef;

	QVBoxLayout* _vLayout;
	TermRef _labelRef;

	QAction _addRowAction;
	QList<QLabel*> _columnHeaders = {};
	QList<QPropertyAnimation*> _columnAnimation = {};
	uint8_t _wrappedRowItemCount = 3;
};

