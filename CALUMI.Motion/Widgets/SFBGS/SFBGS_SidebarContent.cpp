#include "stdafx.h"
#include "SFBGS_SidebarContent.h"
#include <Utilities/AgxJsonHelper.h>

SFBGS_SidebarContent::SFBGS_SidebarContent(QWidget* parent) : AgxSidebarContent(parent) {

	_titleBar = new QLabel();
	QVBoxLayout* mainLayout = new QVBoxLayout();
	_itemLayout = new QVBoxLayout();

	_mainItem = new QVBoxLayout();
	_subItems = new QVBoxLayout();
	_extraItems = new QVBoxLayout();

	_itemLayout->addLayout(_mainItem);
	_itemLayout->addLayout(_subItems);
	_itemLayout->addLayout(_extraItems);

	mainLayout->addWidget(_titleBar);
	
	_frame = new QFrame();
	_frame->setLayout(_itemLayout);
	_frame->setFrameShape(QFrame::Shape::Box);
	_frame->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	mainLayout->addWidget(_frame);

	connect(this, &SFBGS_SidebarContent::StateChanged, this, &SFBGS_SidebarContent::OnStateChanged);

	setLayout(mainLayout);
}

void SFBGS_SidebarContent::AddMainItem(QWidget* item, unsigned int idx, int stretch, Qt::Alignment alignment, bool asExtra) {
	
	QVBoxLayout* layout = asExtra ? _extraItems : _mainItem;

	if (layout) {
		idx = idx > static_cast<unsigned int>(layout->count()) ? idx = layout->count() : idx;
		layout->insertWidget(idx, item, stretch, alignment);
	}

	if (auto sfbgsWidget = dynamic_cast<AgxNodePropertiesWidget*>(item))
	{
		Q_EMIT sfbgsWidget->SetRefData(*this);

		connect(this, &SFBGS_SidebarContent::ReferenceInitialized, sfbgsWidget, [this, sfbgsWidget](const IAgxEmbedSceneData& source) { sfbgsWidget->SetRefData(source); });

		connect(sfbgsWidget, &AgxNodePropertiesWidget::BroadcastWidth, this, [this]() {
			adjustSize();
			Q_EMIT StateChanged();
				});

		adjustSize();
		QTimer::singleShot(1, this, [this]() {Q_EMIT StateChanged(); });
	}
}

void SFBGS_SidebarContent::AddContentItem(QWidget* item, unsigned int idx, int stretch, Qt::Alignment alignment, bool asExtra)
{
	QVBoxLayout* layout = asExtra ? _extraItems : _subItems;

	if (layout) {
		idx = idx > static_cast<unsigned int>(layout->count()) ? idx = layout->count() : idx;
		layout->insertWidget(idx, item, stretch, alignment);
	}

	if(auto sfbgsWidget = dynamic_cast<SFBGS_SidebarContentItem*>(item))
	{
		Q_EMIT sfbgsWidget->SetUpChildDataRefs(*this);

		connect(this, &SFBGS_SidebarContent::ReferenceInitialized, sfbgsWidget, [this, sfbgsWidget](const IAgxEmbedSceneData& source) { Q_EMIT sfbgsWidget->SetUpChildDataRefs(source); });

		connect(sfbgsWidget, &SFBGS_SidebarContentItem::StateChanged, this, [this]() { 
			adjustSize();
			Q_EMIT StateChanged(); 
				});

		adjustSize();
		QTimer::singleShot(1, this, [this]() {Q_EMIT StateChanged(); });
	}
}

void SFBGS_SidebarContent::OnStateChanged()
{
	QTimer::singleShot(1, this, [this]() {
		if (_frame->frameSize().height() < 30)
		{
			_frame->setFrameShape(QFrame::Shape::HLine);
			return;
		}

		_frame->setFrameShape(QFrame::Shape::Box);
					   });
}

SFBGS_SidebarContentItem::SFBGS_SidebarContentItem(QWidget* parent) : QWidget(parent) {
	_titleBar = new QLabel();
	_layout = new QVBoxLayout();

	QHBoxLayout* titleLayout = new QHBoxLayout();
	titleLayout->addWidget(_titleBar,1,Qt::AlignLeft);

	_togglePropertySheet = new QPushButton("Toggle Property Sheet");
	_togglePropertySheet->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	titleLayout->addWidget(_togglePropertySheet, Qt::AlignRight);
	//_togglePropertySheet->setVisible(_optionalPropertySheet);

	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

	_optionalPropertySheet = false;
	_togglePropertySheet->setVisible(false);

	_layout->addLayout(titleLayout);

	QFrame* hLine = new QFrame();
	hLine->setFrameShape(QFrame::HLine);
	hLine->setFrameShadow(QFrame::Sunken);
	_layout->addWidget(hLine);

	setLayout(_layout);
	Q_EMIT StateChanged();
}

void SFBGS_SidebarContentItem::SetupOptionalPropertySheet(bool setting, const bool* source, const QStringList& list) {
	
	_optionalPropertySheet = setting;
	_togglePropertySheet->setVisible(setting);

	if(setting)
	{
		SetOptionalPropertySheetState(*source);

		connect(_togglePropertySheet, &QPushButton::pressed, this, [this, list, source]() {
			if (!_propertySheet) return;
			QString ssetting = source && *source ? "False" : "True";
			_propertySheet->SendInsertPropertySheetDataCommand(QStringListToQJsonObject(list,ssetting));
			
				});
	}
}

void SFBGS_SidebarContentItem::SetOptionalPropertySheetState(bool state)
{
	_propertySheet->setVisible(state);
	adjustSize();
	Q_EMIT StateChanged();
}

AgxNodePropertiesWidget* SFBGS_SidebarContentItem::SetupPropertySheet()
{
	if (_propertySheet)
		_propertySheet->deleteLater();

	_propertySheet = new AgxNodePropertiesWidget(nullptr, true);
	_layout->addWidget(_propertySheet, 1,Qt::AlignRight);

	if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(_propertySheet))
	{
		connect(this, &SFBGS_SidebarContentItem::SetUpChildDataRefs, _propertySheet, [this, iagx](const IAgxEmbedSceneData& source) {
			iagx->SetRefData(source);
				});
	}

	_propertySheet->FinalizeWidget();
	return _propertySheet;
}

void SFBGS_SidebarContentItem::InsertAdditionalWidget(QWidget* widget, int stretch, Qt::Alignment alignment)
{
	_layout->addWidget(widget, stretch, alignment);

	if (auto iagx = dynamic_cast<IAgxEmbedSceneData*>(widget))
	{
		connect(this, &SFBGS_SidebarContentItem::SetUpChildDataRefs, widget, [this, iagx](const IAgxEmbedSceneData& source) {
					iagx->SetRefData(source);
				});
	}
}