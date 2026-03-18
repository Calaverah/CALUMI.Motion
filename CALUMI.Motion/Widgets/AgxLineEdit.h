//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#pragma warning(pop)

class AgxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	AgxHighlighter(QTextDocument* parent = nullptr);

signals:
	void HighlighterUpdated();

protected:
	void highlightBlock(const QString& text) override;
};


class AgxLineEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	explicit AgxLineEdit(QWidget* parent = nullptr);
	~AgxLineEdit() override;

	void SetUpDoubleClickEdit();
	void SetUpSingleClickEdit();

	QString text() const;
	void setValidator(QValidator* validator = nullptr);

signals:
	void Clicked();
	void DoubleClicked();
	void EditingFinished(const QString& str);
	void ValidatorUpdated();

public slots:
	void RefreshTooltip(const QString& str);
	void RefreshText();
	void adjustHeight();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void showEvent(QShowEvent*) override;
	
protected:
	qreal getLineHeight();

protected:
	QString _placeHolderText;
	AgxHighlighter* _highlighter;
	QValidator* _validator;
	bool _enabledState = true;
	bool _readOnlyState = false;
	qreal _lineMargin = 15;
};

class AgxLineEditContainer : public QWidget
{
	Q_OBJECT
public:
	AgxLineEditContainer(QWidget* parent = nullptr, bool allowDisable = true, Qt::WindowFlags f = Qt::WindowFlags());
	~AgxLineEditContainer();

	void setContentState(bool enable);
	void setAsButton(bool singleClick = false);
	void setValidator(QValidator* validator = nullptr);
	void setContentFixedWidth(int width);
	void setContentMinWidth(int width);
	void setContentMaxWidth(int width);
	void setCheckbox(bool enabled);
	void setContentPlaceholderText(const QString& text);
	QString text() const;

public slots:
	void onContentStateChanged(bool enabled);
	void setContentText(const QString& string);
	void RefreshContentTooltip(const QString& str);
	void SetContentAlignment(Qt::Alignment alignment);

signals:
	void ContentStateChanged(bool enabled);
	void ContentDoubleClicked();
	void ContentClicked();
	void ContentEditingFinished(const QString& str);
	void ContentValidatorUpdated();

public:
	void SetEqDecoder(bool state = true);

private:
	AgxLineEdit* _lineEdit;
	QCheckBox* _checkBox;
	QWidget* _lineContainer;
	bool _eqDecoder = true;
};