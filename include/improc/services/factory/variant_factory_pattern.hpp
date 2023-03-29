#ifndef IMPROC_SERVICES_VARIANT_FACTORY_PATTERN_HPP
#define IMPROC_SERVICES_VARIANT_FACTORY_PATTERN_HPP

#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/factory/factory_pattern.hpp>
#include <improc/improc_function_traits.hpp>

#include <variant>

namespace improc
{
    /**
     * @brief The variant factory pattern can be used to create different objects that share
     * the same BaseProduct using different functors.
     * 
     * @tparam BaseProduct - output type of the factory pattern 
     * @tparam KeyType - data type of the access key to the factory
     * @tparam VariantProductCreator - different functors that allow to create the object of the factory
     * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
     */
    template    <   class    BaseProduct
                ,   typename KeyType
                ,   typename VariantProductCreator = std::variant<std::function<std::shared_ptr<BaseProduct>>()>
                ,   template <typename,class> class FactoryErrorPolicy = improc::FactoryPatternError 
                >
    class IMPROC_API VariantFactoryPattern : private improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>
    {
        static_assert(std::variant_size_v<VariantProductCreator> > 1, "If only one functor is going to be used, please use FactoryPattern.");
        static_assert(improc::is_variant_arg_function_type_v<VariantProductCreator>, "Variant should have functor arguments.");
        static_assert(improc::is_variant_arg_function_return_type_v<std::shared_ptr<BaseProduct>,VariantProductCreator>, "Return type should be a BaseProduct pointer.");

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