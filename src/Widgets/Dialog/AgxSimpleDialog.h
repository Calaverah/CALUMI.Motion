//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QDialog>
#pragma warning(pop)
#include "Utilities/AgxDictionary.h"

class AgxSimpleDialog : public QDialog {

	Q_OBJECT
public:
	
	static TermRef GetDropDown(QWidget* parent, const QString& title, const QString& label,
							   const QList<TermRef>& items, int current = 0, bool editable = true,
							   bool* ok = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

};