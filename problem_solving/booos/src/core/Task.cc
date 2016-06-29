/*
 * Task.cc
 *
 */

#include <cassert>

#include "Task.h"

namespace BOOOS
{
int Task:: __tid_counter;
volatile Task * Task:: __running;
Task * Task:: __main;
/*!
 * The private Constructor of the task, initialize the task ID and the context
 */
Task::Task() :
    _tid(Task::__tid_counter)
{
    assert(!getcontext(&_context) && "Error to get current user context in Private Constructor.");
}

/*!
 * Constructor of the task, initialize the task with state in READY,
 * and others arguments of the class, such as, stack with array char
 * representing the stack
 */
Task::Task(void (*entry_point)(void *), int nargs, void *arg) :
    _state(READY),
    _tid(Task::__tid_counter++)
{
    _stack = new char[STACK_SIZE];

    /*!
     * The getcontext() function initialises the structure pointed to by _context
     * to the current user context of the calling thread. The ucontext_t type
     * that ucp points to defines the user context and includes the contents of
     * the calling thread's machine registers, the signal mask, and the current
     * execution stack.
     */
    assert(!getcontext(&_context) && "Error to get current user context.");
    /*!
     * pass arguments to use to storage in _context
     * To establish a new alternate signal stack, uc_stack.ss_flags is set to zero
     * uc_link, pointer to the context that will be resumed when this context returns
     */
    _context.uc_stack.ss_sp = _stack;
    _context.uc_stack.ss_size = STACK_SIZE;
    _context.uc_stack.ss_flags = 0;
    _context.uc_link = nullptr;
    /*!
     * The makecontext() function modifies the context specified by _context,
     * which has been initialised using getcontext()
     */
    makecontext(&_context, (void (*)())entry_point, nargs, arg);
}

Task::~Task()
{
    /*!
     * if not finishing this task, call task::exit to finish and
     * pass context to main task.
     *
     * After free the resources
     */
    if (_state != FINISHING)
        this->exit(0);
    delete [] _stack;
}

void Task::exit(int code)
{
    /*!
     * Finish this task and return the execution for main task
     */
    pass_to(Task::__main, FINISHING);
}

void Task::pass_to(Task *t, State s)
{
    /*!
     * The swapcontext() function saves the current context (this) in the context
     * structure pointed to by _context and sets the context to the context
     * structure pointed to by t->_context.
     */
    swapcontext(&_context, &t->_context);
    /*!
     * set t as current task
     */
    Task::__running = t;
    /*!
     * set new state in task (this)
     */
    _state = s;
}

/*!
 * Static method
 */
void Task::init()
{
    /*!
     * Initialize the first task as RUNNING
     */
    Task::__tid_counter = 0;
    Task::__main = new Task();
    __main->_state = RUNNING;
    Task::__running = Task::__main;
    ++Task::__tid_counter;
}

} /* namespace BOOOS */
