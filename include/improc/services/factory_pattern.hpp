#ifndef IMPROC_SERVICES_FACTORY_PATTERN_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_HPP

#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/factory_pattern_policies.hpp>

namespace improc
{
    template    <   class    BaseProduct
                ,   typename KeyType
                ,   typename ProductCreator = std::function<std::shared_ptr<BaseProduct>>()
                ,   template <typename,class> class FactoryErrorPolicy = improc::FactoryPatternError 
                >
    class FactoryPattern : public FactoryErrorPolicy<KeyType,BaseProduct>
    {
        private:
            typedef std::unordered_map<KeyType,ProductCreator> CallbackMap;
            CallbackMap callbacks_;

        public:
            FactoryPattern();

            bool                    Register    (const KeyType& id, ProductCreator creator);
            bool                    Unregister  (const KeyType& id);

            std::vector<KeyType>    GetRegisteredIds()  const;
            size_t                  Size()              const;

            template <typename ... Args>
            std::shared_ptr<BaseProduct>    Create(const KeyType& id, Args&& ... args);
    };
}

#include <factory_pattern.tpp>

#endif