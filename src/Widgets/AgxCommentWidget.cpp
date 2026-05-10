//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxCommentWidget.h"
#include "Widgets/TextEditDialog.h"
#include "Utilities/AgxJsonHelper.h"







AgxCommentWidget::AgxCommentWidget(QWidget* parent, QStringList keyPath) : QScrollArea(parent), IAgxEmbedSceneData(), _keyPath(keyPath)
{
	SetUpLabel();
	SetUpContextMenu();
}

AgxCommentWidget::AgxCommentWidget(const QString& text, QWidget* parent) : QScrollArea(parent), IAgxEmbedSceneData()
{
	SetUpLabel();
	label->setAttribute(Qt::WA_TransparentForMouseEvents);
	viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);
	SetUpContextMenu();
	setText(text);
}

void AgxCommentWidget::setFont(const QFont& font)
{
	if (label)
		label->setFont(font);
}

void AgxCommentWidget::setText(const QString & text)
{
	if (label)
		label->setText(text);
}

QString AgxCommentWidget::text() const
{
	if (label)
		return label->text();

	return QString();
}

void AgxCommentWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	//if(event->button() != Qt::RightButton)
	{
		event->ignore();
		return;
	}
	QScrollArea::mousePressEvent(event);
}

void AgxCommentWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
		//if (event->button() != Qt::RightButton)
	{
		event->ignore();
		return;
	}
	QScrollArea::mouseMoveEvent(event);
}

void AgxCommentWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	//if (event->button() != Qt::RightButton)
	{
		event->ignore();
		return;
	}
	QScrollArea::mouseReleaseEvent(event);
}

void AgxCommentWidget::SetUpLabel()
{
	//setFocusPolicy(Qt::FocusPolicy::ClickFocus);
	setWidgetResizable(true);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	SimpleScrollAreaEventFilter* filter = new SimpleScrollAreaEventFilter(this);
	viewport()->installEventFilter(filter);
	installEventFilter(filter);

	QString text;
	if (label)
	{
		text = label->text();
		label->deleteLater();
	}

	label = new ReadOnlyLabel(text);
	setWidget(label);

	label->setWordWrap(true);

	setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
	label->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	
	setStyleSheet("QScrollArea {background-color: transparent;}"); // background-color: #1526BE20
	label->setStyleSheet("QLabel {background-color: transparent;}"); // background-color: #1526BE20
	
	
	label->setFrameStyle(QFrame::Box | QFrame::Sunken);
	setLineWidth(4);
	setMidLineWidth(1);
	//label->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	//label->setFocusPolicy(Qt::NoFocus);
}

void AgxCommentWidget::SetUpContextMenu()
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QLabel::customContextMenuRequested, this, [this](const QPoint& pos) {
		QMenu* cMenu = new QMenu();
		QAction* editAction = new QAction("Edit Comment");
		cMenu->addAction(editAction);

		connect(editAction, &QAction::triggered, this, [this]() {

			auto dialog = new TextEditDialog(nullptr, this->text(), this->font());
			if (dialog->exec() == QDialog::Accepted) {
				QJsonObject output;
				output["text"] = dialog->GetText();
				output["font"] = dialog->GetFont().toString();
				SendInsertPropertySheetDataCommand(QStringListToQJsonObject(_keyPath, output));
			}
				if (dialog) dialog->deleteLater();
				});

		QPoint scaledPos = mapToAgxView(pos);

		cMenu->exec(scaledPos);

			});

}

void ReadOnlyLabel::mousePressEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		return;
	}
}

void ReadOnlyLabel::mouseMoveEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		return;
	}
}

void ReadOnlyLabel::mouseReleaseEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
	{
		return;
	}
}

bool SimpleScrollAreaEventFilter::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::MiddleButton || (mouseEvent->modifiers() & Qt::AltModifier) != 0)
		{
			event->ignore();
			return false;
		}
		if (mouseEvent->button() == Qt::LeftButton)
		{
			event->ignore();
			return false;
		}
	}
	return QObject::eventFilter(object, event);
}
