#include "testsockets.h"

SocketsTest::SocketsTest() {}

SocketsTest::~SocketsTest() {};

void SocketsTest::SetUp() {};

void SocketsTest::TearDown() {};

TEST_F(SocketsTest, NopTest) {
    EXPECT_EQ(true, true);
}
