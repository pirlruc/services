/**
 * @brief Construct a new improc::BaseService<KeyType,ContextType> object
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 */
template <typename KeyType,typename ContextType>
improc::BaseService<KeyType,ContextType>::BaseService() {}

/**
 * @brief Construct a new improc::BaseService<KeyType,ContextType> object
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 * @param service_json 
 */
template <typename KeyType,typename ContextType>
improc::BaseService<KeyType,ContextType>::BaseService(const Json::Value& service_json) 
{
    this->Load(service_json);
}

/**
 * @brief Load base service according with json structure for service
 * This part of the service is responsible for reading the input and output
 * names of the service.
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 * @param service_json 
 */
template <typename KeyType,typename ContextType>
improc::BaseService<KeyType,ContextType>& improc::BaseService<KeyType,ContextType>::Load(const Json::Value& service_json)
{
    IMPROC_SERVICES_LOGGER_TRACE("Loading service inputs and outputs...");
    static const std::string kInputKey  = "inputs";
    static const std::string kOutputKey = "outputs";
    if (service_json.isMember(kInputKey) == true)
    {
        IMPROC_SERVICES_LOGGER_DEBUG("Analyzing input field for service...");
        this->inputs_ = improc::json::ReadVector<KeyType>(service_json[kInputKey]);
    }
    else
    {
        std::string error_message = fmt::format("Key {} is missing from json",kInputKey);
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::json_error(std::move(error_message));
    }

    if (service_json.isMember(kOutputKey) == true)
    {
        IMPROC_SERVICES_LOGGER_DEBUG("Analyzing output field for service...");
        this->outputs_ = improc::json::ReadVector<KeyType>(service_json[kOutputKey]);
    }
    else
    {
        std::string error_message = fmt::format("Key {} is missing from json",kOutputKey);
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::json_error(std::move(error_message));
    }
    return (*this);
}    
