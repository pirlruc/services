#ifndef IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP
#define IMPROC_SERVICES_FACTORY_PATTERN_POLICIES_HPP

#include <improc/exception.hpp>
#include <improc/improc_type_traits.hpp>

namespace improc
{
    /**
     * @brief Factory pattern error policy throws an error if key is not found in factory.
     * 
     * @tparam KeyType - data type of the access key to the factory
     * @tparam BaseProduct - output type of the factory pattern 
     */
    template <typename KeyType, class BaseProduct>
    struct IMPROC_API FactoryPatternError
    {
        static std::shared_ptr<BaseProduct> OnUnknownType(const KeyType& id)
        {
            static_assert(improc::is_hashable_v<KeyType>, "KeyType should be an integral or a string data type.");
            std::string error_message = fmt::format("Unknown object type passed to factory with id {}", id);
            IMPROC_SERVICES_LOGGER_ERROR("ERROR_01: " + error_message);
            throw improc::key_error(std::move(error_message));
        }
    };
}

#endif