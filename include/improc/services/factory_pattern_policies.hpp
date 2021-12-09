#ifndef IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP

namespace improc
{
    template <typename KeyType, class BaseProduct>
    class IMPROC_EXPORTS FactoryPatternError
    {
        protected:
            static std::shared_ptr<BaseProduct> OnUnknownType(const KeyType&)
            {
                throw improc::not_found_in_factory();
            }
    };
}

#endif