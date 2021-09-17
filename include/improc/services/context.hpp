#ifndef IMPROC_SERVICES_CONTEXT_HPP
#define IMPROC_SERVICES_CONTEXT_HPP

#include <improc/improc_defs.hpp>
#include <improc/services/container.hpp>

#include <string>

namespace improc
{
    /**
     * @brief Heterogeneous context with elements identified by key_type  
     * 
     * @tparam key_type 
     */
    template <typename key_type>
    class IMPROC_EXPORTS Context : public Container<key_type,std::any>
    {
        public:
            Context();
    };

    typedef Context<std::string>    StringKeyContext;
}

#include <context.tpp>

#endif