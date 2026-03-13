#pragma once

#pragma warning(push,0)
#include <QWidget>
#include <QLabel>
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
	AgxNodePropertiesWidget(QWidget* parent = nullptr, bool bblockSignals = false);
	~AgxNodePropertiesWidget();	

public:
	QList<AgxLineEditContainer*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxNode* signalSender, bool split = true);
	QList<AgxLineEditContainer*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxGraphModel* signalSender, bool split = true);
	QList<AgxLineEditContainer*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxPort* signalSender, bool split = true, QStringList path = {});
	QList<QLabel*> CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxNode* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QList<QLabel*> CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxGraphModel* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QLabel* CreateGuidLabel(const QUuid* value, AgxNode* signalSender, bool split = true);
	ModifiedPushButton* CreateFlagEntry(const QString& title, AgxNode* signalSender, AgxFlagField* dataRef);
	AgxLineEditContainer* CreateSimpleLineEdit(QString* defaultText, AgxPort* signalSender, TermRef label = nullptr, bool split = true, QStringList path = {});

	AgxPropertyBlockWidget* CreatePropetryBlock(TermRef blockName, AgxPropertyBlockData& dataRef);

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

public slots:
	void SetHiddenEntries(bool hidden);

private slots:
	void SendWidthAdjustment();

private:
	QVBoxLayout* _MainVBoxLayout = nullptr;
	QFormLayout* _LeftFormLayout = nullptr;
	QFormLayout* _hiddenLeftFormLayout = nullptr;
	QFormLayout* _RightFormLayout = nullptr;
	QFormLayout* _hiddenRightFormLayout = nullptr;
	QHBoxLayout* _hiddenLayout = nullptr;

	QHash<TermRef, AgxPropertyBlockWidget*> _PropertyBlocks;

	bool _nextEntryLeft = true;
};

