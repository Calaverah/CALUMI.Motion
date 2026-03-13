#include "stdafx.h"
#include "AgxFormat.h"
#include "Models/Blend/AgxBlendInputModel.h"

void FormatBlendInput(pugi::xml_node& parent, AgxBlendInputModel* blendData)
{
    if (!blendData) return;

    auto bInput = AgxAppend(parent, "blendInput", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    for (int i = 0; i < blendData->getDataRowCount(); i++)
    {
        auto point = AgxAppend(bInput, "point", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
        AgxAppendValue(point, "Range", blendData->getRangeValue(i), AgxFormat::None);
        AgxAppendValue(point, "Weight", blendData->getWeightValue(i), AgxFormat::None);
    }
    AgxCloseNode(bInput, false, true, 1);
}
