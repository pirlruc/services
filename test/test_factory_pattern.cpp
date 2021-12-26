#include <gtest/gtest.h>

#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory_pattern.hpp>
#include <improc/services/factory.hpp>

#include <improc_services_test_config.hpp>

class IncrementTestDS : public improc::StringKeyHeterogeneousBaseService
{
    public:
        IncrementTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        IncrementTestDS& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            spdlog::info("--- INCREMENT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            return (*this);
        }

        void Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) + 1;
            spdlog::info("Increment Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class SubtractTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_subtract_ = 0;

    public:
        SubtractTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        SubtractTestDS& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for subtract service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_subtract_ = service_field_iter->asInt();
                    break;
                }
            }
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number: {}",this->number_to_subtract_);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) - this->number_to_subtract_;
            spdlog::info("Subtract Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

class MultiplyTestDS : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_multiply_ = 1;

    public:
        MultiplyTestDS() : improc::StringKeyHeterogeneousBaseService() {}
        MultiplyTestDS& Load   (const Json::Value& service_json) override
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);

            for (Json::Value::const_iterator service_field_iter = service_json.begin(); service_field_iter != service_json.end(); ++service_field_iter)
            {
                const std::string kNumberKey = "number";
                spdlog::debug("Analyzing field {} for multiply service...",service_field_iter.name());

                if (service_field_iter.name() == kNumberKey)
                {
                    this->number_to_multiply_ = service_field_iter->asInt();
                    break;
                }
            }
            spdlog::info("--- MULTIPLY SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number: {}",this->number_to_multiply_);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override
        {
            context[this->outputs_[0]] = std::any_cast<int>(context.Get(this->inputs_[0])) * this->number_to_multiply_;
            spdlog::info("Multiply Service: ori = {}",std::any_cast<int>(context[this->outputs_[0]]));
        }
};

typedef improc::FactoryPattern<improc::StringKeyHeterogeneousBaseService,std::string,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>> FactoryPattern;

TEST(FactoryPattern,TestFactoryEmptyConstructor) {
    FactoryPattern factory {};
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Unregister("test"),false);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(FactoryPattern,TestAddItemsToFactory) {
    FactoryPattern factory {};
    EXPECT_EQ(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>}),true);
    EXPECT_EQ(factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} ),true);
    EXPECT_EQ(factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} ),true);
    EXPECT_EQ(factory.GetRegisteredIds().size(),3);    
    EXPECT_EQ(factory.Size(),3);    
}

TEST(FactoryPattern,TestDuplicateItemsToFactory) {
    FactoryPattern factory {};
    EXPECT_EQ(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>}),true);
    EXPECT_EQ(factory.Register("increment" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>}),false);
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.GetRegisteredIds()[0],"increment");    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(FactoryPattern,TestRemoveItemsFromFactory) {
    FactoryPattern factory {};
    EXPECT_EQ(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>}),true);
    EXPECT_EQ(factory.Unregister("increment"),true);
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(FactoryPattern,TestSequenceServiceLoadWithInputError) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    FactoryPattern factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTestDS>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestDS>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTestDS>} );

    EXPECT_NO_THROW(std::shared_ptr<improc::StringKeyHeterogeneousBaseService> increment = factory.Create("increment",json_content));
}
