//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#pragma warning(pop)

#include "Widgets/IAgxEmbedSceneData.h"
#include "Utilities/AgxDefinitions.h"

class AgxGraphicsScene;
class AgxGraphModel;
class AgxPropertyBlockWidget;
class AgxPropertyBlockData;
class AgxLineEdit;
class AgxNode;
class ModifiedPushButton;
class AgxPort;
class AgxLineEditContainer;



class AgxNodePropertiesWidget : public QWidget, public IAgxEmbedSceneData
{
Q_OBJECT

public:
	explicit AgxNodePropertiesWidget(QWidget* parent = nullptr, bool bblockSignals = false);
	~AgxNodePropertiesWidget() override;

	QList<AgxLineEditContainer*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, const AgxNode* signalSender);
	QList<AgxLineEditContainer*> CreatePropertyEntries(const QVector<AgxPropertyEntryDefinition>* dataRef, const AgxGraphModel* signalSender);
	QList<AgxLineEditContainer*> CreatePropertyEntries(const QVector<AgxPropertyEntryDefinition>* dataRef, const AgxPort* signalSender, QStringList path = {});
	QList<QLabel*> CreateReadOnlyEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, const AgxNode* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QList<QLabel*> CreateReadOnlyEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, const AgxGraphModel* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QLabel* CreateGuidLabel(const QUuid* value, const AgxNode* signalSender, bool split = true);
	ModifiedPushButton* CreateFlagEntry(TermRef title, const AgxNode* signalSender, AgxFlagField* dataRef);
	AgxLineEditContainer* CreateSimpleLineEdit(const QString* sourceData, const AgxPort* signalSender, TermRef label = nullptr, bool split = true, const QStringList& path = {});

	AgxPropertyBlockWidget* CreatePropetryBlock(TermRef blockTitleRef, AgxPropertyBlockData& dataRef, uint8_t wrappedRowItemCount = 3);

	void CreateEmbeddedNodeGraphButton(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model);

	void FinalizeWidget();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void SetUpCustomDropDown(AgxLineEditContainer* line, const QList<TermRef>& list, const QStringList& keyPath);
	void ForceRefresh();
	void changeEvent(QEvent* event) override;


signals:
	void ClearFocusSignal();
	void LanguageChanged();
	void BroadcastWidth(int width);

private slots:
	void SendWidthAdjustment();

private:
	QVBoxLayout* _MainVBoxLayout = nullptr;
	QGridLayout* _mainFormLayout = nullptr;
	QFormLayout* _readonlyLeftLayout = nullptr;
	QFormLayout* _readonlyRightLayout = nullptr;
	QHBoxLayout* _readonlyParentLayout = nullptr;

	QHash<TermRef, AgxPropertyBlockWidget*> _PropertyBlocks;

	bool _nextEntryLeft = true;
};

