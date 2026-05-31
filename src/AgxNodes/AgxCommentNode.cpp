//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxCommentNode.h"
#include "Widgets/AgxCommentWidget.h"

void AgxCommentNode::ToggleCollapse()
{
    if (m_collapsed)
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
            m_font.fromString(comment["font"].toString());
    }

    if (data.contains("comment-target"))
    {
        m_hasTarget = true;
        auto targetPos = data["comment-target"].toObject();

        if(targetPos.contains("x"))
            m_target.setX(targetPos["x"].toDouble());
    
        if(targetPos.contains("y"))
            m_target.setY(targetPos["y"].toDouble());
    }
    


    AgxNode::insertPropertySheetData(data);
}

QJsonObject AgxCommentNode::getPropertySheetData(const bool cleared) const
{
    QJsonObject output = AgxNode::getPropertySheetData(cleared);
    QJsonObject comment;
    comment["text"] = cleared ? "" : _text;
    comment["font"] = cleared ? QFont().toString() : m_font.toString();

    output["comment"] = comment;

    if (m_hasTarget) {
        QJsonObject pt;

        pt["x"] = m_target.x();
        pt["y"] = m_target.y();

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
    return m_nameProperty;
}

unsigned int AgxCommentNode::nPorts(AgxPortType portType) const
{
    return 0;
}

AgxNodeDataType AgxCommentNode::dataType(AgxPortType portType, AgxPortIndex portIndex) const
{
    return AgxNodeDataType{ "AgxPort", "Agx Port" };
}

void AgxCommentNode::setInData(const std::shared_ptr<AgxNodeData>& nodeData, AgxPortIndex const portIndex)
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

        connect(this, &AgxNode::PropertySheetUpdated, _emb, [this] {
                _emb->blockSignals(true);
                _emb->setCommentText(this->_text);
                _emb->setCommentFont(this->m_font);
                _emb->blockSignals(false);
            });
        
        
    }

    if (!m_collapsed)
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

    if (m_hasTarget) {
        QJsonObject pt;

        pt["x"] = m_target.x();
        pt["y"] = m_target.y();

        output["comment-target"] = pt;
    }
    return output;
}

void AgxCommentNode::load(QJsonObject const& data)
{
    AgxNode::load(data);

    if (data.contains("comment-target"))
    {
        m_hasTarget = true;
        auto targetPos = data["comment-target"].toObject();
        m_target.setX(targetPos["x"].toDouble());
        m_target.setY(targetPos["y"].toDouble());
    }

    const QJsonObject pSheet = data["property-sheet"].toObject();
    insertPropertySheetData(pSheet);
}

AgxCommentNode::AgxCommentNode(AgxGraphModel* rootGraphRef) : AgxNode(rootGraphRef), _emb(nullptr)
{
    m_font = QFont();
    m_nameProperty = "Comment";
}
