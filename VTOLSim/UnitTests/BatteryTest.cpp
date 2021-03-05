#include "stdafx.h"
#include "gtest/gtest.h"
#include "Battery.h"

class BatteryTestFixture : public ::testing::Test {
public:
    Battery* batt;

    BatteryTestFixture() { }

    void SetUp() {
        batt = new Battery(100, 2.0);
    }

    void TearDown() { 
        delete batt;
    }

    ~BatteryTestFixture() { }
};

/**
  * Testing the Battery class .
  * The type-specific vehicle functions just update class-specific variables
  */
TEST_F(BatteryTestFixture, BatteryTest)
{
    ASSERT_TRUE(batt->isFullyCharged());    // Battery starts full
    
    // Normal drain
    batt->drain(50.0);
    ASSERT_FALSE(batt->needToCharge()); // 1/2 empty
    ASSERT_DOUBLE_EQ(batt->getCurCharge(), 50.0);
    batt->drain(50.0);
    ASSERT_TRUE(batt->needToCharge());  // empty

    // Charge
    batt->charge(1.0);
    ASSERT_FALSE(batt->isFullyCharged());   // 1/2 charge
    ASSERT_DOUBLE_EQ(batt->getCurTimeToCharge(), 1.0);
    batt->charge(1.0);
    ASSERT_TRUE(batt->isFullyCharged());    // full charge

    // Test leftover values
    batt->reset(10.0, 0.0);
    double leftover = batt->drain(15.0);
    ASSERT_DOUBLE_EQ(leftover, 5.0);
    ASSERT_TRUE(batt->needToCharge());

    batt->reset(0.0, 1.0);
    leftover = batt->charge(1.5);
    ASSERT_DOUBLE_EQ(leftover, 0.5);
    ASSERT_TRUE(batt->isFullyCharged());
}