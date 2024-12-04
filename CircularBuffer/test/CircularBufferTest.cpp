#include "CircularBuffer.h"
#include <gtest/gtest.h>

TEST(CircularBufferTest, DefaultConstructor) {
CircularBuffer buffer;
EXPECT_EQ(buffer.size(), 0);
EXPECT_EQ(buffer.capacity(), 0);
EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, CopyConstructor) {
CircularBuffer buffer(5, 'A');
CircularBuffer copyBuffer(buffer);

EXPECT_EQ(copyBuffer.size(), buffer.size());
EXPECT_EQ(copyBuffer.capacity(), buffer.capacity());
for (int i = 0; i < buffer.size(); ++i) {
EXPECT_EQ(copyBuffer[i], buffer[i]);
}
}

TEST(CircularBufferTest, ConstructorWithCapacity) {
CircularBuffer buffer(5);
EXPECT_EQ(buffer.size(), 0);
EXPECT_EQ(buffer.capacity(), 5);
EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, MoveConstructor) {
CircularBuffer originalBuffer(5);
originalBuffer.pushBack('A');
originalBuffer.pushBack('B');
originalBuffer.pushBack('C');

ASSERT_EQ(originalBuffer.size(), 3);
ASSERT_EQ(originalBuffer.capacity(), 5);
ASSERT_EQ(originalBuffer.front(), 'A');
ASSERT_EQ(originalBuffer.back(), 'C');

CircularBuffer movedBuffer(std::move(originalBuffer));

EXPECT_EQ(movedBuffer.size(), 3);
EXPECT_EQ(movedBuffer.capacity(), 5);
EXPECT_EQ(movedBuffer.front(), 'A');
EXPECT_EQ(movedBuffer.back(), 'C');
}

TEST(CircularBufferTest, ConstructorWithCapacityAndElement) {
CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer.size(), 5);
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[4], 'A');
}

TEST(CircularBufferTest, IndexOperator) {
CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[4], 'A');
}

TEST(CircularBufferTest, ConstIndexOperator) {
const CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[4], 'A');
}

TEST(CircularBufferTest, AtOperator) {
CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer.at(0), 'A');
EXPECT_EQ(buffer.at(4), 'A');

EXPECT_THROW(buffer.at(5), std::out_of_range);
}

TEST(CircularBufferTest, AtOperatorConst) {
const CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer.at(0), 'A');
EXPECT_EQ(buffer.at(4), 'A');

EXPECT_THROW(buffer.at(5), std::out_of_range);
}


TEST(CircularBufferTest, FrontMethod) {
CircularBuffer emptyBuffer(5);
EXPECT_THROW(emptyBuffer.front(), std::logic_error);

CircularBuffer buffer(5);
buffer.pushBack('A');
EXPECT_EQ(buffer.front(), 'A');
}

TEST(CircularBufferTest, BackMethod) {
CircularBuffer buffer(5);
buffer.pushBack('A');
EXPECT_EQ(buffer.back(), 'A');

CircularBuffer emptyBuffer(5);
EXPECT_THROW(emptyBuffer.back(), std::logic_error);
}

TEST(CircularBufferTest, ConstFrontMethod) {
const CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer.front(), 'A');

const CircularBuffer emptyBuffer(5);
EXPECT_THROW(emptyBuffer.front(), std::logic_error);
}

TEST(CircularBufferTest, ConstBackMethod) {
const CircularBuffer buffer(5, 'A');
EXPECT_EQ(buffer.back(), 'A');

const CircularBuffer emptyBuffer(5);
EXPECT_THROW(emptyBuffer.back(), std::logic_error);
}

TEST(CircularBufferTest, LinearizeBufferNotLinearized) {
CircularBuffer buffer(5);
buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');
buffer.pushBack('D');

buffer.popFront();
buffer.pushBack('E');

EXPECT_FALSE(buffer.isLinearized());

char* linearBuffer = buffer.linearize();
EXPECT_EQ(linearBuffer[0], 'B');
EXPECT_EQ(linearBuffer[1], 'C');
EXPECT_EQ(linearBuffer[2], 'D');
EXPECT_EQ(linearBuffer[3], 'E');
}

