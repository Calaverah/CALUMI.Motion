#include "stdafx.h"
#include "AgxNodeDelegateModelRegistry.h"
#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>

std::unique_ptr<AgxNode> AgxNodeDelegateModelRegistry::create(QString const& modelName, AgxGraphModel* rootGraphReference)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end())
    {
        return it->second(rootGraphReference);
    }

    return nullptr;
}

AgxNodeDelegateModelRegistry::RegisteredModelCreatorsMap const& AgxNodeDelegateModelRegistry::registeredModelCreators() const
{
    return _registeredItemCreators;
}

AgxNodeDelegateModelRegistry::RegisteredModelsCategoryMap const& AgxNodeDelegateModelRegistry::registeredModelsCategoryAssociation() const
{
    return _registeredModelsCategory;
}

AgxNodeDelegateModelRegistry::CategoriesSet const& AgxNodeDelegateModelRegistry::categories() const
{
    return _categories;
}