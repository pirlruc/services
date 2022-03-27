template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::FactoryPattern() : callbacks_() {}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>& improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Register(const KeyType& id, const ProductCreator& creator)
{
    IMPROC_SERVICES_LOGGER_TRACE("Registering ID {} in factory...", id);
    if (this->callbacks_.insert(typename CallbackMap::value_type(id,creator)).second == 0)
    {
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Duplicated ID {} in factory.", id);
        throw improc::duplicated_key();
    }
    return (*this);
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Unregister(const KeyType& id)
{
    IMPROC_SERVICES_LOGGER_TRACE("Unregistering ID {} from factory...", id);
    return this->callbacks_.erase(id) != 0;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
std::vector<KeyType> improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::GetRegisteredIds() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining IDs from factory...");
    std::vector<KeyType> ids {this->callbacks_.size()};
    std::transform  ( this->callbacks_.begin(), this->callbacks_.end(), ids.begin()
                    , [] (const typename CallbackMap::value_type& item) -> KeyType {return item.first;} );
    return ids;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining size of factory...");
    return this->callbacks_.size();
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ... Args   >
std::shared_ptr<BaseProduct> improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Create(const KeyType& id, Args&& ... args) const
{
    IMPROC_SERVICES_LOGGER_TRACE("Creating object from factory with ID {}...", id);
    typename CallbackMap::const_iterator iter_callback = this->callbacks_.find(id);
    if (iter_callback != this->callbacks_.end())
    {
        return pipes::detail::invoke(iter_callback->second,FWD(args) ...);
    }
    return this->OnUnknownType(id);
}
