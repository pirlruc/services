#include <gtest/gtest.h>

#include <base_service_tests_def.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory/factory_pattern.hpp>
#include <improc/services/factory.hpp>

#include <improc_services_test_config.hpp>

typedef improc::FactoryPattern<improc::StringKeyHeterogeneousBaseService,std::string,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)>> FactoryPattern;

TEST(FactoryPatternPolicy,TestFactoryOnUnknownType) {
    improc::FactoryPatternError<std::string,int> factory_error;
    EXPECT_THROW(factory_error.OnUnknownType("1"),improc::key_error);
}

TEST(FactoryPattern,TestFactoryEmptyConstructor) {
    FactoryPattern factory {};
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Unregister("test"),false);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(FactoryPattern,TestAddItemsToFactory) {
    FactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>}));
    EXPECT_NO_THROW(factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} ));
    EXPECT_NO_THROW(factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} ));
    EXPECT_EQ(factory.GetRegisteredIds().size(),3);    
    EXPECT_EQ(factory.Size(),3);    
}

TEST(FactoryPattern,TestDuplicateItemsToFactory) {
    FactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>}));
    EXPECT_NO_THROW(factory.Register("subtract",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>}));
    EXPECT_THROW(factory.Register("increment" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>}),improc::key_error);
    EXPECT_EQ(factory.GetRegisteredIds().size(),2);    
    EXPECT_EQ(factory.GetRegisteredIds()[0],"subtract");    
    EXPECT_EQ(factory.Size(),2);    
}

TEST(FactoryPattern,TestRemoveExistingItemFromFactory) {
    FactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>}));
    EXPECT_TRUE(factory.Unregister("increment"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),0);    
    EXPECT_EQ(factory.Size(),0);    
}

TEST(FactoryPattern,TestRemoveNonExistingItemFromFactory) {
    FactoryPattern factory {};
    EXPECT_NO_THROW(factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>}));
    EXPECT_FALSE(factory.Unregister("decrement"));
    EXPECT_EQ(factory.GetRegisteredIds().size(),1);    
    EXPECT_EQ(factory.Size(),1);    
}

TEST(FactoryPattern,TestCreationExceptionFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    FactoryPattern factory {};
    EXPECT_THROW(factory.Create("increment",json_content),improc::key_error);
}

TEST(FactoryPattern,TestCreationFromFactory) {
    std::string filepath = std::string(IMPROC_SERVICES_TEST_FOLDER) + "/test/data/test_factory_pattern.json";
    improc::JsonFile json_file {filepath};
    Json::Value json_content = json_file.Read();

    FactoryPattern factory {};
    factory.Register("increment",std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<IncrementTest>});
    factory.Register("subtract" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<SubtractTestOneInput>} );
    factory.Register("multiply" ,std::function<std::shared_ptr<improc::StringKeyHeterogeneousBaseService>(const Json::Value&)> {&improc::LoadServiceFromJson<MultiplyTest>} );

    EXPECT_NO_THROW(std::shared_ptr<improc::StringKeyHeterogeneousBaseService> increment = factory.Create("increment",json_content));
}
