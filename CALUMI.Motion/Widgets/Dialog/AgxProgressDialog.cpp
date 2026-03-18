//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxProgressDialog.h"

AgxProgressDialog::AgxProgressDialog(const QString& labelText, const QString& cancelButtonText, int minimum, int maximum, QWidget* parent, Qt::WindowFlags flags) : QProgressDialog(labelText, cancelButtonText, minimum, maximum, parent, flags)
{
	setWindowModality(Qt::WindowModal);
	setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
	setAutoClose(true);
	setCancelButton(nullptr);
}

AgxProgressDialog::~AgxProgressDialog()
{}

void AgxProgressDialog::keyPressEvent(QKeyEvent * event)
{
	event->ignore();
	
	//if (event->key() == Qt::Key_Escape)
	//{
	//	event->ignore();
	//}
	//else
	//{
	//	QProgressDialog::keyPressEvent(event);
	//}
}

