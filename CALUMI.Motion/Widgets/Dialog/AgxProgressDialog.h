#pragma once

#include <QProgressDialog>

class AgxProgressDialog  : public QProgressDialog
{
	Q_OBJECT

public:
	AgxProgressDialog(const QString& labelText, const QString& cancelButtonText, int minimum, int maximum, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
	~AgxProgressDialog();

protected:
	void keyPressEvent(QKeyEvent* event) override;
};

