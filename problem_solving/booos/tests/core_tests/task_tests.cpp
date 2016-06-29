/*
 * Queue_Test.cc
 *
 */

#include <gtest/gtest.h>

#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "Task.h"


class TaskTest : public testing::Test
{
protected:
    void SetUp() override
    {
        BOOOS::Task::init();
    }
};

TEST_F(TaskTest, init)
{
    ASSERT_EQ(0, BOOOS::Task::self()->tid());
    ASSERT_EQ(BOOOS::Task::RUNNING, BOOOS::Task::self()->state());

    BOOOS::Task::init(); // should execute only once!
    ASSERT_EQ(0, BOOOS::Task::self()->tid());
    ASSERT_EQ(BOOOS::Task::RUNNING, BOOOS::Task::self()->state());
}

void f1(void*) {}
void f2(void*) {}
void f3(void*) {}
void f4(void*) {}

TEST_F(TaskTest, creation_destruction)
{
    BOOOS::Task t1(f1, 1, (void*)"f1"), t2(f2, 1, (void*)"f2");
    auto t3 = std::unique_ptr<BOOOS::Task>(new BOOOS::Task(f3, 1, (void*)"f3"));
    auto t4 = std::unique_ptr<BOOOS::Task>(new BOOOS::Task(f4, 1, (void*)"f4"));

    ASSERT_EQ(1, t1.tid());
    ASSERT_EQ(BOOOS::Task::READY, t1.state());
    ASSERT_EQ(2, t2.tid());
    ASSERT_EQ(BOOOS::Task::READY, t2.state());
    ASSERT_EQ(3, t3->tid());
    ASSERT_EQ(BOOOS::Task::READY, t3->state());
    ASSERT_EQ(4, t4->tid());
    ASSERT_EQ(BOOOS::Task::READY, t4->state());
}

/**************************************************/
/*          Globals for cooperative test          */
/**************************************************/
BOOOS::Task *ping, *pong;
std::queue<std::string> log, correct;

void f_ping(void * arg) {
    int i ;
    std::stringstream str1;
    str1 << (char *) arg << " Start\n";
    log.push(str1.str());
    str1.str().clear();

    for (i=0; i<4; i++)
    {
        std::stringstream str;
        str << "\t" << (char*) arg << " " << i << "\n";
        log.push(str.str());
        ping->pass_to(pong);
    }
    std::stringstream str2;
    str2 << (char *) arg << " End" << "\n";
    log.push(str2.str());

    ping->exit(0);
}

void f_pong(void * arg) {
    int i ;

    std::stringstream str1;
    str1 << (char *) arg << " Start\n";
    log.push(str1.str());
    str1.str().clear();

    for (i=0; i<4; i++)
    {
        std::stringstream str;
        str << "\t" << (char*) arg << " " << i << "\n";
        log.push(str.str());
        str.str().clear();
        pong->pass_to(ping);
    }
    std::stringstream str2;
    str2 << (char *) arg << " End" << "\n";
    log.push(str2.str());
    str2.str().clear();

    pong->exit(0);
}

TEST_F(TaskTest, DISABLED_cooperative_execution)
{
    correct.push("Main Start\n");
    correct.push("\tPing Start\n");
    correct.push("\t\tPing 0\n");
    correct.push("\tPong Start\n");
    correct.push("\t\tPong 0\n");
    correct.push("\t\tPing 1\n");
    correct.push("\t\tPong 1\n");
    correct.push("\t\tPing 2\n");
    correct.push("\t\tPong 2\n");
    correct.push("\t\tPing 3\n");
    correct.push("\t\tPong 3\n");
    correct.push("\tPing End\n");
    correct.push("\tPong End\n");
    correct.push("Main End\n");

    log.push("Main Start\n");

    ping = new BOOOS::Task(f_ping, 1, (char*)"\tPing");
    pong = new BOOOS::Task(f_pong, 1, (char*)"\tPong");

    BOOOS::Task::self()->pass_to(ping);
    BOOOS::Task::self()->pass_to(pong);

    log.push("Main End\n");

    ASSERT_EQ(correct.size(), log.size());

    std::string expected, current;
    while(log.size()) {
        current = log.front();
        log.pop();
        expected = correct.front();
        correct.pop();
        ASSERT_EQ(expected, current);
    }

    delete ping;
    delete pong;
}
