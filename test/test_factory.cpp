#include <gtest/gtest.h>

#include <base_service_tests_def.hpp>
#include <improc/infrastructure/filesystem/file.hpp>
#include <improc/services/context.hpp>
#include <improc/services/base_service.hpp>
#include <improc/services/factory.hpp>
#include <improc/services/sequence_service.hpp>

#include <improc_services_test_config.hpp>

TEST(Factory,TestFactoryEmptyConstructor) {
    improc::StringKeyHeterogeneousServicesFactory factory {};
    EXPECT_EQ(factory.Size(),0);    
}

TEST(Factory,TestAddItemsToFactory) {
    improc::StringKeyHeterogeneousServicesFactory factory = LoadFactory().factory;
    EXPECT_EQ(factory.Size(),3);    
}
