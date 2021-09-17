/**
 * @brief Construct a new improc::SequenceService<key type> object
 * 
 * @tparam key_type 
 */
template <typename key_type>
improc::SequenceService<key_type>::SequenceService() {}

/**
 * @brief Load services sequence from json structure and corresponding service factory
 * 
 * @tparam key_type 
 * @param factory 
 * @param sequence_service_json 
 */
template <typename key_type>
void improc::SequenceService<key_type>::Load( const improc::ServicesFactory<key_type>& factory
                                            , const Json::Value& sequence_service_json )
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Loading sequence of services..." );
    const std::string kServicesKey = "services";
    if (sequence_service_json.isMember(kServicesKey) == false) 
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::err
                          , "ERROR_01: Member {} missing on json.",kServicesKey );
        throw improc::file_processing_error();
    }
    Json::Value service_elements = std::move(sequence_service_json[kServicesKey]);

    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::debug
                      , "{} services in factory.",factory.Size() );
    for (Json::Value::const_iterator srvce_elem_iter = service_elements.begin(); srvce_elem_iter != service_elements.end(); ++srvce_elem_iter)
    {
        const std::string kServiceType = "type";
        const std::string kServiceArgs = "args";

        Json::Value service_args {};
        key_type    service_type {};
        for (Json::Value::const_iterator srvce_elem_field_iter = srvce_elem_iter->begin(); srvce_elem_field_iter != srvce_elem_iter->end(); ++srvce_elem_field_iter)
        {
            SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                              , spdlog::level::debug
                              , "Analyzing field {} for service element..."
                              , srvce_elem_field_iter.name() );
            if      (srvce_elem_field_iter.name() == kServiceType)   service_type = improc::jsonfile::ReadElement<key_type>(*srvce_elem_field_iter);
            else if (srvce_elem_field_iter.name() == kServiceArgs)   service_args = *srvce_elem_field_iter;
            else 
            {
                SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                                  , spdlog::level::warn
                                  , "WARN_02: Member {} not recognized for service element."
                                  , srvce_elem_field_iter.name() );
            }
        }
        
        if (service_type.empty() == true)
        {
            SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                              , spdlog::level::err
                              , "ERROR_03: Service type missing for service element." );
            throw improc::file_processing_error();
        }

        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::info
                          , "Adding service element {}...",service_type );
        improc::SequenceService<key_type>::Service service {};
        service.type = service_type;
        service.data = factory.Get(std::move(service_type))(std::move(service_args));
        this->data_.push_back(std::move(service));
    }
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::info
                      , "{} services added to sequence.",this->data_.size() );
}

/**
 * @brief Run service sequence
 * The inputs and outputs for the service sequence should be provided in the 
 * context.
 * 
 * @tparam key_type 
 * @param context 
 */
template <typename key_type>
void improc::SequenceService<key_type>::Run(improc::Context<key_type>& context) const
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Running service sequence..." );
    for (auto service_iter = this->data_.begin(); service_iter != this->data_.end(); service_iter++)
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::debug
                          , "Running service {}...",(*service_iter).type );
        std::chrono::time_point<std::chrono::high_resolution_clock> service_start_time = std::chrono::high_resolution_clock::now();

        (*service_iter).data->Run(context);
        
        std::chrono::time_point<std::chrono::high_resolution_clock> service_stop_time  = std::chrono::high_resolution_clock::now();
        auto exec_time = std::chrono::duration_cast<std::chrono::microseconds>(service_stop_time - service_start_time);
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::info
                          , "Service {} executed in {} ms"
                          , (*service_iter).type,exec_time.count() / 1000.0 );
    }
}

/**
 * @brief Number of services in the service sequence.
 * 
 * @tparam key_type 
 * @return size_t 
 */
template <typename key_type>
size_t improc::SequenceService<key_type>::Size() const
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining number of services in sequence..." );
    return this->data_.size();
}
