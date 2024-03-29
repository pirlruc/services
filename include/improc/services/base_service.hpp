#ifndef IMPROC_SERVICES_BASE_SERVICE_HPP
#define IMPROC_SERVICES_BASE_SERVICE_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/context.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/infrastructure/parsers/json_parser.hpp>

#include <json/json.h>
#include <vector>

namespace improc 
{
    // TODO: Review implementation
    /**
     * @brief Skeleton for base service with KeyType inputs and output names.
     * The ContextType defines the type of content that can be exchagend between
     * services.
     * 
     * @tparam KeyType 
     * @tparam ContextType 
     */
    template <typename KeyType,typename ContextType>
    class IMPROC_API BaseService
    {
        protected:
            std::vector<KeyType>    inputs_;
            std::vector<KeyType>    outputs_;

        public:
            BaseService();
            BaseService(const Json::Value& service_json);

            virtual BaseService&    Load            (const Json::Value& service_json);
            virtual void            Run             (Context<KeyType,ContextType>& context) const = 0;
    };

    typedef BaseService<std::string,std::any>   StringKeyHeterogeneousBaseService;
}

#include <improc/services/base_service.tpp>

#endif
