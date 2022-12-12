/**
 * @brief Construct a new improc::Handler<RequestType,HandleOutputType> object
 * 
 * @tparam RequestType - Data type of the request to the chain of responsibility
 * @tparam HandleOutputType - Data type of the output from the handle
 */
template<class RequestType, class HandleOutputType>
improc::Handler<RequestType,HandleOutputType>::Handler(): next_handler_(nullptr) {};

/**
 * @brief Construct a new improc::BaseHandler<RequestType,HandleOutputType> object
 * 
 * @tparam RequestType - Data type of the request to the chain of responsibility
 * @tparam HandleOutputType - Data type of the output from the handle
 */
template<class RequestType, class HandleOutputType>
improc::BaseHandler<RequestType,HandleOutputType>::BaseHandler(): improc::Handler<RequestType,HandleOutputType>() {};

/**
 * @brief Sets next handler to chain of responsibility
 * 
 * @param next_handler - next handler pointer 
 * @tparam RequestType - Data type of the request to the chain of responsibility
 * @tparam HandleOutputType - Data type of the output from the handle
 * @return std::shared_ptr<improc::Handler<RequestType,HandleOutputType>> - next handler pointer
 */
template<class RequestType, class HandleOutputType>
std::shared_ptr<improc::Handler<RequestType,HandleOutputType>> improc::BaseHandler<RequestType,HandleOutputType>::set_next_handler(std::shared_ptr<Handler<RequestType,HandleOutputType>> next_handler)
{
    IMPROC_SERVICES_LOGGER_TRACE("Setting next handler...");
    this->next_handler_ = std::move(next_handler);
    return this->next_handler_;
}

/**
 * @brief Sets next handler to chain of responsibility
 * 
 * @param request - request information
 * @tparam RequestType - Data type of the request to the chain of responsibility
 * @tparam HandleOutputType - Data type of the output from the handle
 * @return HandleOutputType
 */
template<class RequestType, class HandleOutputType>
HandleOutputType improc::BaseHandler<RequestType,HandleOutputType>::Handle(const RequestType& request)
{
    if (this->next_handler_ != nullptr)
    {
        return this->next_handler_->Handle(std::move(request));
    }
    return HandleOutputType();
}
