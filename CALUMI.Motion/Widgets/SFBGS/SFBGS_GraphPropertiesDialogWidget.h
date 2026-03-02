#pragma once
#pragma warning(push,0)
#include <QDialog>
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Models/AgxGraphicsScene.h"
#include "Widgets/IAgxEmbedSceneData.h"
#include <QScrollarea>
#pragma warning(pop)
#include "Interfaces/IButtonBox.h"

class SFBGS_GraphPropertiesDialogWidget : public QWidget, public IButtonBox
{
	Q_OBJECT

public:
	SFBGS_GraphPropertiesDialogWidget(AgxGraphicsScene& scene, QWidget* parent = nullptr);
	~SFBGS_GraphPropertiesDialogWidget() { qDebug() << "Destroyed Graph Properties Widget"; }

	QDialogButtonBox* GetButtonBox() const override;

public slots:
	void SetWidth(int width);

private:
	QGridLayout* _mainLayout;
	QScrollArea* _scrollArea;
	AgxNodePropertiesWidget* _propertyWidgets;
	QDialogButtonBox* _buttons;

	QSet<QString> _categoryList = {
			"1stPerson",
			"AddonRigs",
			"Components",
			"Creatures",
			"Critters",
			"DynamicFurniture",
			"Effects",
			"EffectsPowers",
			"FurnitureMarkers",
			"GenericGamebryo",
			"GenericStateGraphs",
			"GenericVFXSequence",
			"GunAssembly",
			"NPC",
			"Outpost",
			"Player",
			"QuestFX",
			"Robots",
			"RootGraphs",
			"Ship",
			"ShipEffects",
			"Subgraphs",
			"System",
			"Turret",
			"VFXBackup",
			"VFXTestGym",
			"WorldArt",
			"_OLD",
			"_tempGraphs"
	};
};

