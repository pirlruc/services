/**
 * @brief Construct a new improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy> object
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::VariantFactoryPattern() : improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>()
{
    static_assert(std::variant_size_v<VariantProductCreator> > 1, "If only one functor is going to be used, please use FactoryPattern.");
    static_assert(improc::is_variant_arg_function_type_v<VariantProductCreator>, "Variant should have functor arguments.");
    static_assert(improc::is_variant_arg_function_return_type_v<std::shared_ptr<BaseProduct>,VariantProductCreator>, "Return type should be a BaseProduct pointer.");
};

/**
 * @brief Registers a new object type to the factory
 * 
 * @param id - identification for the object type in the factory
 * @param creator - functor to create the object type
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 */
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
        std::string error_message = fmt::format("Duplicated ID {} in variant factory", id);
        IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: " + error_message);
        throw improc::key_error(std::move(error_message));
    }
    return (*this);
}

/**
 * @brief Unregisters a given object type from the factory
 * 
 * @param id - identification for the object type in the factory
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return true if key exists in the factory and false if key does not exist
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
bool improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Unregister(const KeyType& id)
{
    IMPROC_SERVICES_LOGGER_TRACE("Unregistering ID {} from variant factory...", id);
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Unregister(std::move(id));
}

/**
 * @brief Obtains the identifiers for the object types registered in the factory
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return std::vector<KeyType>
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
std::vector<KeyType> improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::GetRegisteredIds() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining IDs from variant factory...");
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::GetRegisteredIds();
}

/**
 * @brief Obtains the number of object types registered in the factory
 * 
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @return size_t
 */
template    <   class    BaseProduct
            ,   typename KeyType
            ,   typename VariantProductCreator
            ,   template <typename,class> class FactoryErrorPolicy  >
size_t improc::VariantFactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Size() const
{
    IMPROC_SERVICES_LOGGER_TRACE("Obtaining size of variant factory...");
    return this->improc::FactoryPattern<BaseProduct,KeyType,VariantProductCreator,FactoryErrorPolicy>::Size();
}

/**
 * @brief Creates the object type using the functor registered on the factory
 * 
 * @param id - identification for the object type in the factory
 * @param args - arguments to pass to the functor of the object type
 * @tparam BaseProduct - output type of the factory pattern 
 * @tparam KeyType - data type of the access key to the factory
 * @tparam VariantProductCreator - different functors that allow to create the object of the factory
 * @tparam FactoryErrorPolicy - policy to consider if key is not found in factory
 * @tparam ProductCreator - specific functor to create the object of the factory
 * @return std::shared_ptr<BaseProduct>
 */
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
