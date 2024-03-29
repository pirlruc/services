#ifndef IMPROC_SERVICES_SEQUENCE_SERVICE_HPP
#define IMPROC_SERVICES_SEQUENCE_SERVICE_HPP

#include <improc/improc_defs.hpp>
#include <improc/exception.hpp>
#include <improc/services/logger_services.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/infrastructure/parsers/json_parser.hpp>

#include <json/json.h>
#include <vector>

namespace improc
{
    // TODO: Review implementation
    /**
     * @brief Define an orderered sequence of services that should be executed together
     * 
     * @tparam KeyType 
     * @tparam ContextType 
     */
    template <typename KeyType,typename ContextType>
    class IMPROC_API SequenceService
    {
        private:
            struct Service
            {
                KeyType                                             type;
                std::shared_ptr<BaseService<KeyType,ContextType>>   data;
            };

            std::vector<Service>                                    data_;

        public:
            SequenceService();

            SequenceService&    Load    ( const ServicesFactory<KeyType,ContextType>& factory
                                        , const Json::Value& sequence_service_json );
            void                Run     ( improc::Context<KeyType,ContextType>& context ) const;

            size_t              Size() const;
    };

    typedef SequenceService<std::string,std::any>   StringKeyHeterogeneousSequenceService;
}

#include <improc/services/sequence_service.tpp>

#endif