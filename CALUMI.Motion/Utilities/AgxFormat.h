//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QString>
#include "3rdPartyDirect/pugiXML/pugixml.hpp"
#include "AgxDefinitions.h"
#pragma warning(pop)

class AgxBlendInputModel;

constexpr double SFBGSxScalar = 2.0;
constexpr double SFBGSyScalar = 1.0;

inline QString cleanFileName(QString name, bool toLowerCase = false, bool replaceSpaces = true) {
    
    QRegularExpression illegalChars("[\\\\/:*?\"<>|]");
    
    if(replaceSpaces)
        name.replace(" ", "_");

    name.remove(illegalChars);
    
    if (toLowerCase)
        name = name.toLower();

    return name.trimmed();
}

inline QString ConvertEquationToText(QString text)
{
    text.replace(">", "!greaterthan!");
    text.replace("<", "!lessthan!");
    text.replace("&", "!ampersand!");
    return text;
}

inline QString ConvertTextToEquation(QString text)
{
    text.replace("!greaterthan!", ">", Qt::CaseInsensitive);
    text.replace("!lessthan!","<", Qt::CaseInsensitive);
    text.replace("!ampersand!", "&", Qt::CaseInsensitive);
    return text;
}


enum class AgxFormat {
    None = 0x0,
    NewLine = 0x1,
    Indent = 0x2,
    IndentToParent = 0x04,
    SpaceBefore = 0x08,
    SpaceAfter = 0x10,

    Default = NewLine | Indent | IndentToParent
};

using AgxFormatSet = QFlags<AgxFormat>;

inline QString CleanUpDecimals(const QString& number)
{
    QString output = number.isEmpty() ? "0" : number;

    if (!output.contains(".")) return output;

    output.remove(QRegularExpression("0+$"));
    output.remove(QRegularExpression("\\.$"));

    return output;
}

inline size_t GetParentCount(pugi::xml_node& node) 
{
    size_t count = 0;
    auto parent = node.parent();

    while (parent != nullptr && parent.type() != pugi::node_document) {
        count++;
        parent = parent.parent();
    }

    return count;
}

inline void AgxCloseNode(pugi::xml_node& node, bool toParent = true, bool indent = true, unsigned int indentOverride = 1)
{
    node.append_child(pugi::node_pcdata).set_value("\n");
    
    if(indent)
    {
        QString pId(indentOverride, '\t');
        node.append_child(pugi::node_pcdata).set_value(pId.toStdString());
    }
    
    if (!toParent) return;

    QString indentation;
    for (size_t i = 0; i < GetParentCount(node); i++)
    {
        indentation += "\t";
    }

    node.append_child(pugi::node_pcdata).set_value(indentation.toStdString());
}

inline pugi::xml_node AgxAppend(pugi::xml_node& node, const QString& tag, AgxFormatSet format = AgxFormat::Default, unsigned int indentOverride = 1)
{
    if (format & AgxFormat::NewLine)
        node.append_child(pugi::node_pcdata).set_value("\n");

    if (format & AgxFormat::IndentToParent)
    {
        QString pId(GetParentCount(node),'\t');
        node.append_child(pugi::node_pcdata).set_value(pId.toStdString());
    }

    if(format & AgxFormat::Indent)
    {
        QString pId(indentOverride, '\t');
        node.append_child(pugi::node_pcdata).set_value(pId.toStdString());
    }

    if (format & AgxFormat::SpaceBefore)
        node.append_child(pugi::node_pcdata).set_value(" ");

    auto output = node.append_child(tag.toStdString());

    if (format & AgxFormat::SpaceAfter)
        node.append_child(pugi::node_pcdata).set_value(" ");
    
    return output;
}

inline pugi::xml_node AgxAppendValue(pugi::xml_node& node, const QString& tag, const QString& value, AgxFormatSet format = AgxFormat::Default, unsigned int indentOverride = 1)
{
    auto output = AgxAppend(node, tag, format, indentOverride);

    output.append_child(pugi::node_pcdata).set_value(value.toStdString());

    return output;
}

