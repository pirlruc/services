#include <gtest/gtest.h>

#include <improc/services/context.hpp>
#include <improc_services_test_config.hpp>

TEST(Context,TestContextEmptyConstructor) {
    improc::StringKeyHeterogeneousContext cntxt {};
    EXPECT_EQ(cntxt.Size(),0);    
}

TEST(Context,TestAddItemToContext) {
    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori",2);
    EXPECT_EQ(cntxt.Size(),1);    
    EXPECT_EQ(std::any_cast<int>(cntxt.Get("ori")),2);
}

TEST(Context,TestRemoveItemFromContext) {
    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori-1",2);
    cntxt.Add("ori-2",1);
    cntxt.Erase("ori-1");
    EXPECT_EQ(cntxt.Size(),1);
}

TEST(Context,TestClear) {
    improc::StringKeyHeterogeneousContext cntxt {};
    cntxt.Add("ori-1",2);
    cntxt.Add("ori-2",1);
    cntxt.Clear();
    EXPECT_EQ(cntxt.Size(),0);
}
