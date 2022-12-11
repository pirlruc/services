#ifndef IMPROC_SERVICES_CONTAINER_HPP
#define IMPROC_SERVICES_CONTAINER_HPP

#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>

#include <unordered_map>
#include <any>
#include <string>

namespace improc
{
    /**
     * @brief The container can be used to define homogeneous or heterogeneous
     * containers with different types of keys. The container does not allow
     * duplicate keys.
     * 
     * @tparam KeyType 
     * @tparam ContainerType 
     */
    template <typename KeyType,typename ContainerType>
    class IMPROC_API Container
    {
        private:
            typedef std::unordered_map<KeyType,ContainerType> HashMap;
            HashMap         hash_table_;

        public:
            Container();

            bool            Exists      (const KeyType& key) const;
            ContainerType   Get         (const KeyType& key) const;
            ContainerType&  operator[]  (const KeyType& key);

            Container&      Add         (const KeyType& key, const ContainerType& item);
            bool            Erase       (const KeyType& key);
            Container&      Clear();

            size_t          Size() const;
    };

    typedef Container<std::string,std::any> StringKeyHeterogeneousContainer;
}

#include <improc/services/container/container.tpp>

#endif