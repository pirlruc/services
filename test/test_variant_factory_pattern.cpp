#include <gtest/gtest.h>

#include <base_service_tests_def.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory/variant_factory_pattern.hpp>
#include <improc/services/factory.hpp>

#include <improc_services_test_config.hpp>

class SubtractTestWithOneInputLoad : public improc::StringKeyHeterogeneousBaseService
{
    private: 
        int number_to_subtract_1_ = 0;
        int number_to_subtract_2_ = 0;

    public:
        SubtractTestWithOneInputLoad() : improc::StringKeyHeterogeneousBaseService() {}
        SubtractTestWithOneInputLoad& Load   (const Json::Value& service_json, int number_to_subtract)
        {
            this->improc::StringKeyHeterogeneousBaseService::Load(service_json);
            this->number_to_subtract_2_ = number_to_subtract;
            spdlog::info("--- SUBTRACT SERVICE ---");
            spdlog::info("Input : {}",this->inputs_[0]);
            spdlog::info("Output: {}",this->outputs_[0]);
            spdlog::info("Number 1: {}",this->number_to_subtract_1_);
            spdlog::info("Number 2: {}",this->number_to_subtract_2_);
            return (*this);
        }

        void    Run    (improc::StringKeyHeterogeneousContext&  context) const override {}
};

template <class ServiceType>
std::shared_ptr<improc::StringKeyHeterogeneousBaseService> TestLoadServiceFromJson(const Json::Value& service_json)
{
    ServiceType service {};
    std::shared_ptr<improc::StringKeyHeterogeneousBaseService> base_service {std::make_shared<ServiceType>(service.Load(service_json))};
    return base_service;
}

template <class ServiceType>
std::shared_ptr<improc::StringKeyHeterogeneousBaseService> TestLoadServiceFromJsonInt(const Json::Value& service_json, int number)
{
    ServiceType service {};
    std::shared_ptr<improc::StringKeyHeterogeneousBaseService> base_service {std::make_shared<ServiceType>(service.Load(service_json,number))};
    return base_service;
}

typedef improc::VariantFactoryPattern   < improc::StringKeyHeterogeneousBaseService,std::string
                                        , std::variant  < std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>
                                                        , std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&,int)>
                                                        >
                                        > VariantFactoryPattern;

TEST(VariantFactoryPattern,TestFactoryEmptyConstructor) {
    VariantFactoryPattern factory {};
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Unregister("test"),false);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(VariantFactoryPattern,TestAddItemsToFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>     {&TestLoadServiceFromJson<IncrementTest>}));
    EXPECT_NO_THROW(factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestWithOneInputLoad>} ));
    EXPECT_EQ(factory.GetRegisteredIds().size(),2);    
    EXPECT_EQ(factory.Size(),2);    
}

TEST(VariantFactoryPattern,TestDuplicateItemsToFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>   {&TestLoadServiceFromJson<IncrementTest>}));
    EXPECT_THROW(factory.Register("increment" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestWithOneInputLoad>}),improc::duplicated_key);
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.GetRegisteredIds()[0],"increment");    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(VariantFactoryPattern,TestRemoveExistingItemFromFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTest>}));
    EXPECT_TRUE(factory.Unregister("increment"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(VariantFactoryPattern,TestRemoveNonExistingItemFromFactory) {
    VariantFactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTest>}));
    EXPECT_FALSE(factory.Unregister("decrement"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(VariantFactoryPattern,TestCreationExceptionFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    VariantFactoryPattern factory {};
    EXPECT_THROW(factory.Create<std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>>("increment",json_content),improc::not_found_in_factory);
}

TEST(VariantFactoryPattern,TestCreationFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    VariantFactoryPattern factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&TestLoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&,int)> {&TestLoadServiceFromJsonInt<SubtractTestWithOneInputLoad>} );

    EXPECT_NO_THROW(factory.Create<std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>>("increment",json_content));
    EXPECT_NO_THROW(factory.Create<std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&,int)>>("subtract",json_content,3));
}
