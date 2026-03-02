#include "stdafx.h"
#include "AgxCommentNode.h"
#include "Widgets/AgxCommentWidget.h"

void AgxCommentNode::ToggleCollapse()
{
    if (collapsed)
        _emb->show();
    else
        _emb->hide();

    //if (collapsed)
    //    _dummyWidget->show();
    //else
    //    _dummyWidget->hide();

    AgxNode::ToggleCollapse();
}

void AgxCommentNode::insertPropertySheetData(const QJsonObject& data)
{
    if (data.contains("comment"))
    {
        QJsonObject comment = data["comment"].toObject();

        if (comment.contains("text"))
            _text = comment["text"].toString();

        if (comment.contains("font"))
            _font.fromString(comment["font"].toString());
    }

    if (data.contains("comment-target"))
    {
        _hasTarget = true;
        auto targetPos = data["comment-target"].toObject();

        if(targetPos.contains("x"))
            _target.setX(targetPos["x"].toDouble());
    
        if(targetPos.contains("y"))
            _target.setY(targetPos["y"].toDouble());
    }
    


    AgxNode::insertPropertySheetData(data);
}

QJsonObject AgxCommentNode::getPropertySheetData(bool cleared) const
{
    QJsonObject output = AgxNode::getPropertySheetData(cleared);
    QJsonObject comment;
    comment["text"] = cleared ? "" : _text;
    comment["font"] = cleared ? QFont().toString() : _font.toString();

    output["comment"] = comment;

    if (_hasTarget) {
        QJsonObject pt;

        pt["x"] = _target.x();
        pt["y"] = _target.y();

        output["comment-target"] = pt;
    }

    return output;
}


QString AgxCommentNode::name() const
{
    return QStringLiteral("COMMENT");
}

QString AgxCommentNode::caption() const
{
    return _nameProperty;
}

unsigned int AgxCommentNode::nPorts(AgxPortType portType) const
{
    return 0;
}

AgxNodeDataType AgxCommentNode::dataType(AgxPortType portType, AgxPortIndex portIndex) const
{
    return AgxNodeDataType{ "AgxPort", "Agx Port" };
}

void AgxCommentNode::setInData(std::shared_ptr<AgxNodeData> nodeData, AgxPortIndex const portIndex)
{
    //
}

std::shared_ptr<AgxNodeData> AgxCommentNode::outData(AgxPortIndex const port)
{
    return std::shared_ptr<AgxNodeData>();
}

QWidget* AgxCommentNode::embeddedWidget()
{
    //return nullptr;

    /*if (!_dummyWidget)
    {
        _dummyWidget = new QWidget();
        _dummyWidget->setMinimumSize(100, 50);
        _dummyWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        _dummyWidget->setStyleSheet("QWidget{ background: transparent }");
        _dummyWidget->resize(150, 100);
        
    }

    return _dummyWidget;*/

    if (!_emb)
    {
        _emb = new AgxCommentWidget(nullptr, {"comment"});
        _emb->setContentsMargins(3, 3, 3, 3);

        connect(this, &AgxNode::PropertySheetUpdated, _emb, [this]() {
                _emb->blockSignals(true);
                _emb->setText(this->_text);
                _emb->setFont(this->_font);
                _emb->blockSignals(false);
            });
        
        
    }

    if (!collapsed)
        return _emb;

    return nullptr;
}

AgxNodeType AgxCommentNode::GetNodeType() const
{
    return AgxNodeType::Comment;
}

QJsonObject AgxCommentNode::save() const
{
    QJsonObject output = AgxNode::save();
    
    output["property-sheet"] = getPropertySheetData(false);

    if (_hasTarget) {
        QJsonObject pt;

        pt["x"] = _target.x();
        pt["y"] = _target.y();

        output["comment-target"] = pt;
    }
    return output;
}

void AgxCommentNode::load(QJsonObject const& data)
{
    AgxNode::load(data);

    if (data.contains("comment-target"))
    {
        _hasTarget = true;
        auto targetPos = data["comment-target"].toObject();
        _target.setX(targetPos["x"].toDouble());
        _target.setY(targetPos["y"].toDouble());
    }
    
    QJsonObject pSheet = data["property-sheet"].toObject();
    insertPropertySheetData(pSheet);
}

AgxCommentNode::AgxCommentNode(AgxGraphModel* rootGraphRef) : AgxNode(rootGraphRef), _emb(nullptr) {
    _font = QFont();
    _nameProperty = "Comment";
    AgxNodeStyle style = nodeStyle();
    style.GradientColor0 = { 60,60,65 };
    style.GradientColor1 = { 60,60,65 };
    style.GradientColor2 = { 60,60,65 };
    style.GradientColor3 = { 60,60,65 };
    setNodeStyle(style);

}
