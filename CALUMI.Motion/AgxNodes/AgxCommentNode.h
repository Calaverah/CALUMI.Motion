#pragma once
#pragma warning(push,0)
#include <QObject>
#include <QTextEdit>
#pragma warning(pop)
#include "AgxNode.h"


class AgxCommentWidget;

class AgxCommentNode : public AgxNode
{
public:
	QString name() const override;
	QString caption() const override;
	unsigned int nPorts(AgxPortType portType) const override;
	AgxNodeDataType dataType(AgxPortType portType, AgxPortIndex portIndex) const override;
	void setInData(std::shared_ptr<AgxNodeData> nodeData, AgxPortIndex const portIndex) override;
	std::shared_ptr<AgxNodeData> outData(AgxPortIndex const port) override;
	QWidget* embeddedWidget() override;
	AgxNodeType GetNodeType() const override;
	bool resizable() const override { return true; }

	QJsonObject save() const override;
	void load(QJsonObject const&) override;

	AgxCommentNode(AgxGraphModel* rootGraphRef); //auto nodeStyle = AgxNode::nodeStyle(); nodeStyle.FontColor = QColor(0, 255, 0); this->setNodeStyle(nodeStyle);} //
	~AgxCommentNode() {}

	inline bool CanSetNameProperty() const override { return false; }
	void ToggleCollapse() override;
	inline AgxPortType CanModifyPorts() const override { return AgxPortType::None; }

public:
	void insertPropertySheetData(const QJsonObject& data) override;
	QJsonObject getPropertySheetData(bool cleared = false) const override;

private:
	AgxCommentWidget* _emb;
	QWidget* _dummyWidget = nullptr;

private:
	QFont _font;
	QString _text;

	QPointF _target;
	bool _hasTarget = false;

public:
	friend class AgxGraphModel;
};