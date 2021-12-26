/**
 * @brief Construct a new improc::BaseService<KeyType,ContextType> object
 * 
 * @tparam KeyType 
 * @tparam ContextType 
 */
template <typename KeyType,typename ContextType>
improc::BaseService<KeyType,ContextType>::BaseService() {}

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

    const std::string kInputKey  = "inputs";
    const std::string kOutputKey = "outputs";

    if (service_json.isMember(kInputKey) == true)
    {
        IMPROC_SERVICES_LOGGER_DEBUG("Analyzing input field for service...");
        this->inputs_ = improc::json::ReadVector<KeyType>(service_json[kInputKey]);
    }
    else
    {
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Input member missing.");
        throw improc::file_processing_error();
    }

    if (service_json.isMember(kOutputKey) == true)
    {
        IMPROC_SERVICES_LOGGER_DEBUG("Analyzing output field for service...");
        this->outputs_ = improc::json::ReadVector<KeyType>(service_json[kOutputKey]);
    }
    else
    {
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_02: Output member missing.");
        throw improc::file_processing_error();
    }
    return (*this);
}    
