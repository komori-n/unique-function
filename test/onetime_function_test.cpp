#include "komori/onetime_function.hpp"
#include "gtest/gtest.h"


namespace {
int test_func(void) {
    return 334;
}

struct TestObj {
    TestObj(void) = default;
    TestObj(const TestObj&) = delete;
    TestObj(TestObj&&) noexcept = default;
    TestObj& operator=(const TestObj&) = delete;
    TestObj& operator=(TestObj&&) noexcept = default;
    ~TestObj(void) = default;

    int func(void) const { return 346; }
    int operator()(void) const { return 227; }
};

TEST(OnetimeFunctionTest, Constructor) {
    // function pointer
    komori::onetime_function<int(void)> f1 = test_func;
    EXPECT_EQ(f1(), 334);
    EXPECT_THROW(f1(), std::runtime_error);

    // lambda
    komori::onetime_function<int(void)> f2 = [](void) { return 264; };
    EXPECT_EQ(f2(), 264);
    EXPECT_THROW(f2(), std::runtime_error);

    // function object
    komori::onetime_function<int(void)> f3 = TestObj{};
    EXPECT_EQ(f3(), 227);
    EXPECT_THROW(f3(), std::runtime_error);

    // default
    komori::onetime_function<int(void)> f4;
    EXPECT_FALSE(f4);
    EXPECT_THROW(f4(), std::runtime_error);

    // nullptr
    komori::onetime_function<int(void)> f5;
    EXPECT_FALSE(f5);
    EXPECT_THROW(f5(), std::runtime_error);

    // member function pointer
    komori::onetime_function<int(const TestObj&)> f6 = &TestObj::func;
    EXPECT_EQ(f6(TestObj{}), 346);
    EXPECT_THROW(f6(TestObj{}), std::runtime_error);

    // move constructor
    komori::onetime_function<int(void)> f7 = TestObj{};
    komori::onetime_function<int(void)> f8 = std::move(f7);
    EXPECT_FALSE(f7);
    EXPECT_EQ(f8(), 227);
    EXPECT_THROW(f8(), std::runtime_error);
}

TEST(UniqueFunctionTest, Destructor) {
    komori::onetime_function<int(void)> f1;

    EXPECT_FALSE(f1);
    {
        komori::onetime_function<int(void)> f2 = [](void) { return 264; };
        f1 = std::move(f2);
        EXPECT_FALSE(f2);
    }
    ASSERT_TRUE(f1);
    EXPECT_EQ(f1(), 264);
}

TEST(UniqueFunctionTest, AssignOperator) {
    // function pointer
    komori::onetime_function<int(void)> f1;
    f1 = test_func;
    EXPECT_EQ(f1(), 334);

    // lambda
    komori::onetime_function<int(void)> f2;
    f2 = [](void) { return 264; };
    EXPECT_EQ(f2(), 264);

    // function object
    komori::onetime_function<int(void)> f3;
    f3 = TestObj{};
    EXPECT_EQ(f3(), 227);

    // nullptr
    komori::onetime_function<int(void)> f4;
    f4 = nullptr;
    EXPECT_FALSE(f4);

    // member function pointer
    komori::onetime_function<int(const TestObj&)> f5;
    f5 = &TestObj::func;
    EXPECT_EQ(f5(TestObj{}), 346);

    // move constructor
    komori::onetime_function<int(void)> f6;
    komori::onetime_function<int(void)> f7 = TestObj{};
    f6 = std::move(f7);
    EXPECT_FALSE(f7);
    EXPECT_EQ(f6(), 227);
}

TEST(UniqueFunctionTest, BoolOperator) {
    komori::onetime_function<int(void)> f1;
    EXPECT_FALSE(f1);

    komori::onetime_function<int(void)> f2 = [](void) { return 227; };
    EXPECT_TRUE(f2);
    f2();
    EXPECT_FALSE(f2);
}

TEST(UniqueFunctionTest, Swap) {
    komori::onetime_function<int(void)> f1 = [](void) { return 227; };
    komori::onetime_function<int(void)> f2 = [](void) { return 346; };
    f1.swap(f2);
    EXPECT_EQ(f1(), 346);
    EXPECT_EQ(f2(), 227);

    komori::onetime_function<int(void)> f3 = [](void) { return 227; };
    komori::onetime_function<int(void)> f4 = [](void) { return 346; };
    swap(f3, f4);
    EXPECT_EQ(f3(), 346);
    EXPECT_EQ(f4(), 227);
}

TEST(UniqueFunctionTest, EqualOperator) {
    komori::onetime_function<int(void)> f1 = [](void) { return 227; };
    komori::onetime_function<int(void)> f2;
    EXPECT_FALSE(f1 == nullptr);
    EXPECT_FALSE(nullptr == f1);
    EXPECT_TRUE(f2 == nullptr);
    EXPECT_TRUE(nullptr == f2);
}

}  // namespace