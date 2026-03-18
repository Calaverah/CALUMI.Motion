//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxSimpleDialog.h"

TermRef AgxSimpleDialog::GetDropDown(QWidget* parent, const QString& title, const QString& label, const QList<TermRef>& items, int current, bool editable, bool* ok, Qt::WindowFlags flags)
{
    QDialog* dialog = new QDialog(parent, flags);
    dialog->setWindowTitle(title);
    dialog->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    

    QVBoxLayout* topLayout = new QVBoxLayout();
    topLayout->setSizeConstraint(QLayout::SetFixedSize);
    QFormLayout* frmLayout = new QFormLayout();
    frmLayout->setSizeConstraint(QLayout::SetFixedSize);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSizeConstraint(QLayout::SetFixedSize);

    QComboBox* comboBox = new QComboBox();
    {
        comboBox->setEditable(editable);
        for (auto ref : items) {
            comboBox->addItem(ref().translation);
        }
        comboBox->setCurrentIndex(current);
        comboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        frmLayout->addRow(label, comboBox);
        topLayout->addLayout(frmLayout);
    }

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    {
        buttonBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        buttonLayout->addStretch(1);
        buttonLayout->addWidget(buttonBox);
        topLayout->addLayout(buttonLayout);
    }

    dialog->setLayout(topLayout);

    dialog->adjustSize();
    
    if(ok)
        *ok = false;

    TermRef output = nullptr;

    if (dialog->exec() == QDialog::Accepted) {
        
        if(ok)
            *ok = true;

        output = items.at(comboBox->currentIndex());
    }

    dialog->deleteLater();
    
    return output;
}
