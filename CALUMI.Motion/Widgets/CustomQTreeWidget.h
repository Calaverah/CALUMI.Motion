//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QTreeWidget>
#pragma warning(pop)




class CustomQTreeWidget : public QTreeWidget
{
	Q_OBJECT
public:

	explicit CustomQTreeWidget(QWidget* parent = nullptr);

public Q_SLOTS:
	void onDeselectCalled();

Q_SIGNALS:
	void deselectCalled();
	void mousePressEventCalled();

protected:
	void mousePressEvent(QMouseEvent* event) override;
};