TEST(CircularBufferTest, LinearizeBufferAlreadyLinearized) {
CircularBuffer buffer(5);
buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

EXPECT_TRUE(buffer.isLinearized());

char* linearBuffer = buffer.linearize();
EXPECT_EQ(linearBuffer[0], 'A');
EXPECT_EQ(linearBuffer[1], 'B');
EXPECT_EQ(linearBuffer[2], 'C');
}

TEST(CircularBufferTest, IsLinearized) {
CircularBuffer buffer(5);
EXPECT_TRUE(buffer.isLinearized());
}

TEST(CircularBufferTest, ValidRotate) {
CircularBuffer buffer(5);
buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

buffer.rotate(1);
EXPECT_EQ(buffer[0], 'B');
EXPECT_EQ(buffer[1], 'C');
EXPECT_EQ(buffer[2], 'A');
}

TEST(CircularBufferTest, InvalidRotate) {
CircularBuffer bufferWithZeroCapacity;
EXPECT_THROW(bufferWithZeroCapacity.rotate(2), std::logic_error);

CircularBuffer buffer(5);
buffer.pushBack('A');
buffer.pushBack('B');

EXPECT_THROW(buffer.rotate(3), std::out_of_range);
}

TEST(CircularBufferTest, Size) {
CircularBuffer buffer(5);

EXPECT_EQ(buffer.size(), 0);

buffer.pushBack('A');
EXPECT_EQ(buffer.size(), 1);

buffer.popBack();
EXPECT_EQ(buffer.size(), 0);
}

TEST(CircularBufferTest, Empty) {
CircularBuffer buffer(5);

EXPECT_TRUE(buffer.empty());

buffer.pushBack('A');
EXPECT_FALSE(buffer.empty());

buffer.popBack();
EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, Full) {
CircularBuffer buffer(3);

EXPECT_FALSE(buffer.full());

buffer.pushBack('A');
EXPECT_FALSE(buffer.full());

buffer.pushBack('B');
EXPECT_FALSE(buffer.full());

buffer.pushBack('C');
EXPECT_TRUE(buffer.full());

buffer.popBack();
EXPECT_FALSE(buffer.full());
}

TEST(CircularBufferTest, Reserve) {
CircularBuffer buffer(5);

EXPECT_EQ(buffer.reserve(), 5);

buffer.pushBack('A');
EXPECT_EQ(buffer.reserve(), 4);

buffer.pushBack('B');
EXPECT_EQ(buffer.reserve(), 3);

buffer.pushBack('E');
EXPECT_EQ(buffer.reserve(), 2);

buffer.popBack();
EXPECT_EQ(buffer.reserve(), 3);
}

TEST(CircularBufferTest, Capacity) {
CircularBuffer buffer(5);
EXPECT_EQ(buffer.capacity(), 5);

buffer.pushBack('A');
buffer.pushBack('B');
EXPECT_EQ(buffer.capacity(), 5);

buffer.popBack();
EXPECT_EQ(buffer.capacity(), 5);
}

TEST(CircularBufferTest, SetCapacity) {
CircularBuffer buffer(5);

buffer.pushBack('A');
buffer.pushBack('B');

buffer.setCapacity(10);

EXPECT_EQ(buffer.reserve(), 8);
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[1], 'B');

EXPECT_THROW(buffer.setCapacity(1), std::invalid_argument);
}

TEST(CircularBufferTest, Resize) {
CircularBuffer buffer(5);

buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

buffer.resize(5, 'D');

EXPECT_EQ(buffer.size(), 5);
EXPECT_EQ(buffer[3], 'D');
EXPECT_EQ(buffer[4], 'D');

EXPECT_THROW(buffer.resize(11, 'D'), std::invalid_argument);
EXPECT_THROW(buffer.resize(2, 'D'), std::invalid_argument);

buffer.resize(5, 'D');
EXPECT_EQ(buffer.size(), 5);
EXPECT_EQ(buffer[3], 'D');
EXPECT_EQ(buffer[4], 'D');
}

