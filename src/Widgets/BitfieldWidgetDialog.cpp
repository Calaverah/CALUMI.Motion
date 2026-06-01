//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "BitfieldWidgetDialog.h"

#include "Utilities/QWidgetFactories.h"

BitfieldWidgetDialog::BitfieldWidgetDialog(QWidget* parent) : QDialog(parent)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

	_vLayout = new QVBoxLayout();
	_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	_buttonBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	// ReSharper disable once CppDFAMemoryLeak
	const auto scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setWidgetResizable(true);

	_scrollAreaWidget = new QWidget();
	_scrollAreaLayout = new QVBoxLayout();

	_scrollAreaWidget->setLayout(_scrollAreaLayout);
	SetTransparentBackground(_scrollAreaWidget);
	_scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);

	scrollArea->setWidget(_scrollAreaWidget);
	_vLayout->addWidget(scrollArea);
	_vLayout->addWidget(_buttonBox);
	//_vLayout->setVerticalSizeConstraint(QLayout::SetMaximumSize);
	_vLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);

	setLayout(_vLayout);

	connect(_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QCheckBox* BitfieldWidgetDialog::AddCheckBox(const QString& title)
{
	const auto box = new QCheckBox(title);
	box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_checkBoxes.append(box);
	_scrollAreaLayout->addWidget(box);
	
	return box;
}

QList<QCheckBox*> BitfieldWidgetDialog::AddCheckBoxes(const QStringList& list)
{
	for (size_t i = 0; i < list.count(); i++)
	{
		// ReSharper disable once CppDFAMemoryLeak
		const auto box = new QCheckBox(list.at(i));
		box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		
		_scrollAreaLayout->addWidget(box);

		_checkBoxes.append(box);
	}

	return _checkBoxes;
}

void BitfieldWidgetDialog::SetInitialValues(const size_t value) const
{
	for (size_t i = 0; i < _checkBoxes.size(); i++)
	{
		_checkBoxes.at(i)->setChecked(value >> i & 1);
	}
}

QList<bool> BitfieldWidgetDialog::GetValuesAsList() const
{
	QList<bool> output;
	
	for (auto& box : _checkBoxes)
	{
		output.append(box->isChecked());
	}
	
	return output;
}

size_t BitfieldWidgetDialog::GetValues() const
{
	size_t output = 0;

	for (size_t i = 0; i < _checkBoxes.count() && i < sizeof(size_t)*8; i++)
	{
		if (_checkBoxes.at(i)->isChecked()) {
			output |= 1ULL << i;
		} else {
			output &= ~(1ULL << i);
		}
	}

	return output;
}
