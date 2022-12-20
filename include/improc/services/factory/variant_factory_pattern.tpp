template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::VariantFactoryPattern() : improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>() {};

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ProductCreator >
improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>& improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Register(const KeyType& id, const ProductCreator& creator)
{
    IMPROC_SERVICES_LOGGER_TRACE("Registering ID {} in variant factory...", id);
    if (this->callbacks_.insert(typename improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::CallbackMap::value_type(id,std::move(creator))).second == 0)
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
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Unregister(std::move(id));
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
std::vector<KeyType> improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::GetRegisteredIds() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining IDs from variant factory...");
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::GetRegisteredIds();
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining size of variant factory...");
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Size();
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ProductCreator, typename ... Args   >
std::shared_ptr<BaseProduct> improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Create(const KeyType& id, Args&& ... args) const
{
    IMPROC_SERVICES_LOGGER_TRACE("Creating object from variant factory with ID {}...", id);
    typename improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::CallbackMap::const_iterator iter_callback = this->callbacks_.find(id);
    if (iter_callback != this->callbacks_.end())
    {
        if (std::holds_alternative<ProductCreator>(iter_callback->second) == true)
        {
            return pipes::detail::invoke(std::get<ProductCreator>(std::move(iter_callback->second)),IMPROC_FWD(std::move(args)) ...);
        }
    }
    return this->OnUnknownType(std::move(id));
}
