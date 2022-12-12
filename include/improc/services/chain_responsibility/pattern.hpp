#ifndef IMPROC_SERVICES_CHAIN_RESPONSIBILITY_PATTERN_HPP
#define IMPROC_SERVICES_CHAIN_RESPONSIBILITY_PATTERN_HPP

#include <improc/services/logger_services.hpp>

#include <memory>

namespace improc
{
    /**
     * @brief Template for chain of responsibility handlers
     * 
     * @tparam RequestType - Data type of the request to the chain of responsibility
     * @tparam HandleOutputType - Data type of the output from the handle
     */
    template<class RequestType, class HandleOutputType>
    class IMPROC_API Handler 
    {
        protected:
            std::shared_ptr<Handler>            next_handler_;
        
        public:
            Handler();

            virtual std::shared_ptr<Handler>    set_next_handler(std::shared_ptr<Handler> handler) = 0;
            virtual HandleOutputType            Handle(const RequestType& request) = 0;
    };

    /**
     * @brief Chain of responsibility handlers base
     * 
     * @tparam RequestType - Data type of the request to the chain of responsibility
     * @tparam HandleOutputType - Data type of the output from the handle
     */
    template<class RequestType, class HandleOutputType>
    class IMPROC_API BaseHandler : public improc::Handler<RequestType,HandleOutputType>
    {
        public:
            BaseHandler();

            std::shared_ptr<Handler<RequestType,HandleOutputType>>  set_next_handler(std::shared_ptr<Handler<RequestType,HandleOutputType>> next_handler) final;
            HandleOutputType                                        Handle(const RequestType& request);
    };    
}

#include <improc/services/chain_responsibility/pattern.tpp>

#endif