TEST(CircularBufferTest, AssignmentOperator) {
CircularBuffer firstBuffer(5);
firstBuffer.pushBack('A');
firstBuffer.pushBack('B');

CircularBuffer SecondBuffer(5);
SecondBuffer = firstBuffer;

EXPECT_EQ(SecondBuffer.size(), 2);
EXPECT_EQ(SecondBuffer[0], 'A');
EXPECT_EQ(SecondBuffer[1], 'B');

EXPECT_NE(&firstBuffer, &SecondBuffer);


SecondBuffer = SecondBuffer;
EXPECT_EQ(&SecondBuffer, &SecondBuffer);
}

TEST(CircularBufferTest, Swap) {
CircularBuffer firstBuffer(5);
firstBuffer.pushBack('A');
firstBuffer.pushBack('B');

CircularBuffer secondBuffer(5);
secondBuffer.pushBack('X');
secondBuffer.pushBack('Y');
secondBuffer.pushBack('Z');

firstBuffer.swap(secondBuffer);

EXPECT_EQ(firstBuffer.size(), 3);
EXPECT_EQ(firstBuffer[0], 'X');
EXPECT_EQ(firstBuffer[1], 'Y');
EXPECT_EQ(firstBuffer[2], 'Z');

EXPECT_EQ(secondBuffer.size(), 2);
EXPECT_EQ(secondBuffer[0], 'A');
EXPECT_EQ(secondBuffer[1], 'B');

EXPECT_NE(&firstBuffer, &secondBuffer);
}

TEST(CircularBufferTest, PushBack) {
CircularBuffer buffer(3);

buffer.pushBack('A');
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer[0], 'A');

buffer.pushBack('B');
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[1], 'B');

buffer.pushBack('C');
EXPECT_EQ(buffer.size(), 3);
EXPECT_EQ(buffer[2], 'C');

EXPECT_THROW(buffer.pushBack('D'), std::overflow_error);
}

TEST(CircularBufferTest, PushFront) {
CircularBuffer buffer(3);

buffer.pushFront('A');
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer[0], 'A');

buffer.pushFront('B');
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[0], 'B');

buffer.pushFront('C');
EXPECT_EQ(buffer.size(), 3);
EXPECT_EQ(buffer[0], 'C');

EXPECT_THROW(buffer.pushFront('D'), std::overflow_error);
}

TEST(CircularBufferTest, PopBack) {
CircularBuffer buffer(3);

buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

buffer.popBack();
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[1], 'B');

buffer.popBack();
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer[0], 'A');

buffer.popBack();
EXPECT_EQ(buffer.size(), 0);

EXPECT_THROW(buffer.popBack(), std::underflow_error);
}

TEST(CircularBufferTest, PopFront) {
CircularBuffer buffer(3);

buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

buffer.popFront();
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[0], 'B');
EXPECT_EQ(buffer[1], 'C');

buffer.popFront();
EXPECT_EQ(buffer.size(), 1);
EXPECT_EQ(buffer[0], 'C');

buffer.popFront();
EXPECT_EQ(buffer.size(), 0);

EXPECT_THROW(buffer.popFront(), std::underflow_error);
}

