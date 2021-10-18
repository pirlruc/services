/**
 * @brief Construct a new improc::Container<KeyType,ContainerType> object
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 */
template <typename KeyType,typename ContainerType>
improc::Container<KeyType,ContainerType>::Container() 
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Creating container with {} key and {} container..."
                      , typeid(KeyType).name()
                      , typeid(ContainerType).name() );
}

/**
 * @brief Add an item of ContainerType with key of KeyType in container
 * This is a method that allows a safe insert without allowing an item to be 
 * overwritten.
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 * @param key 
 * @param item 
 */
template <typename KeyType,typename ContainerType>
void improc::Container<KeyType,ContainerType>::Add(const KeyType& key, const ContainerType& item)
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
 * @brief Get an item with key of KeyType from container
 * This is a method that ensures that the item exists on the container. If the item
 * does not exist, the method will throw an exception. The retrieved item
 * cannot be edited.
 *  
 * @tparam KeyType 
 * @tparam ContainerType 
 * @param key 
 * @return ContainerType 
 */
template <typename KeyType,typename ContainerType>
ContainerType improc::Container<KeyType,ContainerType>::Get(const KeyType& key) const
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
 * @brief Get an item with key of KeyType from container  
 * This method will retrieve an item if it exists otherwise it will create a new 
 * entry on the container which can be edited. 
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 * @param key 
 * @return ContainerType& 
 */
template <typename KeyType,typename ContainerType>
ContainerType& improc::Container<KeyType,ContainerType>::operator[](const KeyType& key)
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining key {} from container...", key );
    return this->hash_table_[std::move(key)];
}

/**
 * @brief Erase item with key of KeyType from container
 * The method does not throw an error if key does not exist.
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 * @param key 
 */
template <typename KeyType,typename ContainerType>
void improc::Container<KeyType,ContainerType>::Erase(const KeyType& key)
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
 * @tparam KeyType 
 * @tparam ContainerType 
 */
template <typename KeyType,typename ContainerType>
void improc::Container<KeyType,ContainerType>::Clear()
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Deleting container..." );
    this->hash_table_.clear();
}

/**
 * @brief Obtain number of items in container
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 * @return size_t 
 */
template <typename KeyType,typename ContainerType>
size_t improc::Container<KeyType,ContainerType>::Size() const
{
    SPDLOG_LOGGER_CALL( improc::ServicesLogger::get()->data()
                      , spdlog::level::trace
                      , "Obtaining container size..." );
    return this->hash_table_.size();
}
