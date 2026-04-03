//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxLineEdit.h"
#include "Utilities/AgxFormat.h"
#include <Utilities/SettingsRegistry.h>

AgxLineEdit::AgxLineEdit(QWidget* parent) : QPlainTextEdit(parent), _validator(nullptr)
{
	connect(this, &AgxLineEdit::EditingFinished, this, &AgxLineEdit::RefreshTooltip);
	setContextMenuPolicy(Qt::NoContextMenu);

	setFrameShape(QFrame::StyledPanel);
	setFrameShadow(QFrame::Sunken);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

	setLineWrapMode(QPlainTextEdit::WidgetWidth);
	setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

	setFixedHeight(30);

	connect(this, &QPlainTextEdit::textChanged, this, &AgxLineEdit::adjustHeight);

	_highlighter = new AgxHighlighter(this->document());

	connect(_highlighter, &AgxHighlighter::HighlighterUpdated, this, &AgxLineEdit::RefreshText);

	adjustHeight();
}

AgxLineEdit::~AgxLineEdit()
{
	
}

void AgxLineEdit::RefreshTooltip(const QString& str)
{
	if (str == "")
	{
		setToolTip("");
	} else
	{
		setToolTip(_placeHolderText);
	}
}

void AgxLineEdit::RefreshText()
{
	if (text().isEmpty()) 
		return;

	setPlainText(text());
}

void AgxLineEdit::setupButtonStyle()
{
	viewport()->setCursor(Qt::PointingHandCursor);
	setReadOnly(true);
	setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

	auto pal = palette();
	pal.setColor(QPalette::Base, palette().color(QPalette::Button));
	pal.setColor(QPalette::ButtonText, palette().color(QPalette::ButtonText));
	setPalette(pal);

	setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void AgxLineEdit::SetUpDoubleClickEdit()
{
	setupButtonStyle();
	_placeHolderText = "Double Click To Edit";
	setPlaceholderText(_placeHolderText);
	connect(this, &AgxLineEdit::DoubleClicked, this, [this]() { /*deselect();*/ });
}

void AgxLineEdit::SetUpSingleClickEdit()
{
	setupButtonStyle();
	_placeHolderText = "Click To Edit";
	setPlaceholderText(_placeHolderText);
	connect(this, &AgxLineEdit::Clicked, this, [this]() { /*deselect();*/ });
}

QString AgxLineEdit::text() const
{
	QString output = toPlainText();
	output.remove("\n");
	output.remove("\r");
	output.remove("\f");
	output.remove("\a");
	output.remove("\b");
	output.remove("\t");
	output.remove("\v");

	if(_validator)
	{
		int pos = 0;
		auto result = _validator->validate(output, pos);

		if (result != QValidator::State::Acceptable)
			qWarning() << "AgxLineEdit Entry Does Meet Validation Criteria. Please Check Entry: " << output;
	}

	return output;
}

void AgxLineEdit::setValidator(QValidator* validator)
{
	_validator = validator;
	Q_EMIT ValidatorUpdated();
}

void AgxLineEdit::adjustHeight()
{
	document()->adjustSize();
	qreal newHeight = document()->size().toSize().height()*getLineHeight() + _lineMargin;
	setFixedHeight(qMax(getLineHeight(), newHeight));

	QScrollBar* vScrollBar = verticalScrollBar();
	vScrollBar->setValue(vScrollBar->minimum());
}

void AgxLineEdit::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		event->ignore();
		return;
	}
	QPlainTextEdit::mousePressEvent(event);
	Q_EMIT Clicked();
}
void AgxLineEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		event->ignore();
		return;
	}
	QPlainTextEdit::mouseDoubleClickEvent(event);
	Q_EMIT DoubleClicked();
}

void AgxLineEdit::focusOutEvent(QFocusEvent* event)
{
	QPlainTextEdit::focusOutEvent(event);

	QTextCursor cursor = textCursor();
	cursor.clearSelection();
	setTextCursor(cursor);

	Q_EMIT EditingFinished(text());
}

void AgxLineEdit::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Backspace) {
		QPlainTextEdit::keyPressEvent(event);
		return;
	}

	QValidator::State result = QValidator::Acceptable;

	if (_validator)
	{
		QString proposal = toPlainText() + event->text();
		int pos = 0;
		result = _validator->validate(proposal, pos);
	}

	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		if ((event->modifiers() & Qt::ShiftModifier) != 0 && result == QValidator::Acceptable)
		{
			//we can newline for now, but will need to remove when getting text
		}
		else
		{
			clearFocus();
			event->accept();
			return;
		}
	}

	if (result != QValidator::Acceptable)
	{
		event->ignore();
		return;
	}

	QPlainTextEdit::keyPressEvent(event);
}

void AgxLineEdit::showEvent(QShowEvent*)
{
	adjustHeight();
}

qreal AgxLineEdit::getLineHeight()
{
	return QFontMetricsF(font()).height();
}

AgxHighlighter::AgxHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
}

