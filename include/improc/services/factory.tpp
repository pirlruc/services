/**
 * @brief Construct a new improc::ServicesFactory<KeyType,ContextType> object
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 */
template <typename KeyType,typename ContextType>
improc::ServicesFactory<KeyType,ContextType>::ServicesFactory() 
: improc::FactoryPattern    < BaseService<KeyType,ContextType>
                            , KeyType
                            , std::function<std::shared_ptr<BaseService<KeyType,ContextType>>(const Json::Value&)>
                            >() {}

/**
 * @brief Load service from json structure
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 * @tparam ServiceType 
 * @param service_json 
 * @return std::shared_ptr<improc::BaseService<KeyType,ContextType>> 
 */
template<typename KeyType,typename ContextType,typename ServiceType>
std::shared_ptr<improc::BaseService<KeyType,ContextType>> improc::LoadServiceFromJson(const Json::Value& service_json)
{
    IMPROC_SERVICES_LOGGER_TRACE( "Creating shared pointer for key {} service {}..."
                                , typeid(KeyType).name(), typeid(ServiceType).name() );
    std::shared_ptr<improc::BaseService<KeyType,ContextType>> service 
            {std::make_shared<ServiceType>(ServiceType(std::move(service_json)))};
    return service;
}

/**
 * @brief Load service from json structure
 * This service assumes that the KeyType is an std::string
 * 
 * @tparam ServiceType 
 * @param service_json 
 * @return std::shared_ptr<improc::StringKeyHeterogeneousBaseService> 
 */
template<typename ServiceType>
std::shared_ptr<improc::StringKeyHeterogeneousBaseService> improc::LoadServiceFromJson(const Json::Value& service_json)
{
    IMPROC_SERVICES_LOGGER_TRACE( "Creating shared pointer for key string service {}..."
                                , typeid(ServiceType).name() );
    std::shared_ptr<improc::StringKeyHeterogeneousBaseService> service 
            {std::make_shared<ServiceType>(ServiceType(std::move(service_json)))};
    return service;
}
