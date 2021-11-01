#ifndef IMPROC_SERVICES_TEMPLATE_FACTORY_HPP
#define IMPROC_SERVICES_TEMPLATE_FACTORY_HPP

namespace improc
{
    template <typename KeyType, class BaseProduct>
    class FactoryError
    {
        public:
            class not_found_type : public std::exception
            {
                public:
                    const char* what() const throw()
                    {
                        return "Unknown object type passed to factory.";
                    }
            };
        
        protected:
            static std::shared_ptr<BaseProduct> OnUnknownType(const KeyType&)
            {
                throw improc::FactoryError<KeyType,BaseProduct>::not_found_type();
            }
    };

    template    <   class    BaseProduct
                ,   typename KeyType
                ,   typename ProductCreator = std::function<std::shared_ptr<BaseProduct>>()
                ,   template <typename,class> class FactoryErrorPolicy = improc::FactoryError 
                >
    class TemplateFactory : public  FactoryErrorPolicy<KeyType,BaseProduct>
    {
        private:
            typedef std::unordered_map<KeyType,ProductCreator> CallbackMap;
            CallbackMap callbacks_;

        public:
            TemplateFactory() : callbacks_() {}

            bool                Register(const KeyType& id, ProductCreator creator)
            {
                SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                                    , spdlog::level::trace
                                    , "Registering ID {} in factory...", id );
                return this->callbacks_.insert(typename CallbackMap::value_type(id,creator)).second != 0;
            }

            bool                Unregister(const KeyType& id)
            {
                SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                                    , spdlog::level::trace
                                    , "Unregistering ID {} from factory...", id );
                return this->callbacks_.erase(id) != 0;
            }

            std::vector<KeyType> GetRegisteredIds()
            {
                SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                                    , spdlog::level::trace
                                    , "Obtaining IDs from factory...");
                std::vector<KeyType> ids;
                for(typename CallbackMap::const_iterator iter_callback = this->callbacks_.begin(); iter_callback != this->callbacks_.end(); ++iter_callback)
                {
                    ids.push_back(iter_callback->first);
                }
                return ids;
            }

            template<typename ... Args>
            std::shared_ptr<BaseProduct>    Create(const KeyType& id, Args&& ... args)
            {
                SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                                    , spdlog::level::trace
                                    , "Creating object from factory ID {}...", id);
                typename CallbackMap::const_iterator iter_callback = this->callbacks_.find(id);
                if (iter_callback != this->callbacks_.end())
                {
                    return std::invoke(iter_callback->second,std::forward<Args>(args) ...);
                }
                return this->OnUnknownType(id);
            }

            size_t          Size() const
            {
                SPDLOG_LOGGER_CALL  ( improc::ServicesLogger::get()->data()
                                    , spdlog::level::trace
                                    , "Obtaining size of factory...");
                return this->callbacks_.size();
            }
    };
}

#endif