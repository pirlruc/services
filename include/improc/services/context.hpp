#ifndef IMPROC_SERVICES_CONTEXT_HPP
#define IMPROC_SERVICES_CONTEXT_HPP

#include <improc/improc_defs.hpp>
#include <improc/services/container.hpp>

#include <string>

namespace improc
{
    /**
     * @brief Context with elements identified by KeyType
     * 
     * @tparam KeyType 
     * @tparam ContainerType 
     */
    template <typename KeyType,typename ContainerType>
    class IMPROC_EXPORTS Context : public Container<KeyType,ContainerType>
    {
        public:
            Context();
    };

    typedef Context<std::string,std::any>    StringKeyHeterogeneousContext;
}

#include <context.tpp>

#endif