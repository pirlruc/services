#include <gtest/gtest.h>

#include <improc/services/chain_responsibility/chain_responsibility_pattern.hpp>

class HandlerA : public improc::BaseHandler<std::string,std::string>
{
    public:
        std::string Handle(const std::string& request) override
        {
            if (request == "A")
            {
                return "A";
            }
            return this->improc::BaseHandler<std::string,std::string>::Handle(request);
        }
};

class HandlerB : public improc::BaseHandler<std::string,std::string>
{
    public:
        std::string Handle(const std::string& request) override
        {
            if (request == "B")
            {
                return "B";
            }
            return this->improc::BaseHandler<std::string,std::string>::Handle(request);
        }
};

class HandlerC : public improc::BaseHandler<std::string,std::string>
{
    public:
        std::string Handle(const std::string& request) override
        {
            if (request == "C")
            {
                return "C";
            }
            return this->improc::BaseHandler<std::string,std::string>::Handle(request);
        }
};

TEST(BaseHandler,TestEmptyConstructor) {
    improc::BaseHandler<std::string,std::string> handler_empty {};
    EXPECT_EQ(handler_empty.Handle(""),"");
}

TEST(BaseHandler,TestSetNextHandle) {
    std::shared_ptr<HandlerA> handler_a {std::make_shared<HandlerA>(HandlerA())};
    std::shared_ptr<HandlerB> handler_b {std::make_shared<HandlerB>(HandlerB())};
    std::shared_ptr<HandlerC> handler_c {std::make_shared<HandlerC>(HandlerC())};
    handler_a->set_next_handler(handler_b)->set_next_handler(handler_c);
}

TEST(BaseHandler,TestHandle) {
    std::shared_ptr<HandlerA> handler_a {std::make_shared<HandlerA>(HandlerA())};
    std::shared_ptr<HandlerB> handler_b {std::make_shared<HandlerB>(HandlerB())};
    std::shared_ptr<HandlerC> handler_c {std::make_shared<HandlerC>(HandlerC())};
    handler_a->set_next_handler(handler_b)->set_next_handler(handler_c);
    EXPECT_EQ(handler_a->Handle("A"),"A");
    EXPECT_EQ(handler_a->Handle("B"),"B");
    EXPECT_EQ(handler_a->Handle("C"),"C");
    EXPECT_EQ(handler_a->Handle("D"),"");
    EXPECT_EQ(handler_b->Handle("A"),"");
    EXPECT_EQ(handler_b->Handle("B"),"B");
    EXPECT_EQ(handler_b->Handle("C"),"C");
    EXPECT_EQ(handler_b->Handle("D"),"");
    EXPECT_EQ(handler_c->Handle("A"),"");
    EXPECT_EQ(handler_c->Handle("B"),"");
    EXPECT_EQ(handler_c->Handle("C"),"C");
    EXPECT_EQ(handler_c->Handle("D"),"");
}
