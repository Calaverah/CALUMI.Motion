//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxCommentWidget.h"
#include "Widgets/TextEditDialog.h"
#include "Utilities/AgxJsonHelper.h"
#include "Utilities/QWidgetFactories.h"
#include <QString>
#include <utility>


AgxCommentWidget::AgxCommentWidget(QWidget* parent, QStringList  keyPath) :
QScrollArea(parent),
_keyPath(std::move(keyPath))
{
	SetUpLabel();
	SetUpContextMenu();
}

AgxCommentWidget::AgxCommentWidget(const QString& text, QWidget* parent) :
QScrollArea(parent)
{
	SetUpLabel();
	label->setAttribute(Qt::WA_TransparentForMouseEvents);
	viewport()->setAttribute(Qt::WA_TransparentForMouseEvents);
	SetUpContextMenu();
	setCommentText(text);
}

void AgxCommentWidget::setCommentFont(const QFont& font) const
{
	if (label)
		label->setFont(font);
}

void AgxCommentWidget::setCommentText(const QString & text) const
{
	if (label)
		label->setText(text);
}

QString AgxCommentWidget::text() const
{
	if (label)
		return label->text();

	return {};
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

	const auto filter = new SimpleScrollAreaEventFilter(this);
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


	SetTransparentBackground(this);
	SetTransparentBackground(label);

	setLineWidth(4);
	setMidLineWidth(1);
}

void AgxCommentWidget::SetUpContextMenu()
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QLabel::customContextMenuRequested, this, [this](const QPoint& pos) {
		const auto cMenu = new QMenu();
		const auto editAction = new QAction("Edit Comment");
		cMenu->addAction(editAction);

		connect(editAction, &QAction::triggered, this, [this] {

			const auto dialog = new TextEditDialog(nullptr, this->text(), this->font());
			if (dialog->exec() == QDialog::Accepted)
				{
					QJsonObject output;
					output["text"] = dialog->GetText();
					output["font"] = dialog->GetFont().toString();
					SendInsertPropertySheetDataCommand(QStringListToQJsonObject(_keyPath, output));
				}
				dialog->deleteLater();
		});

		const QPoint scaledPos = mapToAgxView(pos);

		cMenu->exec(scaledPos);

	});

}

void ReadOnlyLabel::mousePressEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
		{ }
}

void ReadOnlyLabel::mouseMoveEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
		{ }
}

void ReadOnlyLabel::mouseReleaseEvent(QMouseEvent* event) {
	event->ignore();
	if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
		{ }
}

bool SimpleScrollAreaEventFilter::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		if (const auto mouseEvent = dynamic_cast<QMouseEvent*>(event))
		{
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
	}
	return QObject::eventFilter(object, event);
}
