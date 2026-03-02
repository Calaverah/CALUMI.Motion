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



class AgxNodePropertiesWidget : public QWidget, public IAgxEmbedSceneData
{
Q_OBJECT

public:
	AgxNodePropertiesWidget(QWidget* parent = nullptr, bool bblockSignals = false);
	~AgxNodePropertiesWidget();	

public:
	QList<AgxLineEdit*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxNode* signalSender, bool split = true);
	QList<AgxLineEdit*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxGraphModel* signalSender, bool split = true);
	QList<AgxLineEdit*> CreatePropertyEntries(QVector<AgxPropertyEntryDefinition>* dataRef, AgxPort* signalSender, bool split = true, QStringList path = {});
	QList<QLabel*> CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxNode* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QList<QLabel*> CreateHiddenEntries(QMap<TermRef, QPair<AgxColumnTypes, QString>>* dataRef, AgxGraphModel* signalSender, bool split = true, const QList<TermRef>& priorityOrder = {});
	QLabel* CreateGuidLabel(const QUuid* value, AgxNode* signalSender, bool split = true);
	ModifiedPushButton* CreateFlagEntry(const QString& title, AgxNode* signalSender, AgxFlagField* dataRef);
	AgxLineEdit* CreateSimpleLineEdit(QString* defaultText, AgxPort* signalSender, const QString& label = "", bool split = true, QStringList path = {});

	AgxPropertyBlockWidget* CreatePropetryBlock(TermRef blockName, AgxPropertyBlockData& dataRef);

	void CreateEmbeddedNodeGraphButton(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model);

	void FinalizeWidget();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void SetUpCustomDropDown(AgxLineEdit* line, const QStringList& list, const QStringList& keyPath);
	void ForceRefresh();
	void changeEvent(QEvent* event) override;

signals:
	void ClearFocusSignal();
	void BroadcastWidth(int width);
	void LanguageChanged();

public slots:
	void SetHiddenEntries(bool hidden);
		

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

