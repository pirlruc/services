template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::VariantFactoryPattern() : callbacks_() {}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ProductCreator >
improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>& improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Register(const KeyType& id, const ProductCreator& creator)
{
    IMPROC_SERVICES_LOGGER_TRACE("Registering ID {} in variant factory...", id);
    if (this->callbacks_.insert(typename CallbackMap::value_type(id,creator)).second == 0)
    {
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Duplicated ID {} in variant factory.", id);
        throw improc::duplicated_key();
    }
    return (*this);
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Unregister(const KeyType& id)
{
    IMPROC_SERVICES_LOGGER_TRACE("Unregistering ID {} from variant factory...", id);
    return this->callbacks_.erase(id) != 0;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
std::vector<KeyType> improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::GetRegisteredIds() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining IDs from variant factory...");
    std::vector<KeyType> ids {this->callbacks_.size()};
    std::transform  ( this->callbacks_.begin(), this->callbacks_.end(), ids.begin()
                    , [] (const typename CallbackMap::value_type& item) -> KeyType {return item.first;} );
    return ids;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining size of variant factory...");
    return this->callbacks_.size();
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ProductCreator, typename ... Args   >
std::shared_ptr<BaseProduct> improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Create(const KeyType& id, Args&& ... args) const
{
    IMPROC_SERVICES_LOGGER_TRACE("Creating object from variant factory with ID {}...", id);
    typename CallbackMap::const_iterator iter_callback = this->callbacks_.find(id);
    if (iter_callback != this->callbacks_.end())
    {
        if (std::holds_alternative<ProductCreator>(iter_callback->second) == true)
        {
            return std::invoke(std::get<ProductCreator>(iter_callback->second),std::forward<Args>(args) ...);
        }
    }
    return this->OnUnknownType(id);
}