#ifndef IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP

#include <improc/exception.hpp>
#include <improc/improc_type_traits.hpp>

namespace improc
{
    template <typename KeyType, class BaseProduct>
    struct IMPROC_API FactoryPatternError
    {
        static std::shared_ptr<BaseProduct> OnUnknownType(const KeyType&)
        {
            static_assert(improc::is_hashable_v<KeyType>, "KeyType should be an integral or a string data type.");
            throw improc::not_found_in_factory();
        }
    };
}

#endif