/**
 * @brief Construct a new improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy> object
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::FactoryPattern() : callbacks_() 
{
    static_assert(improc::is_hashable_v<KeyType>, "KeyType should be an integral or a string data type.");
    if constexpr (!improc::is_variant_v<ProductCreator>)
    {
        static_assert(improc::is_function_v<ProductCreator>, "Product creator must be a functor.");
        using ProductCreatorTraits = improc::function_traits<ProductCreator>;
        static_assert(std::is_same<typename ProductCreatorTraits::return_type, std::shared_ptr<BaseProduct>>::value,"Return type should be a BaseProduct pointer.");
    }
}

/**
 * @brief Registers a new object type to the factory
 * 
 * @param id - identification for the object type in the factory
 * @param creator - functor to create the object type
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>& improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Register(const KeyType& id, const ProductCreator& creator)
{
    IMPROC_SERVICES_LOGGER_TRACE("Registering ID {} in factory...", id);
    if (this->callbacks_.insert(typename CallbackMap::value_type(id,std::move(creator))).second == 0)
    {
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: Duplicated ID {} in factory.", id);
        throw improc::duplicated_key();
    }
    return (*this);
}

/**
 * @brief Unregisters a given object type from the factory
 * 
 * @param id - identification for the object type in the factory
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return true if key exists in the factory and false if key does not exist
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Unregister(const KeyType& id)
{
    IMPROC_SERVICES_LOGGER_TRACE("Unregistering ID {} from factory...", id);
    return this->callbacks_.erase(std::move(id)) != 0;
}

/**
 * @brief Obtains the identifiers for the object types registered in the factory
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return std::vector<KeyType>
 */
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

/**
 * @brief Obtains the number of object types registered in the factory
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return size_t
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename ProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::FactoryPattern<BaseProduct,KeyType,ProductCreator,FactoryErrorPolicy>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining size of factory...");
    return this->callbacks_.size();
}

/**
 * @brief Creates the object type using the functor registered on the factory
 * 
 * @param id - identification for the object type in the factory
 * @param args - arguments to pass to the functor of the object type
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam ProductCreator - functor to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return std::shared_ptr<BaseProduct>
 */
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
        return pipes::detail::invoke(std::move(iter_callback->second),IMPROC_FWD(std::move(args)) ...);
    }
    return this->OnUnknownType(std::move(id));
}
