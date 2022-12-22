#ifndef IMPROC_SERVICES_VARIANT_FACTORY_PATTERN_HPP
#define IMPROC_SERVICES_VARIANT_FACTORY_PATTERN_HPP

#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/factory/factory_pattern.hpp>
#include <improc/improc_function_traits.hpp>

#include <variant>

namespace improc
{
    template    <   class    BaseProduct
                ,   typename KeyType
                ,   typename VariantProductCreator = std::variant<std::function<std::shared_ptr<BaseProduct>>()>
                ,   template <typename,class> class FactoryErrorPolicy = improc::FactoryPatternError 
                >
    class IMPROC_API VariantFactoryPattern : private improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>
    {
        public:
            VariantFactoryPattern();

            template<typename ProductCreator>
            VariantFactoryPattern&          Register    (const KeyType& id, const ProductCreator& creator);
            bool                            Unregister  (const KeyType& id);

            std::vector<KeyType>            GetRegisteredIds()  const;
            size_t                          Size()              const;

            template <typename ProductCreator, typename ... Args>
            std::shared_ptr<BaseProduct>    Create(const KeyType& id, Args&& ... args)  const;
    };
}

#include <improc/services/factory/variant_factory_pattern.tpp>

#endif