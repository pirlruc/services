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
    class Container
    {
        private:
            std::unordered_map<KeyType,ContainerType> hash_table_;

        public:
            Container();

            void            Add         (const KeyType& key, const ContainerType& item);
            ContainerType   Get         (const KeyType& key) const;
            ContainerType&  operator[]  (const KeyType& key);

            void            Erase       (const KeyType& key);
            void            Clear();

            size_t          Size() const;
    };

    typedef Container<std::string,std::any> StringKeyHeterogeneousContainer;
}

#include <container.tpp>

#endif