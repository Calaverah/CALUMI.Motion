#pragma once
#pragma warning(push,0)
#include <QDialog>
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Models/AgxGraphicsScene.h"
#include "Widgets/IAgxEmbedSceneData.h"
#include <QScrollArea>
#pragma warning(pop)

class SFBGS_GraphPropertiesDialogWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SFBGS_GraphPropertiesDialogWidget(AgxGraphicsScene& scene, QWidget* parent = nullptr);
	~SFBGS_GraphPropertiesDialogWidget() override
	{ qDebug() << "Destroyed Graph Properties Widget"; }

public slots:
	void SetWidth(int width);

private:
	QGridLayout* _mainLayout;
	QScrollArea* _scrollArea;
	AgxNodePropertiesWidget* _propertyWidgets;

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

