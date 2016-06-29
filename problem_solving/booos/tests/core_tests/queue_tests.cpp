
#include "gtest/gtest.h"

#include "Queue.h"


class MyElement : public BOOOS::Queue::Element {
public:
    MyElement(std::string str) : _name(str) {}

    virtual ~MyElement() {}

    std::string & name() { return _name; }

private:
    std::string _name;
};

TEST(QueueTest, test_creation_destruction)
{
    BOOOS::Queue q;

    ASSERT_EQ(0, q.length());
    ASSERT_EQ(nullptr, q.head()->next());
    ASSERT_EQ(nullptr, q.head()->prev());
}

TEST(QueueTest, test_insertion)
{
    BOOOS::Queue q;

    MyElement * person1 = new MyElement("João");
    MyElement * person2 = new MyElement("Pedro");
    MyElement * person3 = new MyElement("Augusto");
    MyElement * person4 = new MyElement("Fábio");

    q.insert(person1);
    q.insert(person2);
    q.insert(person3);
    q.insert(person4);

    ASSERT_EQ(person1, q.head()->next()); // head
    ASSERT_EQ(person4, q.head()->prev()); // tail

    MyElement * tmp = dynamic_cast<MyElement*>(q.head()->next());
    ASSERT_EQ(tmp, person1);

    tmp = dynamic_cast<MyElement*>(tmp->next());
    ASSERT_EQ(tmp, person2);

    tmp = dynamic_cast<MyElement*>(tmp->next());
    ASSERT_EQ(tmp, person3);

    tmp = dynamic_cast<MyElement*>(tmp->next());
    ASSERT_EQ(tmp, person4);
}

TEST(QueueTest, test_remotion) {
    BOOOS::Queue q;

    MyElement * person1 = new MyElement("João");
    MyElement * person2 = new MyElement("Pedro");
    MyElement * person3 = new MyElement("Augusto");
    MyElement * person4 = new MyElement("Fábio");

    q.insert(person1);
    q.insert(person2);
    q.insert(person3);
    q.insert(person4);

    MyElement * removed_person = dynamic_cast<MyElement *>(q.remove());
    ASSERT_EQ(person1, removed_person);
    delete removed_person;

    ASSERT_EQ(q.head()->next(), person2);
    ASSERT_EQ(person2->prev(), q.head()->prev());
    ASSERT_EQ(q.head()->prev()->next(), person2);
}
