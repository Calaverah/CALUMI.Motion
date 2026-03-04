#pragma once

#include <functional>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <AgxNodes/AgxNode.h>


/// Class uses map for storing models (name, model)
class AgxNodeDelegateModelRegistry
{
public:
    using RegistryItemPtr = std::unique_ptr<AgxNode>;
    using RegistryItemCreator = std::function<RegistryItemPtr(AgxGraphModel* rootGraphReference)>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = std::set<QString>;

    //using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

    AgxNodeDelegateModelRegistry() = default;
    ~AgxNodeDelegateModelRegistry() = default;

    AgxNodeDelegateModelRegistry(AgxNodeDelegateModelRegistry const&) = delete;
    AgxNodeDelegateModelRegistry(AgxNodeDelegateModelRegistry&&) = default;

    AgxNodeDelegateModelRegistry& operator=(AgxNodeDelegateModelRegistry const&) = delete;

    AgxNodeDelegateModelRegistry& operator=(AgxNodeDelegateModelRegistry&&) = default;

public:
    template<typename ModelType>
    void registerModel(RegistryItemCreator creator, QString const& category = "Nodes")
    {
        QString const name = computeName<ModelType>(HasStaticMethodName<ModelType>{}, creator);
        if (!_registeredItemCreators.count(name)) {
            _registeredItemCreators[name] = std::move(creator);
            _categories.insert(category);
            _registeredModelsCategory[name] = category;
        }
    }

    template<typename ModelType>
    void registerModel(QString const& category = "Nodes")
    {
        RegistryItemCreator creator = [](AgxGraphModel* ref) { return std::make_unique<ModelType>(ref); };
        registerModel<ModelType>(std::move(creator), category);
    }


    template <typename ModelCreator>
    void
        registerModel(ModelCreator&& creator, QString const& category = "Nodes")
    {
        using ModelType = compute_model_type_t<decltype(creator())>;
        registerModel<ModelType>(std::forward<ModelCreator>(creator), category);
    }

    std::unique_ptr<AgxNode> create(QString const& modelName, AgxGraphModel* rootGraphReference);

    RegisteredModelCreatorsMap const& registeredModelCreators() const;

    RegisteredModelsCategoryMap const& registeredModelsCategoryAssociation() const;

    CategoriesSet const& categories() const;


private:
    RegisteredModelsCategoryMap _registeredModelsCategory;

    CategoriesSet _categories;

    RegisteredModelCreatorsMap _registeredItemCreators;

private:
    // If the registered ModelType class has the static member method
    // `static QString Name();`, use it. Otherwise use the non-static
    // method: `virtual QString name() const;`
    template<typename T, typename = void>
    struct HasStaticMethodName : std::false_type
    {
    };

    template<typename T>
    struct HasStaticMethodName<
        T,
        typename std::enable_if<std::is_same<decltype(T::Name()), QString>::value>::type>
        : std::true_type
    {
    };

    template<typename ModelType>
    static QString computeName(std::true_type, RegistryItemCreator const&)
    {
        return ModelType::Name();
    }

    template<typename ModelType>
    static QString computeName(std::false_type, RegistryItemCreator const& creator)
    {
        return creator(nullptr)->name();
    }

    template<typename T>
    struct UnwrapUniquePtr
    {
        // Assert always fires, but the compiler doesn't know this:
        static_assert(!std::is_same<T, T>::value,
                        "The ModelCreator must return a std::unique_ptr<T>, where T "
                        "inherits from NodeDelegateModel");
    };

    template<typename T>
    struct UnwrapUniquePtr<std::unique_ptr<T>>
    {
        static_assert(std::is_base_of<AgxNode, T>::value,
                        "The ModelCreator must return a std::unique_ptr<T>, where T "
                        "inherits from NodeDelegateModel");
        using type = T;
    };

    template<typename CreatorResult>
    using compute_model_type_t = typename UnwrapUniquePtr<CreatorResult>::type;
};
