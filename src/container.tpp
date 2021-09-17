/**
 * @brief Construct a new improc::Container<key type,container type> object
 * 
 * @tparam key_type 
 * @tparam container_type 
 */
template <typename key_type,typename container_type>
improc::Container<key_type,container_type>::Container() 
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Creating container with {} key and {} container..."
                      , typeid(key_type).name()
                      , typeid(container_type).name() );
}

/**
 * @brief Add an item of container_type with key of key_type in container
 * This is a method that allows a safe insert without allowing an item to be 
 * overwritten.
 * 
 * @tparam key_type 
 * @tparam container_type 
 * @param key 
 * @param item 
 */
template <typename key_type,typename container_type>
void improc::Container<key_type,container_type>::Add(const key_type& key, const container_type& item)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Adding key {} to container...", key );
    if (this->hash_table_.find(key) == this->hash_table_.end())
    {
        this->hash_table_[std::move(key)] = std::move(item);
    }
    else
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::err
                          , "ERROR_01: Duplicated key {} in container.", key );
        throw improc::duplicated_key();
    }
}

/**
 * @brief Get an item with key of key_type from container
 * This is a method that ensures that the item exists on the container. If the item
 * does not exist, the method will throw an exception. The retrieved item
 * cannot be edited.
 *  
 * @tparam key_type 
 * @tparam container_type 
 * @param key 
 * @return container_type 
 */
template <typename key_type,typename container_type>
container_type improc::Container<key_type,container_type>::Get(const key_type& key) const
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining key {} from container...", key );
    if (this->hash_table_.find(key) != this->hash_table_.end())
    {
        return this->hash_table_.at(std::move(key));
    }
    else
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::err
                          , "ERROR_02: Key {} not found in container.", key );
        throw improc::not_found_key();
    }
}

/**
 * @brief Get an item with key of key_type from container  
 * This method will retrieve an item if it exists otherwise it will create a new 
 * entry on the container which can be edited. 
 * 
 * @tparam key_type 
 * @tparam container_type 
 * @param key 
 * @return container_type& 
 */
template <typename key_type,typename container_type>
container_type& improc::Container<key_type,container_type>::operator[](const key_type& key)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining key {} from container...", key );
    return this->hash_table_[std::move(key)];
}

/**
 * @brief Erase item with key of key_type from container
 * The method does not throw an error if key does not exist.
 * 
 * @tparam key_type 
 * @tparam container_type 
 * @param key 
 */
template <typename key_type,typename container_type>
void improc::Container<key_type,container_type>::Erase(const key_type& key)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Deleting key {} from container...", key );
    if (this->hash_table_.find(key) != this->hash_table_.end()) 
    {
        this->hash_table_.erase(std::move(key));
    }
    else
    {
        SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                          , spdlog::level::warn
                          , "WARN_01: Key {} not found in container.", key );
    }
}

/**
 * @brief Clear items from container
 * 
 * @tparam key_type 
 * @tparam container_type 
 */
template <typename key_type,typename container_type>
void improc::Container<key_type,container_type>::Clear()
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Deleting container..." );
    this->hash_table_.clear();
}

/**
 * @brief Obtain number of items in container
 * 
 * @tparam key_type 
 * @tparam container_type 
 * @return size_t 
 */
template <typename key_type,typename container_type>
size_t improc::Container<key_type,container_type>::Size() const
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining container size..." );
    return this->hash_table_.size();
}
