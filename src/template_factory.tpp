template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::TemplateFactory() : callbacks_() {}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Register(const KeyType& id, ProductCreator creator)
{
    SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                        , spdlog::level::trace
                        , "Registering ID {} in factory...", id );
    return this->callbacks_.insert(typename CallbackMap::value_type(id,creator)).second != 0;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Unregister(const KeyType& id)
{
    SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                        , spdlog::level::trace
                        , "Unregistering ID {} from factory...", id );
    return this->callbacks_.erase(id) != 0;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
std::vector<KeyType> improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::GetRegisteredIds() const
{
    SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                        , spdlog::level::trace
                        , "Obtaining IDs from factory...");
    std::vector<KeyType> ids {};
    for (typename CallbackMap::const_iterator iter_callback = this->callbacks_.begin(); iter_callback != this->callbacks_.end(); ++iter_callback)
    {
        ids.push_back(iter_callback->first);
    }
    return ids;
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Size() const
{
    SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                        , spdlog::level::trace
                        , "Obtaining size of factory...");
    return this->callbacks_.size();
}

template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
template    <   typename ... Args   >
std::shared_ptr<BaseProduct> improc::TemplateFactory<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Create(const KeyType& id, Args&& ... args)
{
    SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                        , spdlog::level::trace
                        , "Creating object from factory with ID {}...", id);
    typename CallbackMap::const_iterator iter_callback = this->callbacks_.find(id);
    if (iter_callback != this->callbacks_.end())
    {
        return std::invoke(iter_callback->second,std::forward<Args>(args) ...);
    }
    return this->OnUnknownType(id);
}
