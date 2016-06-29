
#include "gtest/gtest.h"

#include "Queue.h"

TEST(QueueTest, creation_destruction)
{
    BOOOS::Queue q;
    ASSERT_EQ(0, q.length());
    ASSERT_EQ(nullptr, q.head()->next());
    ASSERT_EQ(nullptr, q.head()->prev());
}

TEST(QueueTest, insertion)
{
    auto person1 = new BOOOS::Queue::Element();
    auto person2 = new BOOOS::Queue::Element();
    auto person3 = new BOOOS::Queue::Element();
    auto person4 = new BOOOS::Queue::Element();

    BOOOS::Queue q;
    q.insert(person1);
    q.insert(person2);
    q.insert(person3);
    q.insert(person4);

    ASSERT_EQ(person1, q.head()->next()); // head
    ASSERT_EQ(person4, q.head()->prev()); // tail

    auto tmp = q.head()->next();
    ASSERT_EQ(tmp, person1);

    tmp = tmp->next();
    ASSERT_EQ(tmp, person2);

    tmp = tmp->next();
    ASSERT_EQ(tmp, person3);

    tmp = tmp->next();
    ASSERT_EQ(tmp, person4);
}

TEST(QueueTest, remotion)
{
    auto person1 = new BOOOS::Queue::Element();
    auto person2 = new BOOOS::Queue::Element();
    auto person3 = new BOOOS::Queue::Element();
    auto person4 = new BOOOS::Queue::Element();

    BOOOS::Queue q;
    q.insert(person1);
    q.insert(person2);
    q.insert(person3);
    q.insert(person4);

    auto removed_person = q.remove();
    ASSERT_EQ(person1, removed_person);
    delete removed_person;

    ASSERT_EQ(q.head()->next(), person2);
    ASSERT_EQ(person2->prev(), q.head()->prev());
    ASSERT_EQ(q.head()->prev()->next(), person2);
}
