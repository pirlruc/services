#include <gtest/gtest.h>

#include <improc/services/container/container.hpp>

TEST(Container,TestEmptyContainerConstructor) {
    improc::Container<int,std::any> container_empty {};
    EXPECT_EQ(container_empty.Size(),0);
}

TEST(Container,TestAddItemToContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    EXPECT_EQ(container.Size(),1);
}

TEST(Container,TestExistsInContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    EXPECT_TRUE(container.Exists(1));
    EXPECT_FALSE(container.Exists(2));
}

TEST(Container,TestAddDuplicateItemToContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    EXPECT_THROW(container.Add(1,"Test 2"),improc::duplicated_key);
}

TEST(Container,TestAddDifferentItemsToContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    EXPECT_EQ(container.Size(),2);
}

TEST(Container,TestGetExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    EXPECT_EQ(container.Get(1).type(),typeid(const char*));
    EXPECT_EQ(container.Get(2).type(),typeid(int));
    EXPECT_EQ(std::any_cast<int>(container.Get(2)),45);
    EXPECT_STREQ(std::any_cast<const char*>(container.Get(1)),"Test 1");
}

TEST(Container,TestGetNonExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    EXPECT_THROW(container.Get(1),improc::not_found_key);
}

TEST(Container,TestEraseNonExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    EXPECT_FALSE(container.Erase(2));
    EXPECT_EQ(container.Size(),1);
}

TEST(Container,TestEraseExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    EXPECT_TRUE(container.Erase(1));
    EXPECT_EQ(container.Size(),1);
}

TEST(Container,TestClear) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    container.Clear();
    EXPECT_EQ(container.Size(),0);
}

TEST(Container,TestChangeExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    container[2] = 0.15;
    EXPECT_EQ(container.Size(),2);
    EXPECT_EQ(container.Get(2).type(),typeid(double));
    EXPECT_EQ(std::any_cast<double>(container.Get(2)),0.15);
}

TEST(Container,TestChangeNonExistingItemFromContainer) {
    improc::Container<int,std::any> container {};
    container.Add(1,"Test 1");
    container.Add(2,45);
    container[3] = 0.15;
    EXPECT_EQ(container.Size(),3);
    EXPECT_EQ(container.Get(3).type(),typeid(double));
    EXPECT_EQ(std::any_cast<double>(container.Get(3)),0.15);
}
