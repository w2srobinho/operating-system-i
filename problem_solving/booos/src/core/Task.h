/*
 * Task.h
 *
 */

#ifndef TASK_H_
#define TASK_H_

#include <ucontext.h>

#include "Queue.h"

namespace BOOOS {

const int STACK_SIZE = 32768;    ///< stack size

class Task : public Queue::Element {
public:
    enum State {
        READY,
        WAITING,
        RUNNING,
        FINISHING
    };

    Task(void (*entry_point)(void *), int nargs, void *arg);

    virtual ~Task();

    int tid() { return _tid; }

    State state() { return _state; }

    void pass_to(Task *t, State s = READY);

    void exit(int code);

    static Task *self() { return (Task *) __running; }

    static void init();

private:
    Task();

    /*!
     * static arguments
     */
    static int __tid_counter;           ///< task ID counter
    static volatile Task* __running;    ///< task already running
    static volatile Task* __main;       ///< main task
    /*!
     * created by reference the UML on exercise,
     * but isn't necessary in this block
     */
    static Queue __ready;

    State _state;           ///< state of task @see \enum State
    int _tid;               ///< task ID
    ucontext_t _context;    ///< user context
    char* _stack;           ///< stack is needed for pass to context
};

} /* namespace BOOOS */

#endif /* TASK_H_ */
