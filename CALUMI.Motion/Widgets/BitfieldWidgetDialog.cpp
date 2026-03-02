#include "stdafx.h"
#include "BitfieldWidgetDialog.h"

BitfieldWidgetDialog::BitfieldWidgetDialog(QWidget* parent)
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

	_vLayout = new QVBoxLayout();
	_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	_buttonBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	scrollArea->setWidgetResizable(true);

	_scrollAreaWidget = new QWidget();
	_scrollAreaLayout = new QVBoxLayout();

	_scrollAreaWidget->setLayout(_scrollAreaLayout);
	//_scrollAreaWidget->setStyleSheet("QWidget{border-width: 0px}");
	//scrollArea->setStyleSheet("QScrollArea{background-color:transparent;}");
	scrollArea->setStyleSheet(
		"QScrollArea {"
		"   border-width: 3px;"
		"   border-radius: 5px;"
		"   background-color:transparent;"
		"}"
		"QScrollBar:vertical {"
		"   width: 12px;"
		"   margin: 0px 0px 0px 0px;"
		"}"
	);
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
	QCheckBox* box = new QCheckBox(title);
	box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_checkBoxes.append(box);
	_scrollAreaLayout->addWidget(box);
	
	return box;
}

QList<QCheckBox*> BitfieldWidgetDialog::AddCheckBoxes(const QStringList& list)
{
	for (size_t i = 0; i < list.count(); i++)
	{
		QCheckBox* box = new QCheckBox(list.at(i));	
		box->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		
		_scrollAreaLayout->addWidget(box);

		_checkBoxes.append(box);
	}

	return _checkBoxes;
}

void BitfieldWidgetDialog::SetInitialValues(size_t value)
{
	for (size_t i = 0; i < _checkBoxes.size(); i++)
	{
		_checkBoxes.at(i)->setChecked((value >> i) & 1);
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

	for (size_t i = 0; i < _checkBoxes.count() && i < (sizeof(size_t)*8); i++)
	{
		if (_checkBoxes.at(i)->isChecked()) {
			output |= (1ULL << i);
		} else {
			output &= ~(1ULL << i);
		}
	}

	return output;
}
