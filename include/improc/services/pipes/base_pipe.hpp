#ifndef IMPROC_SERVICES_BASE_PIPE_HPP
#define IMPROC_SERVICES_BASE_PIPE_HPP

#include <improc/improc_defs.hpp>
#include <improc/services/logger_services.hpp>

#include <pipes/base.hpp>
#include <pipes/send.hpp>
#include <pipes/helpers/assignable.hpp>
#include <pipes/helpers/invoke.hpp>

namespace improc 
{
    template<typename Function>
    class IMPROC_EXPORTS BasePipe : public pipes::pipe_base
    {
        protected:
            pipes::detail::assignable<Function> function_;

        public:
            explicit BasePipe(Function function) : function_(function) {};
    };

    template<typename Function>
    class IMPROC_EXPORTS TransformPipe  : public improc::BasePipe<Function>
    {
        public:
            explicit TransformPipe(Function function) : improc::BasePipe<Function>(function) {};

            template<typename ... Values, typename TailPipeline>
            void onReceive(Values&&... values, TailPipeline&& tail_pipeline)
            {
                pipes::send(pipes::detail::invoke(this->function_.get(), std::forward<Values>(values) ...), tail_pipeline);
            }
    };

    template<typename Predicate>
    class IMPROC_EXPORTS FilterPipe : public improc::BasePipe<Predicate>
    {
        public:
            explicit FilterPipe(Predicate predicate) : improc::BasePipe<Predicate>(predicate) {};

            template<typename ... Values, typename TailPipeline>
            void onReceive(Values&&... values, TailPipeline&& tail_pipeline)
            {
                if (this->function_(values ...) == true)
                {
                    pipes::send(std::forward<Values>(values) ..., tail_pipeline);
                }
            }
    };
}

#endif
