#include "stdafx.h"
#include "AgxWrappedRowEntry.h"

AgxWrappedRowEntry::AgxWrappedRowEntry(QList<QWidget*> itemsToAdd, QWidget* parent, uint8_t indentSize, uint8_t maxItemPerRow, int maxWidthForWrapping) : QWidget(parent), _maxWidthForWrapping(maxWidthForWrapping), _vLayout(new QVBoxLayout())
{
	setContentsMargins(0, 0, 0, 0);
	_vLayout->setContentsMargins(0, 0, 0, 0);

	_maxItemPerRow = qMax(uint8_t(1), maxItemPerRow);

	if (_maxWidthForWrapping < 0)
		_maxWidthForWrapping = 0xFFFF;
	
	for (auto item : itemsToAdd)
	{
		if (item)
			_items.append(item);
	}

	setLayout(_vLayout);

	QList<QWidget*> rowList = {};
	int rowWidth = 0;

	for (auto item : _items)
	{
		if ((rowWidth + item->width() > _maxWidthForWrapping || rowList.count() >= _maxItemPerRow ) && !rowList.isEmpty())
		{
			createRow(rowList);
			rowWidth = 0;
			rowList = {};
		}

		rowWidth += item->width();
		rowList.append(item);
	}

	if (!rowList.isEmpty())
		createRow(rowList);
}

AgxWrappedRowEntry::~AgxWrappedRowEntry() {}

QWidget* AgxWrappedRowEntry::getItem(int column) const
{
	if(column < 0 || column >= _items.size())
		return nullptr;

	return _items.at(column);
}

void AgxWrappedRowEntry::createRow(QList<QWidget*> rowItems)
{
	QHBoxLayout* hBox = new QHBoxLayout();

	if (_wRows.count() != 0)
	{
		QSpacerItem* ind = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
		hBox->addSpacerItem(ind);
		_indentations.append(ind);
	}

	for (auto item : rowItems)
	{
		hBox->addWidget(item);
	}

	_vLayout->addLayout(hBox);
}

void AgxWrappedRowEntry::setIndentSize(int size)
{
	for (auto& ind : _indentations)
	{
		ind->changeSize(size, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
	}
}

