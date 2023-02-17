/**
 * @brief Construct a new improc::Container<KeyType,ContainerType> object
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 */
template <typename KeyType,typename ContainerType>
improc::Container<KeyType,ContainerType>::Container() 
{
    static_assert(improc::is_hashable_v<KeyType>, "KeyType should be an integral or a string data type.");
    IMPROC_SERVICES_LOGGER_TRACE( "Creating container with {} key and {} container..."
                                , typeid(KeyType).name()
                                , typeid(ContainerType).name() );
}

/**
 * @brief Check if an item with key exists in the container
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @return true if key exists in the container
 * @return false if key does not exist in the container
 */
template <typename KeyType,typename ContainerType>
bool improc::Container<KeyType,ContainerType>::Exists(const KeyType& key) const
{
    IMPROC_SERVICES_LOGGER_TRACE("Checking if item with key {} exists in container...", key);
    return this->hash_table_.find(std::move(key)) != this->hash_table_.end();
}

/**
 * @brief Add an item of ContainerType with key of KeyType in container
 * This is a method that allows a safe insert without allowing an item to be 
 * overwritten.
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @param key 
 * @param item
 */
template <typename KeyType,typename ContainerType>
improc::Container<KeyType,ContainerType>& improc::Container<KeyType,ContainerType>::Add(const KeyType& key, const ContainerType& item)
{
    IMPROC_SERVICES_LOGGER_TRACE("Adding key {} to container...", key);
    if (this->hash_table_.insert(typename HashMap::value_type(key,std::move(item))).second == 0)
    {
        std::string error_message = fmt::format("Duplicated key {} in container", key);
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::key_error(std::move(error_message));
    }
    return (*this);
}

/**
 * @brief Get an item with key of KeyType from container
 * This is a method that ensures that the item exists on the container. If the item
 * does not exist, the method will throw an exception. The retrieved item
 * cannot be edited.
 *  
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @param key 
 * @return ContainerType 
 */
template <typename KeyType,typename ContainerType>
ContainerType improc::Container<KeyType,ContainerType>::Get(const KeyType& key) const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining key {} from container...", key);
    if (this->Exists(key) == true)
    {
        return this->hash_table_.at(std::move(key));
    }
    else
    {
        std::string error_message = fmt::format("Key {} not found in container", key);
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_02: " + error_message);
        throw improc::key_error(std::move(error_message));
    }
}

/**
 * @brief Get an item with key of KeyType from container  
 * This method will retrieve an item if it exists otherwise it will create a new 
 * entry on the container which can be edited. 
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @param key 
 * @return ContainerType& 
 */
template <typename KeyType,typename ContainerType>
ContainerType& improc::Container<KeyType,ContainerType>::operator[](const KeyType& key)
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining key {} from container...", key);
    return this->hash_table_[std::move(key)];
}

/**
 * @brief Erase item with key of KeyType from container
 * The method does not throw an error if key does not exist.
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @param key 
 * @return true if key exists and is removed from the container
 * @return false if key does not exist in the container
 */
template <typename KeyType,typename ContainerType>
bool improc::Container<KeyType,ContainerType>::Erase(const KeyType& key)
{
    IMPROC_SERVICES_LOGGER_TRACE("Deleting key {} from container...", key);
    return this->hash_table_.erase(std::move(key)) != 0;
}

/**
 * @brief Clear items from container
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 */
template <typename KeyType,typename ContainerType>
improc::Container<KeyType,ContainerType>& improc::Container<KeyType,ContainerType>::Clear()
{
    IMPROC_SERVICES_LOGGER_TRACE("Deleting container...");
    this->hash_table_.clear();
    return (*this);
}

/**
 * @brief Obtain number of items in container
 * 
 * @tparam KeyType - data type of the access key to the container
 * @tparam ContainerType - data type of the items in the container
 * @return size_t 
 */
template <typename KeyType,typename ContainerType>
size_t improc::Container<KeyType,ContainerType>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining container size...");
    return this->hash_table_.size();
}
