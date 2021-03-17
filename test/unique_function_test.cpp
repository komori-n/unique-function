#include "komori/unique_function.hpp"
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

TEST(UniqueFunctionTest, Constructor) {
    // function pointer
    komori::unique_function<int(void)> f1 = test_func;
    EXPECT_EQ(f1(), 334);

    // lambda
    komori::unique_function<int(void)> f2 = [](void) { return 264; };
    EXPECT_EQ(f2(), 264);

    // function object
    komori::unique_function<int(void)> f3 = TestObj{};
    EXPECT_EQ(f3(), 227);

    // default
    komori::unique_function<int(void)> f4;
    EXPECT_FALSE(f4);

    // nullptr
    komori::unique_function<int(void)> f5;
    EXPECT_FALSE(f5);

    // member function pointer
    komori::unique_function<int(const TestObj&)> f6 = &TestObj::func;
    EXPECT_EQ(f6(TestObj{}), 346);

    // move constructor
    komori::unique_function<int(void)> f7 = std::move(f3);
    EXPECT_FALSE(f3);
    EXPECT_EQ(f7(), 227);
}

TEST(UniqueFunctionTest, Destructor) {
    komori::unique_function<int(void)> f1;

    EXPECT_FALSE(f1);
    {
        komori::unique_function<int(void)> f2 = [](void) { return 264; };
        f1 = std::move(f2);
        EXPECT_FALSE(f2);
    }
    ASSERT_TRUE(f1);
    EXPECT_EQ(f1(), 264);
}

TEST(UniqueFunctionTest, AssignOperator) {
    // function pointer
    komori::unique_function<int(void)> f1;
    f1 = test_func;
    EXPECT_EQ(f1(), 334);

    // lambda
    komori::unique_function<int(void)> f2;
    f2 = [](void) { return 264; };
    EXPECT_EQ(f2(), 264);

    // function object
    komori::unique_function<int(void)> f3;
    f3 = TestObj{};
    EXPECT_EQ(f3(), 227);

    // nullptr
    komori::unique_function<int(void)> f4;
    f4 = nullptr;
    EXPECT_FALSE(f4);

    // member function pointer
    komori::unique_function<int(const TestObj&)> f5;
    f5 = &TestObj::func;
    EXPECT_EQ(f5(TestObj{}), 346);

    // move constructor
    komori::unique_function<int(void)> f6;
    f6 = std::move(f3);
    EXPECT_FALSE(f3);
    EXPECT_EQ(f6(), 227);
}

TEST(UniqueFunctionTest, BoolOperator) {
    komori::unique_function<int(void)> f1;
    EXPECT_FALSE(f1);

    komori::unique_function<int(void)> f2 = [](void) { return 227; };
    EXPECT_TRUE(f2);
}

TEST(UniqueFunctionTest, Swap) {
    komori::unique_function<int(void)> f1 = [](void) { return 227; };
    komori::unique_function<int(void)> f2 = [](void) { return 346; };
    f1.swap(f2);
    EXPECT_EQ(f1(), 346);
    EXPECT_EQ(f2(), 227);

    swap(f1, f2);
    EXPECT_EQ(f2(), 346);
    EXPECT_EQ(f1(), 227);
}

TEST(UniqueFunctionTest, EqualOperator) {
    komori::unique_function<int(void)> f1 = [](void) { return 227; };
    komori::unique_function<int(void)> f2;
    EXPECT_FALSE(f1 == nullptr);
    EXPECT_FALSE(nullptr == f1);
    EXPECT_TRUE(f2 == nullptr);
    EXPECT_TRUE(nullptr == f2);
}

}  // namespace