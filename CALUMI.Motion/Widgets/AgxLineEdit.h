#pragma once

#pragma warning(push,0)
#include <QLineEdit>
#pragma warning(pop)

class AgxLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	explicit AgxLineEdit(QWidget* parent = nullptr);
	~AgxLineEdit() override;

	void SetUpDoubleClickEdit();
	void SetUpSingleClickEdit();

signals:
	void Clicked();
	void DoubleClicked();

public slots:
	void RefreshTooltip(const QString& str);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;


	QString _placeHolderText;
};