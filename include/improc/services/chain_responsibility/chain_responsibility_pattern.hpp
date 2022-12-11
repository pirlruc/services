#ifndef IMPROC_SERVICES_CHAIN_RESPONSIBILITY_PATTERN_HPP
#define IMPROC_SERVICES_CHAIN_RESPONSIBILITY_PATTERN_HPP

#include <improc/services/logger_services.hpp>

#include <memory>

namespace improc
{
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

    template<class RequestType, class HandleOutputType>
    class IMPROC_API BaseHandler : public improc::Handler<RequestType,HandleOutputType>
    {
        public:
            BaseHandler();

            std::shared_ptr<Handler<RequestType,HandleOutputType>>  set_next_handler(std::shared_ptr<Handler<RequestType,HandleOutputType>> next_handler);
            HandleOutputType                                        Handle(const RequestType& request);
    };    
}

#include <improc/services/chain_responsibility/chain_responsibility_pattern.tpp>

#endif