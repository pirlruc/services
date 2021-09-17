/**
 * @brief Construct a new improc::BaseService<key type> object
 * 
 * @tparam key_type 
 */
template <typename key_type>
improc::BaseService<key_type>::BaseService() {}

/**
 * @brief Load base service according with json structure for service
 * This part of the service is responsible for reading the input and output
 * names of the service.
 * 
 * @tparam key_type 
 * @param service_json 
 */
template <typename key_type>
void improc::BaseService<key_type>::Load(const Json::Value& service_json)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Loading service inputs and outputs..." );
    for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
    {
        const std::string kInputKey  = "inputs";
        const std::string kOutputKey = "outputs";

        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::info
                          , "Analyzing field {} for service...",service_field_iter.name() );
        if (service_field_iter.name() == kInputKey || service_field_iter.name() == kOutputKey)
        {
            std::vector<key_type> list_args;
            if (service_field_iter->isArray() == true)
            {
                for (Json::Value::const_iterator input_iter = service_field_iter->begin(); input_iter != service_field_iter->end(); ++input_iter)
                {
                    list_args.push_back(improc::jsonfile::ReadElement<key_type>(*input_iter));
                }
            }
            else
            {
                list_args.push_back(improc::jsonfile::ReadElement<key_type>(*service_field_iter));
            }

            if (service_field_iter.name() == kInputKey)
            {
                this->inputs_  = std::move(list_args);
            }
            else // (service_field_iter.name() == kOutputKey)
            {
                this->outputs_ = std::move(list_args);
            }
        }

        if (this->inputs_.empty() != true && this->outputs_.empty() != true)  break;
    }

    if (this->inputs_.empty() == true || this->outputs_.empty() == true)
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::err
                          , "ERROR_01: Input or output members missing." );
        throw improc::file_processing_error();
    }
}    
