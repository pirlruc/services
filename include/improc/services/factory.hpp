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
     * @tparam key_type 
     */
    template <typename key_type>
    class IMPROC_EXPORTS ServicesFactory : public Container<key_type,std::function<std::shared_ptr<BaseService<key_type>>(const Json::Value&)>>
    {
        public:
            ServicesFactory();
    };

    typedef ServicesFactory<std::string>    StringKeyServicesFactory;

    template<typename key_type,typename service_type>
    IMPROC_EXPORTS std::shared_ptr<BaseService<key_type>>   LoadServiceFromJson(const Json::Value& service_json);

    template<typename service_type>
    IMPROC_EXPORTS std::shared_ptr<StringKeyBaseService>    LoadServiceFromJson(const Json::Value& service_json);
}

#include <factory.tpp>

#endif