void AgxHighlighter::highlightBlock(const QString& text)
{
	if (text.isEmpty()) return;

	for (const auto& rule : SettingsRegistry::GetInstance().GetHighlightingRules()) {
		QRegularExpressionMatchIterator i = rule.pattern.globalMatch(text);
		while (i.hasNext()) {
			QRegularExpressionMatch match = i.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
}

AgxLineEditContainer::AgxLineEditContainer(QWidget* parent, bool allowDisable, Qt::WindowFlags f) : QWidget(parent, f), _lineEdit(new AgxLineEdit()), _checkBox(new QCheckBox()), _lineContainer(new QWidget())
{
	QGridLayout* containerLayout = new QGridLayout();
	containerLayout->setContentsMargins(0, 0, 0, 0);
	containerLayout->addWidget(_lineEdit);
	_lineContainer->setContentsMargins(0, 0, 0, 0);
	_lineContainer->setLayout(containerLayout);

	QHBoxLayout* hBox = new QHBoxLayout();
	hBox->setContentsMargins(0, 0, 0, 0);
	
	hBox->addWidget(_lineContainer);
	hBox->addWidget(_checkBox);
	_checkBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	setLayout(hBox);
	connect(_checkBox, &QCheckBox::toggled, this, &AgxLineEditContainer::onContentStateChanged);
	connect(_lineEdit, &AgxLineEdit::Clicked, this, &AgxLineEditContainer::ContentClicked);
	connect(_lineEdit, &AgxLineEdit::DoubleClicked, this, &AgxLineEditContainer::ContentDoubleClicked);
	connect(_lineEdit, &AgxLineEdit::EditingFinished, this, &AgxLineEditContainer::ContentEditingFinished);
	connect(_lineEdit, &AgxLineEdit::ValidatorUpdated, this, &AgxLineEditContainer::ContentValidatorUpdated);

	setCheckbox(allowDisable);

	hBox->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	_lineContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	setContentsMargins(0, 0, 0, 0);

	containerLayout->setSizeConstraint(QLayout::SetFixedSize);
	hBox->setSizeConstraint(QLayout::SetFixedSize);
}

AgxLineEditContainer::~AgxLineEditContainer()
{}

void AgxLineEditContainer::setContentState(bool enable)
{
	blockSignals(true);
	_checkBox->blockSignals(true);
	_lineContainer->blockSignals(true);

	_checkBox->setChecked(enable);
	_lineContainer->setEnabled(enable);

	_lineContainer->blockSignals(false);
	_checkBox->blockSignals(false);
	blockSignals(false);
}

void AgxLineEditContainer::setAsButton(bool singleClick)
{
	if (singleClick)
		_lineEdit->SetUpSingleClickEdit();
	else
		_lineEdit->SetUpDoubleClickEdit();	
}

void AgxLineEditContainer::setValidator(QValidator* validator)
{
	_lineEdit->setValidator(validator);
}

void AgxLineEditContainer::setContentFixedWidth(int width)
{
	_lineEdit->setFixedWidth(width);
	adjustSize();
}

void AgxLineEditContainer::setContentMinWidth(int width)
{
	_lineEdit->setMinimumWidth(width);
	adjustSize();
}

int AgxLineEditContainer::getContentMinWidth() const
{
	return _lineEdit->minimumWidth();
}

void AgxLineEditContainer::setContentMaxWidth(int width)
{
	_lineEdit->setMaximumWidth(width);
	adjustSize();
}

void AgxLineEditContainer::setCheckbox(bool enabled)
{
	_checkBox->setEnabled(enabled);
	_checkBox->setVisible(enabled);
	adjustSize();
}

void AgxLineEditContainer::setContentPlaceholderText(const QString& text)
{
	_lineEdit->setPlaceholderText(text);
}

QString AgxLineEditContainer::text() const
{
	if (_eqDecoder)
		return ConvertEquationToText(_lineEdit->text());

	return _lineEdit->text();
}

void AgxLineEditContainer::setContentText(const QString& string)
{
	QString text = string;
	_lineEdit->blockSignals(true);

	if (_eqDecoder)
		text = ConvertTextToEquation(string);
		
	_lineEdit->setPlainText(text);
	_lineEdit->adjustHeight();
	_lineEdit->blockSignals(false);
}

void AgxLineEditContainer::RefreshContentTooltip(const QString& str)
{
	_lineEdit->RefreshTooltip(str);
}

void AgxLineEditContainer::SetContentAlignment(Qt::Alignment alignment)
{
	//Will have to revisit
	adjustSize();
}

void AgxLineEditContainer::onContentStateChanged(bool enabled)
{
	_lineContainer->setEnabled(enabled);
	Q_EMIT ContentStateChanged(enabled);
}

void AgxLineEditContainer::SetEqDecoder(bool state)
{
	_eqDecoder = state;
}

void AgxLineEditContainer::enterEvent(QEnterEvent* event)
{
	QWidget::enterEvent(event);

	Q_EMIT hovered();
}

void AgxLineEditContainer::leaveEvent(QEvent* event)
{
	QWidget::leaveEvent(event);

	Q_EMIT unhovered();
}
