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
    class TemplateFactory : public FactoryErrorPolicy<KeyType,BaseProduct>
    {
        private:
            typedef std::unordered_map<KeyType,ProductCreator> CallbackMap;
            CallbackMap callbacks_;

        public:
            TemplateFactory();

            bool                    Register    (const KeyType& id, ProductCreator creator);
            bool                    Unregister  (const KeyType& id);

            std::vector<KeyType>    GetRegisteredIds()  const;
            size_t                  Size()              const;

            template <typename ... Args>
            std::shared_ptr<BaseProduct>    Create(const KeyType& id, Args&& ... args);
    };
}

#include <template_factory.tpp>

#endif