#ifndef IMPROC_SERVICES_BASE_SERVICE_HPP
#define IMPROC_SERVICES_BASE_SERVICE_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/context.hpp>
#include <improc/infrastructure/file.hpp>
#include <improc/infrastructure/jsonfile_parser.hpp>

#include <json/json.h>
#include <vector>

namespace improc 
{
    /**
     * @brief Skeleton for base service with key_type inputs and output names.
     * 
     * @tparam key_type 
     */
    template <typename key_type>
    class IMPROC_EXPORTS BaseService
    {
        protected:
            std::vector<key_type>   inputs_;
            std::vector<key_type>   outputs_;

        public:
            BaseService();

            virtual void        Load            (const Json::Value& service_json);
            virtual void        Run             (Context<key_type>& context) const = 0;
    };

    typedef BaseService<std::string>    StringKeyBaseService;
}

#include <base_service.tpp>

#endif
