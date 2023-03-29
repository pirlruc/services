#ifndef IMPROC_SERVICES_FACTORY_PATTERN_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_HPP

#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/factory/factory_pattern_policies.hpp>
#include <improc/improc_function_traits.hpp>

#include <pipes/helpers/invoke.hpp>

namespace improc
{
    /**
     * @brief The factory pattern can be used to create different objects that share
     * the same BaseProduct.
     * 
     * @tparam BaseProduct - output type of the factory pattern 
     * @tparam KeyType - data type of the access key to the factory
     * @tparam ProductCreator - functor to create the object of the factory
     * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
     */
    template    <   class    BaseProduct
                ,   typename KeyType
                ,   typename ProductCreator = std::function<std::shared_ptr<BaseProduct>>()
                ,   template <typename,class> class FactoryErrorPolicy = improc::FactoryPatternError 
                >
    class IMPROC_API FactoryPattern : public FactoryErrorPolicy<KeyType,BaseProduct>
    {
        static_assert(improc::is_hashable_v<KeyType>, "KeyType should be an integral or a string data type.");
        
        protected:
            typedef std::unordered_map<KeyType,ProductCreator> CallbackMap;
            CallbackMap                     callbacks_;

        public:
            FactoryPattern();

            FactoryPattern&                 Register    (const KeyType& id, const ProductCreator& creator);
            bool                            Unregister  (const KeyType& id);

            std::vector<KeyType>            GetRegisteredIds()  const;
            size_t                          Size()              const;

            template <typename ... Args>
            std::shared_ptr<BaseProduct>    Create(const KeyType& id, Args&& ... args)  const;
    };
}

#include <improc/services/factory/factory_pattern.tpp>

#endif