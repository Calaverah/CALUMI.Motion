#include "stdafx.h"
#include "SFBGS_GraphPropertiesDialogWidget.h"
#include "Utilities/UndoRedoCommands.h"

SFBGS_GraphPropertiesDialogWidget::SFBGS_GraphPropertiesDialogWidget(AgxGraphicsScene& scene, QWidget* parent)
{
	_mainLayout = new QGridLayout();
	_mainLayout->setContentsMargins(0, 0, 10, 0);

	QHBoxLayout* graphCategoryLayout = new QHBoxLayout();
	QLabel* graphCategoryLabel = new QLabel("Category: ");
	QComboBox* graphCategoryCombo = new QComboBox();
	graphCategoryCombo->insertItems(0, { "<none>","<custom>" });
	auto tempList = _categoryList.values();
	tempList.sort();
	graphCategoryCombo->insertItems(2, tempList);
	graphCategoryCombo->setMaximumHeight(graphCategoryLabel->height()-5);
	graphCategoryLayout->addWidget(graphCategoryLabel, 1, Qt::AlignLeft | Qt::AlignBottom);
	graphCategoryLayout->addWidget(graphCategoryCombo, 0, Qt::AlignLeft | Qt::AlignBottom);

	QHBoxLayout* graphTypeLayout = new QHBoxLayout();
	QLabel* graphTypeLabel = new QLabel("Graph Type: ");
	QLabel* graphTypeTitle = new QLabel(AgxGraphTypeAsString(scene.agxGraphModel().getGraphType()));
	graphTypeTitle->setDisabled(true);
	
	auto aFont = graphTypeLabel->font();
	aFont.setBold(true);
	graphTypeLabel->setFont(aFont);
	graphCategoryLabel->setFont(aFont);

	auto bFont = graphTypeTitle->font();
	bFont.setPointSizeF(bFont.pointSizeF() + 1);
	graphTypeTitle->setFont(bFont);

	graphTypeLayout->addWidget(graphTypeLabel, 0, Qt::AlignLeft | Qt::AlignBottom);
	graphTypeLayout->addWidget(graphTypeTitle, 1, Qt::AlignLeft | Qt::AlignBottom);

	QPushButton* graphTypeButton = new QPushButton("Change Type");
	graphTypeLayout->addWidget(graphTypeButton);

	auto scenePtr = &scene;
	connect(graphTypeButton, &QPushButton::pressed, this, [this, scenePtr]() {
		if (scenePtr)
		{
			bool res = false;
			int current = scenePtr->agxGraphModel().getGraphType() == AgxGraphType::SFBGS_Default ? 1 : 0;
			QString result = QInputDialog::getItem(this, "Graph Types", "Select New Type", { "Default","State Machine" }, current, false, &res);
			if (res) {
				if (result == "Default" && scenePtr->agxGraphModel().getGraphType() != AgxGraphType::SFBGS_Default) { scenePtr->undoStack().push(new SetNewGraphPropertiesCommand(scenePtr, AgxGraphType::SFBGS_Default)); }
				else if (result == "State Machine" && scenePtr->agxGraphModel().getGraphType() != AgxGraphType::SFBGS_StateMachine) { scenePtr->undoStack().push(new SetNewGraphPropertiesCommand(scenePtr, AgxGraphType::SFBGS_StateMachine)); }
			}
			}
			});

	QString currentCategory = scene.agxGraphModel().getGraphCategory();
	if(currentCategory != "NONE")
	{
		if (!_categoryList.contains(currentCategory))
		{
			_categoryList.insert(currentCategory);
			graphCategoryCombo->clear();
			graphCategoryCombo->insertItems(0, { "<none>","<custom>" });
			auto tempList = _categoryList.values();
			tempList.sort();
			graphCategoryCombo->insertItems(2, tempList);
		}

		graphCategoryCombo->setCurrentText(currentCategory);
	}

	AgxGraphModel* modelRef = &scene.agxGraphModel();
	if(modelRef)
		connect(modelRef, &AgxGraphModel::PropertySheetUpdated, graphCategoryCombo, [this, modelRef, graphCategoryCombo]() {
		QString text = modelRef->getGraphCategory();
				if (text == "NONE" || text == "<custom>")
					text = "<none>";
				graphCategoryCombo->blockSignals(true);
				graphCategoryCombo->setCurrentText(text);
				graphCategoryCombo->blockSignals(false);
			});

	connect(graphCategoryCombo, &QComboBox::currentTextChanged, this, [this, &scene, graphCategoryCombo](const QString& text) {
				QString output = text;
				if (output == "<custom>")
				{
					QInputDialog dialog;
					dialog.setWindowTitle("Custom Input");
					dialog.setLabelText("Enter Category:");
					dialog.show();
					
					if (auto lineEdit = dialog.findChild<QLineEdit*>()) {
						qDebug() << "TEST";
						QRegularExpression rex("^[a-zA-Z0-9_]+$");
						QValidator* valdr = new QRegularExpressionValidator(rex, lineEdit);
						lineEdit->setValidator(valdr);
					}

					if (dialog.exec() == QDialog::Accepted) {
						output = dialog.textValue();
					}
				}

				if (text == "<none>" || output == "<none>")
				{
					output = "NONE";
				}
				
				//scene.agxGraphModel().setGraphCategory(output);
				scene.undoStack().push(new AgxSetGraphCategory(&scene.agxGraphModel(), output));

				graphCategoryCombo->blockSignals(true);
				if (!_categoryList.contains(output) && output != "NONE")
				{
					_categoryList.insert(output);
					graphCategoryCombo->clear();
					graphCategoryCombo->insertItems(0, { "<none>","<custom>" });
					auto tempList = _categoryList.values();
					tempList.sort();
					graphCategoryCombo->insertItems(2, tempList);
				}

				graphCategoryCombo->setCurrentText(output);
				graphCategoryCombo->blockSignals(false);
			});

	_mainLayout->addLayout(graphCategoryLayout,0,0);
	_mainLayout->addLayout(graphTypeLayout,1,0);

	_propertyWidgets = new AgxNodePropertiesWidget();
	connect(_propertyWidgets, &AgxNodePropertiesWidget::BroadcastWidth, this, &SFBGS_GraphPropertiesDialogWidget::SetWidth);
	
	_scrollArea = new QScrollArea();
	//_scrollArea->setStyleSheet(" QScrollArea { border: 1px solid lightgray; background-color: transparent; border-radius: 5px; } ");
	_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	_scrollArea->setWidget(_propertyWidgets);
	_scrollArea->setWidgetResizable(true);
	_scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	_propertyWidgets->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	_mainLayout->addWidget(_scrollArea, 2, 0);

	_buttons = new QDialogButtonBox(QDialogButtonBox::Close);
	_buttons->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	_mainLayout->addWidget(_buttons);

	setLayout(_mainLayout);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

	_propertyWidgets->SetRefData(&scene.agxGraphModel(),&scene);

	_propertyWidgets->CreatePropertyEntries(scene.agxGraphModel().GetPropertyEntries(), &scene.agxGraphModel(), false);
	
	auto blocks = scene.agxGraphModel().GetPropertyBlocks();
	auto keys = blocks->keys();
	for (unsigned int i = 0; i < blocks->count(); i++)
	{
		auto key = keys.at(i);
		_propertyWidgets->CreatePropetryBlock(key,*scene.agxGraphModel().getPropertyBlock(key));
	}

	if (scene.agxGraphModel().rootGraphReference() != &scene.agxGraphModel())
	{
		graphCategoryCombo->setDisabled(true);
		graphCategoryLabel->setDisabled(true);
	}
}

void SFBGS_GraphPropertiesDialogWidget::SetWidth(int width)
{
	_scrollArea->setFixedWidth(width);
}

QDialogButtonBox* SFBGS_GraphPropertiesDialogWidget::GetButtonBox() const
{
	return _buttons;
}
