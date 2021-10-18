#ifndef IMPROC_SERVICES_FACTORY_HPP
#define IMPROC_SERVICES_FACTORY_HPP

#include <improc/improc_defs.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/container.hpp>

#include <functional>

namespace improc
{
    /**
     * @brief Factory to create services from json structures
     * This factory is based on the BaseService object.
     * 
     * @tparam KeyType 
     * @tparam ContextType 
     */
    template <typename KeyType,typename ContextType>
    class IMPROC_EXPORTS ServicesFactory : public Container<KeyType,std::function<std::shared_ptr<BaseService<KeyType,ContextType>>(const Json::Value&)>>
    {
        public:
            ServicesFactory();
    };

    typedef ServicesFactory<std::string,std::any>    StringKeyHeterogeneousServicesFactory;

    template<typename KeyType,typename ContextType,typename ServiceType>
    IMPROC_EXPORTS std::shared_ptr<BaseService<KeyType,ContextType>>    LoadServiceFromJson(const Json::Value& service_json);

    template<typename ServiceType>
    IMPROC_EXPORTS std::shared_ptr<StringKeyHeterogeneousBaseService>   LoadServiceFromJson(const Json::Value& service_json);
}

#include <factory.tpp>

#endif