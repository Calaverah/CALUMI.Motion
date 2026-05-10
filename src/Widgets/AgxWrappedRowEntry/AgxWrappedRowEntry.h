#pragma once

#include <QWidget>

class AgxWrappedRowEntry  : public QWidget
{
	Q_OBJECT

public:
	AgxWrappedRowEntry(QList<QWidget*> itemsToAdd, QWidget *parent = nullptr, uint8_t maxItemPerRow = 8, bool addSeparator = false, int maxWidthForWrapping = -1);
	~AgxWrappedRowEntry();

public:
	QWidget* getItem(int column) const;

public slots:
	void setIndentSize(int size = 10);

private:
	void createRow(QList<QWidget*> rowItems);

private:
	int _maxWidthForWrapping = -1;
	uint8_t _maxItemPerRow = 8;

	QVBoxLayout* _vLayout = nullptr;
	QList<QPointer<QHBoxLayout>> _wRows = {};
	QList<QWidget*> _items = {};
	QList<QSpacerItem*> _indentations = {};
};

