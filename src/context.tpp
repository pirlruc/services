/**
 * @brief Construct a new improc::Context<KeyType,ContainerType>::Context object
 * 
 * @tparam KeyType 
 * @tparam ContainerType 
 */
template <typename KeyType, typename ContainerType>
improc::Context<KeyType,ContainerType>::Context() : improc::Container<KeyType,ContainerType>() {}

template <typename KeyType, typename ContainerType>
improc::Context<KeyType,ContainerType>& improc::Context<KeyType,ContainerType>::Add(const KeyType& key, const ContainerType& item)
{
    this->improc::Container<KeyType,ContainerType>::Add(key,item);
    return (*this);
}

template <typename KeyType, typename ContainerType>
improc::Context<KeyType,ContainerType>& improc::Context<KeyType,ContainerType>::Erase(const KeyType& key)
{
    this->improc::Container<KeyType,ContainerType>::Erase(key);
    return (*this);
}

template <typename KeyType, typename ContainerType>
improc::Context<KeyType,ContainerType>& improc::Context<KeyType,ContainerType>::Clear()
{
    this->improc::Container<KeyType,ContainerType>::Clear();
    return (*this);
}
