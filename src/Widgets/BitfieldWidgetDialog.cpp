//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "BitfieldWidgetDialog.h"

#include "Utilities/QWidgetFactories.h"

BitfieldWidgetDialog::BitfieldWidgetDialog(QWidget* parent) : QDialog(parent, Qt::Dialog | Qt::WindowStaysOnTopHint)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

	setModal(true);

	m_vLayout = new QVBoxLayout();
	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	m_buttonBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	// ReSharper disable once CppDFAMemoryLeak
	const auto scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setWidgetResizable(true);

	m_scrollAreaWidget = new QWidget();
	m_scrollAreaLayout = new QVBoxLayout();

	m_scrollAreaWidget->setLayout(m_scrollAreaLayout);
	SetTransparentBackground(m_scrollAreaWidget);
	m_scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);

	scrollArea->setWidget(m_scrollAreaWidget);
	m_vLayout->addWidget(scrollArea);
	m_vLayout->addWidget(m_buttonBox);
	//_vLayout->setVerticalSizeConstraint(QLayout::SetMaximumSize);
	m_vLayout->setHorizontalSizeConstraint(QLayout::SetMinimumSize);

	setLayout(m_vLayout);

	connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QCheckBox* BitfieldWidgetDialog::AddCheckBox(const QString& title)
{
	const auto box = new QCheckBox(title);
	box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_checkBoxes.append(box);
	m_scrollAreaLayout->addWidget(box);
	
	return box;
}

QList<QCheckBox*> BitfieldWidgetDialog::AddCheckBoxes(const QStringList& list)
{
	for (size_t i = 0; i < list.count(); i++)
	{
		// ReSharper disable once CppDFAMemoryLeak
		const auto box = new QCheckBox(list.at(i));
		box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		
		m_scrollAreaLayout->addWidget(box);

		m_checkBoxes.append(box);
	}

	return m_checkBoxes;
}

void BitfieldWidgetDialog::SetInitialValues(const size_t value) const
{
	for (size_t i = 0; i < m_checkBoxes.size(); i++)
	{
		m_checkBoxes.at(i)->setChecked(value >> i & 1);
	}
}

QList<bool> BitfieldWidgetDialog::GetValuesAsList() const
{
	QList<bool> output;
	
	for (auto& box : m_checkBoxes)
	{
		output.append(box->isChecked());
	}
	
	return output;
}

size_t BitfieldWidgetDialog::GetValues() const
{
	size_t output = 0;

	for (size_t i = 0; i < m_checkBoxes.count() && i < sizeof(size_t)*8; i++)
	{
		if (m_checkBoxes.at(i)->isChecked()) {
			output |= 1ULL << i;
		} else {
			output &= ~(1ULL << i);
		}
	}

	return output;
}
