#ifndef IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP

#include <improc/exception.hpp>

namespace improc
{
    template <typename KeyType, class BaseProduct>
    struct IMPROC_API FactoryPatternError
    {
        static std::shared_ptr<BaseProduct> OnUnknownType(const KeyType&)
        {
            static_assert( std::is_integral_v<KeyType> || std::is_same_v<KeyType,std::string>
                         , "KeyType should be an integral or a string data type.");
            throw improc::not_found_in_factory();
        }
    };
}

#endif