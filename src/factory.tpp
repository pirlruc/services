/**
 * @brief Construct a new improc::ServicesFactory<key type> object
 * 
 * @tparam key_type 
 */
template <typename key_type>
improc::ServicesFactory<key_type>::ServicesFactory() : improc::Container<key_type,std::function<std::shared_ptr<improc::BaseService<key_type>>(const Json::Value&)>>() {}

/**
 * @brief Load service from json structure
 * 
 * @tparam key_type 
 * @tparam service_type 
 * @param service_json 
 * @return std::shared_ptr<improc::BaseService<key_type>> 
 */
template<typename key_type,typename service_type>
std::shared_ptr<improc::BaseService<key_type>> improc::LoadServiceFromJson(const Json::Value& service_json)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Creating shared pointer for key {} service {}..."
                      , typeid(key_type).name(), typeid(service_type).name() );
    std::shared_ptr<improc::BaseService<key_type>> service {std::make_shared<service_type>(service_type())};
    service->Load(std::move(service_json));
    return service;
}

/**
 * @brief Load service from json structure
 * This service assumes that the key_type is an std::string
 * 
 * @tparam service_type 
 * @param service_json 
 * @return std::shared_ptr<improc::StringKeyBaseService> 
 */
template<typename service_type>
std::shared_ptr<improc::StringKeyBaseService> improc::LoadServiceFromJson(const Json::Value& service_json)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Creating shared pointer for key string service {}..."
                      , typeid(service_type).name() );
    std::shared_ptr<improc::StringKeyBaseService> service {std::make_shared<service_type>(service_type())};
    service->Load(std::move(service_json));
    return service;
}
