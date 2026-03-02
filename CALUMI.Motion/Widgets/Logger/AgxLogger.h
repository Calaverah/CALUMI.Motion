#pragma once

#include <QWidget>

class AgxLogger  : public QWidget
{
	Q_OBJECT

public:
	explicit AgxLogger(QWidget *parent = nullptr);
	~AgxLogger();

public slots:
		void appendMessage(const QString& text, const QtMsgType& type);

protected:
	void closeEvent(QCloseEvent* event) override;
	void hideEvent(QHideEvent* event) override;

private:
	void writeFormatted(const QString& text, const QColor& color, bool bold = false);

private:
	QPlainTextEdit* _logTE = nullptr;
};