inline void FormatBasicPropertySheet(pugi::xml_node& parent, const QVector<AgxPropertyEntryDefinition>& _PropertyEntries)
{
    //matching with single indent
    auto propSheet = AgxAppend(parent, "property_sheet", { AgxFormat::NewLine, AgxFormat::Indent });
    AgxAppendValue(propSheet, "num_columns", "2", { AgxFormat::NewLine, AgxFormat::Indent }, 2);

    auto column1 = AgxAppend(propSheet, "column", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
    auto column2 = AgxAppend(propSheet, "column", { AgxFormat::NewLine, AgxFormat::Indent }, 2);

    AgxAppendValue(column1, "header", "Property", AgxFormat::SpaceBefore);
    AgxAppendValue(column2, "header", "Value", AgxFormat::SpaceBefore);

    AgxAppendValue(column1, "types", "5", { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });
    AgxAppendValue(column2, "types", "5", { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });

    for (auto& property : _PropertyEntries)
    {
        if (property.isPresent)
        {
            auto row = AgxAppend(propSheet, "row", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
            auto prop1 = AgxAppend(row, "prop", { AgxFormat::NewLine, AgxFormat::Indent }, 3);
            auto prop2 = AgxAppend(row, "prop", { AgxFormat::NewLine, AgxFormat::Indent }, 3);

            AgxAppendValue(prop1, "type", "2", AgxFormat::SpaceBefore);
            AgxAppendValue(prop2, "type", QString("%1").arg(static_cast<int>(GetSFBGSVarTypeFromColumnType(property.columnType).second)), AgxFormat::SpaceBefore);

            AgxAppendValue(prop1, "value", property.Tag(), { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });
            AgxAppendValue(prop2, "value", property.value, { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });
            if (property.columnType == AgxColumnTypes::CustomDropDown)
            {
                int cIndex = 0;
                auto list = property.CustomDropDownList();
                for (qsizetype i = 0; i < list.size(); i++)
                {
                    if (list.at(i)().tag.compare(property.value, Qt::CaseInsensitive) == 0) cIndex = i;
                }

                AgxAppendValue(prop2, "listindex", QString("%1").arg(cIndex), AgxFormat::SpaceAfter, 0);
            }
            AgxCloseNode(row, false, true, 2);
        }
    }
    AgxCloseNode(propSheet, false, true);
}

static void agxStringFilter_1(QString& text) {
    QRegularExpression regex("\\_.*\\_");
    text.replace(regex, "");
}

static void agxStringFilter_2(QString& text) {
    QRegularExpression regex("\\~.*\\~");
    text.replace(regex, "");
}

inline void FormatPropertyBlock(pugi::xml_node& parent, const AgxPropertyBlockData& blockData)
{
    //matching with single indent
    auto propSheet = AgxAppend(parent, "property_sheet", { AgxFormat::NewLine, AgxFormat::Indent });
    AgxAppendValue(propSheet, "num_columns", QString("%1").arg(blockData.GetColumnCount()), {AgxFormat::NewLine, AgxFormat::Indent}, 2);

    for (unsigned int i = 0; i < blockData.GetColumnCount(); i++)
    {
        auto column = AgxAppend(propSheet, "column", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
        QString tag = blockData.GetColumnDefinition(i).Tag();
        agxStringFilter_1(tag);
        agxStringFilter_2(tag);
        AgxAppendValue(column, "header", tag, AgxFormat::SpaceBefore);
        AgxAppendValue(column, "types", QString("%1").arg(static_cast<int>(GetSFBGSVarTypeFromColumnType(blockData.GetColumnType(i)).second)), { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });
    }

    for (unsigned int rowIdx = 0; rowIdx < blockData.GetRowCount(); rowIdx++)
    {   
        auto rowData = blockData.GetRow(rowIdx);
        auto row = AgxAppend(propSheet, "row", { AgxFormat::NewLine, AgxFormat::Indent }, 2);

        for (unsigned int colIdx = 0; colIdx < blockData.GetColumnCount(); colIdx++)
        {
            auto prop = AgxAppend(row, "prop", { AgxFormat::NewLine, AgxFormat::Indent }, 3);
            AgxAppendValue(prop, "type", QString("%1").arg(static_cast<int>(GetSFBGSVarTypeFromColumnType(rowData->at(colIdx).Type).second)), AgxFormat::SpaceBefore);
            AgxAppendValue(prop, "value", rowData->at(colIdx).Value, { AgxFormat::SpaceBefore, AgxFormat::SpaceAfter });
            if (blockData.GetColumnType(colIdx) == AgxColumnTypes::CustomDropDown)
            {
                int cIndex = 0;
                auto list = blockData.GetColumnDefinition(colIdx).CustomDropDownList();
                for (qsizetype i = 0; i < list.size(); i++)
                {
                    if (list.at(i)().tag.compare(rowData->at(colIdx).Value, Qt::CaseInsensitive) == 0) cIndex = i;
                }

                AgxAppendValue(prop, "listindex", QString("%1").arg(cIndex), AgxFormat::SpaceAfter, 0);
            }
        }

        AgxCloseNode(row, false, true, 2);
    }
    AgxCloseNode(propSheet, false, true);
}

void FormatBlendInput(pugi::xml_node& parent, AgxBlendInputModel* blendData);