TEST(CircularBufferTest, Insert) {
CircularBuffer buffer(7);

buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

buffer.insert(0, 'X');
EXPECT_EQ(buffer.size(), 4);
EXPECT_EQ(buffer[0], 'X');
EXPECT_EQ(buffer[1], 'A');
EXPECT_EQ(buffer[2], 'B');
EXPECT_EQ(buffer[3], 'C');


buffer.insert(1, 'Y');
EXPECT_EQ(buffer.size(), 5);
EXPECT_EQ(buffer[0], 'X');
EXPECT_EQ(buffer[1], 'Y');
EXPECT_EQ(buffer[2], 'A');
EXPECT_EQ(buffer[3], 'B');
EXPECT_EQ(buffer[4], 'C');

buffer.insert(5, 'Z');
EXPECT_EQ(buffer.size(), 6);
EXPECT_EQ(buffer[0], 'X');
EXPECT_EQ(buffer[1], 'Y');
EXPECT_EQ(buffer[2], 'A');
EXPECT_EQ(buffer[3], 'B');
EXPECT_EQ(buffer[4], 'C');
EXPECT_EQ(buffer[5], 'Z');

buffer.insert(4, 'Z');
EXPECT_EQ(buffer.size(), 7);
EXPECT_EQ(buffer[0], 'X');
EXPECT_EQ(buffer[1], 'Y');
EXPECT_EQ(buffer[2], 'A');
EXPECT_EQ(buffer[3], 'B');
EXPECT_EQ(buffer[4], 'Z');
EXPECT_EQ(buffer[5], 'C');
EXPECT_EQ(buffer[6], 'Z');

EXPECT_THROW(buffer.insert(-1, 'W'), std::out_of_range);
EXPECT_THROW(buffer.insert(8, 'W'), std::out_of_range);
EXPECT_THROW(buffer.insert(1, 'W'), std::overflow_error);
}

TEST(CircularBufferTest, Erase) {
CircularBuffer buffer(5);

buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');
buffer.pushBack('D');
buffer.pushBack('E');

buffer.erase(1, 3);
EXPECT_EQ(buffer.size(), 3);
EXPECT_EQ(buffer[0], 'A');
EXPECT_EQ(buffer[1], 'D');
EXPECT_EQ(buffer[2], 'E');

buffer.erase(0, 1);
EXPECT_EQ(buffer.size(), 2);
EXPECT_EQ(buffer[0], 'D');
EXPECT_EQ(buffer[1], 'E');

buffer.erase(0, 2);
EXPECT_TRUE(buffer.empty());

EXPECT_THROW(buffer.erase(0, 1), std::out_of_range);
buffer.pushBack('X');
EXPECT_THROW(buffer.erase(1, 1), std::invalid_argument);
}

TEST(CircularBufferTest, Clear) {
CircularBuffer buffer(5);
buffer.pushBack('A');
buffer.pushBack('B');
buffer.pushBack('C');

EXPECT_EQ(buffer.size(), 3);

buffer.clear();
EXPECT_TRUE(buffer.empty());
EXPECT_EQ(buffer.size(), 0);
EXPECT_EQ(buffer.capacity(), 5);
}

TEST(CircularBufferTest, EqualityOperator) {
CircularBuffer firstBuffer(5);
CircularBuffer secondBuffer(5);

firstBuffer.pushBack('A');
firstBuffer.pushBack('B');
firstBuffer.pushBack('C');

secondBuffer.pushBack('A');
secondBuffer.pushBack('B');
secondBuffer.pushBack('C');

EXPECT_TRUE(firstBuffer == secondBuffer);

secondBuffer.pushBack('D');
EXPECT_FALSE(firstBuffer == secondBuffer);

firstBuffer.pushBack('C');
EXPECT_FALSE(firstBuffer == secondBuffer);

CircularBuffer firstNewBuffer;
CircularBuffer secondNewBuffer;
EXPECT_TRUE(firstNewBuffer == secondNewBuffer);
}

TEST(CircularBufferTest, InequalityOperator) {
CircularBuffer buffer1(5);
CircularBuffer buffer2(5);

buffer1.pushBack('A');
buffer1.pushBack('B');
buffer1.pushBack('C');

buffer2.pushBack('A');
buffer2.pushBack('B');
buffer2.pushBack('C');

EXPECT_FALSE(buffer1 != buffer2);

buffer2.pushBack('D');
EXPECT_TRUE(buffer1 != buffer2);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}