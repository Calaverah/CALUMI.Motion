// ReSharper disable CppDFAMemoryLeak
// ReSharper disable CppTooWideScope
#include "stdafx.h"
#include "SFBGS_GraphPropertiesDialogWidget.h"
#include "Utilities/UndoRedoCommands.h"
#include "Widgets/Dialog/AgxSimpleDialog.h"
#include "../../Utilities/Settings/SettingsRegistry.h"

SFBGS_GraphPropertiesDialogWidget::SFBGS_GraphPropertiesDialogWidget(AgxGraphicsScene& scene, QWidget* parent) : QWidget(parent)
{
	_mainLayout = new QGridLayout();
	_mainLayout->setContentsMargins(0, 0, 10, 0);

	const auto titleLayout = new QHBoxLayout();
	const auto titleLabel = new QLabel("Title: ");
	auto graphTitleLabel = new QLabel(scene.agxGraphModel().GetGraphTitle());
	titleLayout->addWidget(titleLabel, 1, Qt::AlignLeft | Qt::AlignBottom);
	titleLayout->addWidget(graphTitleLabel, 0, Qt::AlignLeft | Qt::AlignBottom);

	const auto graphCategoryLayout = new QHBoxLayout();
	const auto graphCategoryLabel = new QLabel("Category: ");
	auto graphCategoryCombo = new QComboBox();
	graphCategoryCombo->insertItems(0, { "<none>" });

	QSet<QString> catSet = _categoryList;
	catSet.unite(SettingsRegistry::GetInstance().GetCustomCategories(AgxGameType::SFBGS, true));
	const QString currentCategory = scene.agxGraphModel().getGraphCategory();

	if(currentCategory.compare("NONE",Qt::CaseInsensitive) != 0 && currentCategory.compare("<none>",Qt::CaseInsensitive) != 0)
	{
		if (!catSet.contains(currentCategory)) {
			SettingsRegistry::GetInstance().AddCustomCategory(currentCategory, AgxGameType::SFBGS);
			catSet.insert(currentCategory);
		}
	}

	QStringList comboList = catSet.values();
	comboList.sort(Qt::CaseInsensitive);

	graphCategoryCombo->insertItems(1, comboList);
	graphCategoryCombo->setCurrentText(currentCategory);

	graphCategoryCombo->setMaximumHeight(graphCategoryLabel->height()-5);
	graphCategoryLayout->addWidget(graphCategoryLabel, 1, Qt::AlignLeft | Qt::AlignBottom);
	graphCategoryLayout->addWidget(graphCategoryCombo, 0, Qt::AlignLeft | Qt::AlignBottom);

	const auto graphTypeLayout = new QHBoxLayout();
	const auto graphTypeLabel = new QLabel("Graph Type: ");
	const auto graphTypeTitle = new QLabel(AgxGraphTypeAsString(scene.agxGraphModel().getGraphType()));
	graphTypeTitle->setDisabled(true);
	
	auto aFont = graphTypeLabel->font();
	aFont.setBold(true);
	titleLabel->setFont(aFont);
	graphTypeLabel->setFont(aFont);
	graphCategoryLabel->setFont(aFont);

	auto bFont = graphTypeTitle->font();
	bFont.setPointSizeF(bFont.pointSizeF() + 1);
	graphTypeTitle->setFont(bFont);

	graphTypeLayout->addWidget(graphTypeLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
	graphTypeLayout->addWidget(graphTypeTitle, 1, Qt::AlignLeft | Qt::AlignBottom);

	const auto graphTypeButton = new QPushButton("Change Type");
	graphTypeLayout->addWidget(graphTypeButton);

	auto scenePtr = &scene;
	connect(graphTypeButton, &QPushButton::pressed, this, [this, scenePtr] {
		if (scenePtr)
		{
			bool res = false;
			const int current = scenePtr->agxGraphModel().getGraphType() == AgxGraphType::SFBGS_Default ? 1 : 0;
			//QString result = QInputDialog::getItem(this, "Graph Types", "Select New Type", { "Default","State Machine" }, current, false, &res);

			const TermRef refResult = AgxSimpleDialog::GetDropDown(this, tr("Graph Types"), tr("Select New Graph Type"), {&AgxDictionary::Default, &AgxDictionary::StateMachine}, current, false, &res);

			if (res) {
				if (refResult == &AgxDictionary::Default && scenePtr->agxGraphModel().getGraphType() != AgxGraphType::SFBGS_Default) { 
					scenePtr->undoStack().push(new SetNewGraphPropertiesCommand(scenePtr, AgxGraphType::SFBGS_Default)); 
				} 
				else if (refResult == &AgxDictionary::StateMachine && scenePtr->agxGraphModel().getGraphType() != AgxGraphType::SFBGS_StateMachine) { 
					scenePtr->undoStack().push(new SetNewGraphPropertiesCommand(scenePtr, AgxGraphType::SFBGS_StateMachine)); 
				}
			}
			}
			});


	//AgxGraphModel* modelRef = &scene.agxGraphModel();
	const AgxGraphModel* rootModelRef = scene.agxGraphModel().rootGraphReference();

	if (rootModelRef)
	{
		connect(rootModelRef, &AgxGraphModel::PropertySheetUpdated, graphCategoryCombo, [rootModelRef, graphCategoryCombo] {
			QString text = rootModelRef->getGraphCategory();
			if (text == "NONE")
				text = "<none>";
			graphCategoryCombo->blockSignals(true);
			graphCategoryCombo->setCurrentText(text);
			graphCategoryCombo->blockSignals(false);
				});

		connect(rootModelRef, &AgxGraphModel::PropertySheetUpdated, graphTitleLabel, [rootModelRef, graphTitleLabel] {
			graphTitleLabel->blockSignals(true);
			graphTitleLabel->setText(rootModelRef->GetGraphTitle());
			graphTitleLabel->blockSignals(false);
				});
	}
	connect(graphCategoryCombo, &QComboBox::currentTextChanged, this, [&scene](const QString& text) {
				QString output = text;

				if (output.compare("<none>", Qt::CaseInsensitive) == 0)
				{
					output = "NONE";
				}

				scene.undoStack().push(new AgxSetGraphCategory(&scene.agxGraphModel(), output));
		});
	
	_mainLayout->addLayout(titleLayout,0,0);
	_mainLayout->addLayout(graphCategoryLayout,1,0);
	_mainLayout->addLayout(graphTypeLayout,2,0);

	_propertyWidgets = new AgxNodePropertiesWidget();
	connect(_propertyWidgets, &AgxNodePropertiesWidget::BroadcastWidth, this, &SFBGS_GraphPropertiesDialogWidget::SetWidth);
	
	_scrollArea = new QScrollArea();
	_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_scrollArea->setWidget(_propertyWidgets);
	_scrollArea->setWidgetResizable(true);
	_scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	_propertyWidgets->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	_mainLayout->addWidget(_scrollArea, 3, 0);

	setLayout(_mainLayout);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

	_propertyWidgets->SetRefData(&scene.agxGraphModel(),&scene);

	_propertyWidgets->CreatePropertyEntries(scene.agxGraphModel().GetPropertyEntries(), &scene.agxGraphModel());

	const auto blocks = scene.agxGraphModel().GetPropertyBlocks();
	const auto& keys = scene.agxGraphModel().m_blockOrder;
	for (unsigned int i = 0; i < blocks->count() && i < keys.count(); i++)
	{
		const auto key = keys.at(i);
		_propertyWidgets->CreatePropetryBlock(key,*scene.agxGraphModel().getPropertyBlock(key));
	}

	if (scene.agxGraphModel().rootGraphReference() != &scene.agxGraphModel())
	{
		graphCategoryCombo->setDisabled(true);
		graphCategoryLabel->setDisabled(true);
		titleLabel->setDisabled(true);
		graphTitleLabel->setDisabled(true);
	}
}

void SFBGS_GraphPropertiesDialogWidget::SetWidth(const int width) const
{
	_scrollArea->setFixedWidth(width);
}
