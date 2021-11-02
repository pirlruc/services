#ifndef IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP

namespace improc
{
    template <typename KeyType, class BaseProduct>
    class FactoryPatternError
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
                throw improc::FactoryPatternError<KeyType,BaseProduct>::not_found_type();
            }
    };
}

#endif