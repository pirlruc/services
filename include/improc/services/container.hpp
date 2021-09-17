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
     * @tparam key_type 
     * @tparam container_type 
     */
    template <typename key_type,typename container_type>
    class Container
    {
        private:
            std::unordered_map<key_type,container_type> hash_table_;

        public:
            Container();

            void            Add         (const key_type& key, const container_type& item);
            container_type  Get         (const key_type& key) const;
            container_type& operator[]  (const key_type& key);

            void            Erase       (const key_type& key);
            void            Clear();

            size_t          Size() const;
    };

    typedef Container<std::string,std::any> StringKeyHeterogeneousContainer;
}

#include <container.tpp>

#endif