#pragma once

#pragma warning(push,0)
#include <QTextEdit>
#pragma warning(pop)
#include "IAgxEmbedSceneData.h"

class SimpleScrollAreaEventFilter : public QObject
{
	Q_OBJECT
public:
	explicit SimpleScrollAreaEventFilter(QObject* parent = nullptr) : QObject(parent) {}

protected:
	bool eventFilter(QObject* object, QEvent* event) override;

};

class ReadOnlyLabel : public QLabel
{
	Q_OBJECT
public:
	using QLabel::QLabel;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
};

class AgxCommentWidget : public QScrollArea, public IAgxEmbedSceneData
{
	Q_OBJECT

public:
	void setFont(const QFont& font);
	void setText(const QString& text);
	QString text() const;

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void SetUpLabel();

	void SetUpContextMenu();


public:
	AgxCommentWidget(QWidget* parent = nullptr, QStringList keyPath = {""});
	AgxCommentWidget(const QString& text, QWidget* parent = nullptr);
	~AgxCommentWidget() = default;

private:
	QStringList _keyPath;

	ReadOnlyLabel* label;
};

