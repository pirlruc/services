template<class RequestType, class HandleOutputType>
improc::Handler<RequestType,HandleOutputType>::Handler(): next_handler_(nullptr) {};

template<class RequestType, class HandleOutputType>
improc::BaseHandler<RequestType,HandleOutputType>::BaseHandler(): improc::Handler<RequestType,HandleOutputType>() {};

template<class RequestType, class HandleOutputType>
std::shared_ptr<improc::Handler<RequestType,HandleOutputType>> improc::BaseHandler<RequestType,HandleOutputType>::set_next_handler(std::shared_ptr<Handler<RequestType,HandleOutputType>> next_handler)
{
    IMPROC_SERVICES_LOGGER_TRACE("Setting next handler...");
    this->next_handler_ = next_handler;
    return next_handler;
}

template<class RequestType, class HandleOutputType>
HandleOutputType improc::BaseHandler<RequestType,HandleOutputType>::Handle(const RequestType& request)
{
    if (this->next_handler_ != nullptr)
    {
        return this->next_handler_->Handle(request);
    }
    return HandleOutputType();
}
