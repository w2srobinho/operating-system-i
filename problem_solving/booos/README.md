## Recommended for 14/06: \[Problem Solving\] (SP3) Solve problem based on the unit "Processes and Threads"

**Due date:** terça, 28 junho 2016, 11:55

**Requested files:** `Process.h`, `Queue.h`, `Scheduler.h`, `Scheduling_Queue.h`, `Thread.h`, `Process.cpp`, `Queue.cpp`, `Scheduler.cpp`, `Scheduling_Queue.cpp`, `Thread.cpp`, `TimerMediator.cpp`, `Traits.h`
**Maximum upload file size:** 1 MiB
**Type of work:** Group work SP3 - Agrupamento 1: Grupo Único.
**Smallest Remaining Time First**

##### Esta é a avaliação de 'Solução de Problemas 5" (SP5).

Ela é uma avaliação extra classe em grupos (mesmo agrupamento) realizada em algumas horas (~2 a ~10) e corresponde basicamente a um exercício de programação encontrado na bibliografia, ao final de alguns capítulos. Ala avalia as "habilidades" dos acadêmicos na resolução de problemas práticos envolvendo os conhecimentos teóricos estudados, e também suas "habilidades" em projetar, modelar, simular, implementar e avaliar os principais componentes de um sistema operacional. Das cerca de 4 soluções de problemas realizadas, será desconsiderada aquela com menor nota.

A Solução de Problemas deve ser realziada exclusivamente através do editor de programas do Moodle, em grupos, e é avaliada conforme os seguintes critérios:
* (i) corretude e funcionamento do programa implementado, que deve seguir rigorosamente a descrição fornecida, resolvendo o problema proposto sem travamentos ou comportamentos inesperados;
* (ii) o projeto e modelagem do código orientado a objetos e as estruturas de dados utilizados;
* (iii) a documentação do sistema, conforme doxigen, realizada ao longo do código,
* (iv)  a capacidade de cooperar e trabalhar em grupo e a utilização de práticas de estudo independente e atitudes profissionais e éticas. Também será observada a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero. O restante deste texto descreve o que deve ser feito nesta Atividade Prática.

Para esta atividade prática você deve implementar um gerenciadorde processos e threads.

Processos chegarão ao sistema quando for invocada a chamada `static Process* Process::exec()`, que precisa ser implementada. A implementação dessa chamada de sistema deve criar um **Process** (assuma que o pai de todos os processos tem `id=0`), alocar memória para ele invocando a chamada de sistema `Partition* MemoryManager::allocateMemory(unsigned int size)`, inicializar os novos atributos que porventura você tenha criado, colocá-lo na lista de processos no sistema e criar uma thread (correspondente a "`main`") para esse processo invocando a chamada `static Thread* Thread::thread_create(Process* parent)`. O método retorna o processo criado.

Processos serão finalizados quando for invocada a chamada `static void Process::exit(int status = 0)`, que precisa ser implementada. A implementação dessa chamada deve desalocar a memória do processo que extá executando invocando `void MemoryManager::deallocateMemory(Partition* partition)`, excluir todas as threads desse processo, excluir o processo (destruir o objeto **Process**), invocar o escalonador para escolher outra thread, invocando `Thread* Scheduler::choose()` e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando `void Thread::dispatch(Thread* previous, Thread* next)`.

Threads criarão novas threads quando for invocada a chamada `static Thread* Thread::thread_create(Process* parent)`, que precisa ser implementada. A implementação dessa chamada deve criar um objeto **Thread**, inicializar os novos atributos que porventura você tenha criado, colocá-la na lista de threads no sistema, e se o escalocador for preemptivo então deve chamar o escalonador para escolher outra thread, invocando `Thread* Scheduler::choose()`` e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando `static void Thread::dispatch(Thread* previous, Thread* next)`. O método retorna a thread criada.

Threads aguardarão pelo término de outras threads quando for invocada a chamada `void Thread::join()`, que precisa ser implementada. A implementação dessa chamada deve verificar se a thread **this** passada como parâmetro está no estado **FINISHING** ou não. Se estiver, então a thread que invocou a chamada (a que está executando **--running--**) simplesmente continua sua execução. Se não estiver, então a thread executando deve ser colocada no estado **WAITING** e colocada na fila de threads esperando pela thread **this**. Ainda nesse caso, deve-se chamar o escalonador para escolher outra thread, invocando `Thread* Scheduler::choose()` e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando `static void Thread::dispatch(Thread* previous, Thread* next)`. Para melhor ilustrar essa chamada, se o código da thread **T1** tiver a chamada `T2->join()`, então **T1** é quem está executando (running) e **T2** é quem foi invocada (**this**), e é **T1** que deve ser bloqueada esperando por **T2** (se **T2** não estiver **FINISHING**).

Threads serão finalizadas quando for invocada a chamada `void Thread::exit()`, que precisa ser implementada. A implementação dessa chamada deve colocar a thread que está executando no estado **FINISHING**, verificar se há alguma thread na lista de threads bloqueadas esperando por essa thread. Se houver, todas as threads na lista devem ser colocadas no estado **READY** e colocadas na fila de threads pronas para executar. Em qualquer caso, deve-se ainda chamar o escalonador para escolher outra thread, invocando `Thread* Scheduler::choose()` e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando `static void Thread::dispatch(Thread* previous, Thread* next)`.

Threads podem decidir deixar a CPU invocando a chamada void Thread::yield(), que precisa ser implementada. A implementação dessa chamada deve colocar a thread que está executando no estado **READY**, incluí-la na fila de threads prontas, chamar o escalonador para escolher outra thread, invocando `Thread* Scheduler::choose()` e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando `static void Thread::dispatch(Thread* previous, Thread* next)`.

Threads são despachadas, ou seja, têm seus contextos trocados, quando se invoca a chamada `static void Thread::dispatch(Thread* previous, Thread* next)`, que já está implementada. A implementaçao desse  método deve inicialmente verificar se a próxima thread (**next**) é nula ou não. Se for, nada precisa ser feito (isso só ocorre quando a fila de prontos é vazia e não há thread para ser escalonada). Se não for, então o atributo _running deve ser atualizado e  a thread a ser executada precisa ser colocada no estado **RUNNING** e retirada da fila de prontos. Então deve ser verificado se a thread anterior (**previous**) é diferente de nula e também se é diferente da próxma thread. Se não for, então basta restaurar o contexto da próxima thread, invocando `static void CPU::restore_context(Thread* next)`. Se for, então é preciso verificar se a thread anterior estava no estado **RUNNING** e caso sim, então a thread anterior deve passar para o estado **READY** e ser colocada na fila de threads prontas. Após esse teste deve-se fazer a troca de contexto entre as threads, invocando o método `static void CPU::switch_context(Thread* previous, Thread* next)`.

Threads podem solicitar ao escalonador para escolher outra thread para executar, invocando o método `Thread* Scheduler::choose()`, que deve ser implementado. A implementação desse método deve verificar se há alguma thread na fila de threads prontas para executar. Se houver, deve retornar o primeiro elemento dessa fila, **SEM O EXCLUIR** da fila. Se a fila estiver vazia, deve retornar nulo. A fila de escalonamento é a `Scheduling_Queue`, e ela deve ser ordenada pelo atributo `_prioriry` da Thread, independente do algoritmo de escalonamento, ou seja, o escalonador pode usar qualquer forma de ordenamento, mas o resultado do ordenamento deve estar no atributo `_priority`, e a fila `Scheduling_Queue` deve ser ordenada de modo que Threads com menor valor numérico de `_priority` estejam antes na fila.

Quando o algoritmo de escalonamento usado for de prioridade dinâmica ou então quando algum mecanismo do tipo `aging` for usado, pode ser necessário alterar algum atributo das threads que estão na fila de prontos e reordenar essa fila conforme os novos valores dos atributos. Nesses casos, isso pode ser feito quando a fila de threads prontas for alterada, invocando o método `void Scheculer::reschedule()`, que precisa ser implementado. Esse método precisa varrer a fila de pronto ajustando os atributos necessários das threads na fila, possivelmente tendo que removê-las da fila e depois reinseri-las para que o ordenamento ocorra, o que depende da estrutura de dados da fila sendo usada.

O sistema simulado possui ainda um **timer**, que gera interrupções periodicamente. O timer foi inicialmente configurado para gerar interrupções a cada 100 unidades de tempo (ver arquivo de configurações `Traits.h`). Sempre que uma interrupção do timer ocorre automaticamente é invocado o método `static void TimerMediator::interrupt_handler()`, que precisa ser implementado. Esse método trata uma interrupção do timer, e sua implementação depende do algoritmo de escalonamento. Ele pode ser usado para preemptar uma thread caso seu `time slice` tenha sido atingido, no caso do algoritmo `round-robin`, por exemplo.

As classes **Scheduler** e **MemoryManager** podem ser obtidas usando os métodos `static Scheduler* System::scheduler()`  e `static MemoryManager* System::memoryManager()`.

Dependendo do algoritmo de escalonamento a ser implementado, pode ser necessário obter o tempo atual. Isso pode ser útil para salvar informações sobre quando uma thread foi escalonada e calcular quanto tempo tempo ela usou a CPU, por exemplo. Para obter o tempo atual, use o método `double Simulator::getInstance()->getTnow()`.

Algumas configurações de depuração, de overhead, do timer e do modelo de simulação em si podem ser ajustados no arquivo `Traits.h`. Sua implementação deveria funcionar para diferentes configurações.

O restante do funcionamento do sistema, que simula um gerenciamento de processos e threads, é feito por classes que não são visíveis ao aluno, mas o efeito final da simulação pode ser visto pelo aluno ao pressionar o botão "Run", que comanda a execução do sistema.

Ao executar o sistema, o console deve mostrar uma saída parecida com a abaixo.

```
Simulation is initing.
Simulation is starting.
[0] <Info> Entity 0, InvokeProcessExec: Process has arrived. Invoking Process::exec()
        Process::exec()
[0] <Info> Entity 0, WaitForProcessToEnd: Entity is waiting for the signal 'Proces_Exit' to be sent
[200] <Info> Entity 1, InvokeProcessExec: Timer interrupt handler invoked
        TimerMediator::interrupt_handler()
```

Depois de iniciar, os logs seguem o formato `[<tempo atual] <<debug level>> Entity <número da entidade>, <módulo de simulação atual>, <mensagem de depuração>`

Os traces também incluem as chamadas de sistema (ex: Process::exec() no mol acima) e demais métodos (ex: `TimerMediator::interrupt_handler()` no log acima) que são invocados. Você pode alterar o nível de depuração desejado ajustando no arquivo `Traits.h` que níveis de depuração deseja visualizar.

Se você quiser apresentar alguma mensagem na tela, recomenda-se que você use a classe **Debug** e o trace level **Trace**.

**Se houver algum problema com a execução do código-fonte dispobilizado, problemas ou dúvidas sobre o o funcionamento do sistema entre em contato com o professor com a maior antecedência possível. Sugere-se que se use o fórum de discussão reservado para isso.**

## Requested files

### Process.h

```c++
/*
 * File:   Process.h
 * Author: <preencher>
 *
 * Created on August 31, 2015, 7:49 AM
 */
 
#ifndef PROCESS_H
#define   PROCESS_H

#include <list>
#include <queue>

#include "MemoryManager.h"
#include "MemorySegment.h"
#include "Entity.h"
#include "ModuleInvoke_ProcessExec.h"

class Process {
    friend class Thread;
    friend class ModuleInvoke_ProcessExec; // for simulation purposes only
    friend class Tester; // for simulation purposes only



    // structs for grouping process' attributes

    struct IdentificationInformation {
        unsigned int _processId;
        unsigned int _parentId;
        int _priority;
    };

    struct MemoryInformation {
        Partition* _partition;
        std::list<MemorySegment*>* _segments;
    };


public: // constrs
    Process(unsigned int parentId);
    Process(const Process& orig);
    virtual ~Process();
public: // id info
    unsigned int getParentId() const;
    unsigned int getId() const;
    Entity* getEntity() const;
public: // statics

    static unsigned int getNewId() {
        static unsigned int _lastId = 0;
        return ++_lastId;
    }

    static std::list<Process*>* getProcessesList() { // return the listr of all processes in the system
        static std::list<Process*>* _ProcessesList;
        if (_ProcessesList == nullptr) {
            _ProcessesList = new std::list<Process*>();
        }
        return _ProcessesList;
    }

    static Process* exec(); // execute a new process
    static void exit(int status = 0); // finishes the running process
private:
    IdentificationInformation _idInfo;
    MemoryInformation _memInfo;
    
public: // For simulation use only. Do not change

    Entity* getEntity() {
        return _entity;
    }

private: // for simulator use only. Do not change
    Entity* _entity; //used only by the simulator
public: // USER DEFINED
    // INSERT YOUR CODE HERE

private: // USER DEFINED
    // INSERT YOUR CODE HERE
};

#endif    /* PROCESS_H */
```

### Scheduler.h

```c++
/*
 * File:   Scheduler.h
 * Author: <PREENCHER>
 *
 * Created on September 27, 2015, 11:19 AM
 */

#ifndef SCHEDULER_H
#define   SCHEDULER_H

#include "Thread.h"
#include "Scheduling_Queue.h"

#include <queue>

class Scheduler {
public:
    Scheduler();

    Scheduler(const Scheduler& orig) {

    }

    virtual ~Scheduler() {

    }
private:
    Scheduling_Queue* _readyQueue;
    Thread* _choosen;
public:
    static const bool preemptive = true;
public:
    Thread* choose();  // choose a new thread to run
    Thread* choosen(); // returns the last chosen thread (syntax error: the method should be called chosen)
    void reschedule();
    void insert(Thread* thread);
    Thread* remove(Thread* thread);
};

#endif    /* SCHEDULER_H */
```

### Scheduling_Queue.h

```c++
/*
 * File:   Scheduling_Queue.h
 * Author: cacian
 *
 * Created on October 9, 2015, 9:34 PM
 */

#ifndef SCHEDULING_QUEUE_H
#define   SCHEDULING_QUEUE_H

#include <queue>

#include "Thread.h"

/**
 * Used by Scheduler to store Threads in the RUNNING state. This is the class for the READY_QUEUE
 */
class Scheduling_Queue /* insert hierarchy if you want */ {
public:
    Scheduling_Queue();
    Scheduling_Queue(const Scheduling_Queue& orig);
    virtual ~Scheduling_Queue();
public:
    bool empty();
    void insert(Thread* t);
    void remove(Thread* t);
    void pop(); // remove the top (head) element
    Thread* top(); // returns the top (head) element
private:
    // INSERT YOUR CODE HERE
    // ...
    std::list<Thread*>* _queue; /* change or delete if you want. It is just a suggestion */
};

#endif    /* SCHEDULING_QUEUE_H */
```

### Thread.h

```c++
/*
 * File:   Thread.h
 * Author: <preencher>
 *
 * Created on September 27, 2015, 10:30 AM
 */

#ifndef THREAD_H
#define   THREAD_H

#include "Queue.h"
#include "Process.h"
#include "TimerMediator.h"
#include "ModuleInvoke_ThreadExec.h"

class Thread {
    friend class ModuleInvoke_ThreadExec; // simulator use only
    friend class TimerMediator;
    friend class Process;
    friend class Tester; // for simulation purposes only

    enum State {
        RUNNING,
        READY,
        WAITING,
        FINISHING
    };

    enum Criterion {
        HIGH,
        NORMAL,
        LOW,
        MAIN,
        IDLE
    };

    /**
     *  struct for grouping threads' attributes
     **/
    struct AccountInformation {
        // do not change
        int _priority;  // this attribute MUST BE the one used to sort the Schelung_Queue, no matter the scheduler algorithm
        double _arrivalTime;
        double _cpuBurstTime;
        double _cpuTime;
        double _waitingTime;
        // add here another information you need
        // INSERT YOUR CODE HERE
        // ...
    };

public: // constructors
    Thread(Process* task, Thread::State state = Thread::State::READY);
    Thread(const Thread& orig);
    virtual ~Thread();
public: // statics
    static void sleep(Queue<Thread*> * q);
    static void wakeup(Queue<Thread*> * q);
    static Thread* thread_create(Process* parent);

    static std::list<Thread*>* getThreadsList() {
        static std::list<Thread*>* _threadsList;
        if (_threadsList == nullptr) {
            _threadsList = new std::list<Thread*>();
        }
        return _threadsList;
    }
protected: // statics
    static Thread* _running;
    static void yield();
    static void exit(int status = 0);
    static Thread* running();
    static void dispatch(Thread* previous, Thread* next);

    static unsigned int getNewId() {
        static unsigned int _lastId = 0;
        return ++_lastId;
    }
public: //
    int join();
    Process* getProcess();
    int getPriority() const;
protected:
    Process * _process;
    State _state;
    unsigned int _id;
    AccountInformation _accountInfo;
    Queue<Thread*>* _queue;

public: // USER DEFINED
    // INSERT YOUR CODE HERE
    // ...
private: // USER DEFINED
    // INSERT YOUR CODE HERE
    // ...
};

#endif    /* THREAD_H */
```

### Process.cpp

```c++
/*
 * File:   Process.cpp
 * Author: <preencher>
 *
 * Created on August 31, 2015, 7:49 AM
 */

#include "Debug.h"
#include "Process.h"
#include "Thread.h"
#include "System.h"
#include "Simulator.h"

Process::Process(unsigned int parentId) {
    this->_entity = Simulator::getInstance()->getEntity(); // simulation purposes only
    this->_entity->setPointer(this); // simulation purposes only
    
    this->_idInfo._processId = Process::getNewId();
    this->_idInfo._parentId = parentId;
    this->_idInfo._priority = Simulator::generate_uniform_distribution(0, 10);
    // random memory info about the new process
    this->_memInfo._segments = new std::list<MemorySegment*>();
    unsigned int numSegments = Simulator::generate_uniform_distribution(2, 5);
    for (unsigned int i = 0; i < numSegments; i++) {
        MemorySegment* segment = new MemorySegment(Simulator::generate_uniform_distribution(250 / (numSegments + 3), 250 / numSegments));
        _memInfo._segments->insert(_memInfo._segments->begin(), segment);
    }    
    // INSERT YOUR CODE HERE
    // ...
}
Process::Process(const Process& orig) {
}

Process::~Process() {
}

unsigned int Process::getParentId() const {
    return _idInfo._parentId;
}

unsigned int Process::getId() const {
    return _idInfo._processId;
}

/**
 * Processos chegarão ao sistema quando for invocada a chamada static Process* Process::exec(), que precisa
 * ser implementada. A implementação dessa chamada de sistema deve criar um Process, alocar memória para ele invocando
 * a chamada de sistema Partition* MemoryManagement::alloc(Process* process), inicializar seus atributos, colocá-lo na lista
 * de processos no sistema e criar uma thread (correspondente a "main") para esse processo invocando a chamada
 * static Thread* Thread::thread_create(Process* parent). O metodo deve retornar o processo criado.
 **/
Process* Process::exec() { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exec()");
    // INSERT YOUR CODE HERE (just complete what is missing... easy this time...)

    // criar um Process
    // Process* newProcess = ...

    // alocar memória para ele
    //... = System::memoryManager()-> ...
    //newProcess->_memInfo->_partition = ...

    // inicializar seus atributos
    //

    // colocá-lo na lista de processos no sistema
    //Process::getProcessesList()-> ...

    // criar uma thread (correspondente a "main") para esse processo
    // ...
    
    // retornar o método criado
    // return newProcess;
}

/**
 * Processos serão finalizados quando for invocada a chamada static void Process::exit(int status = 0), que precisa ser
 * implementada. A implementação dessa chamada deve desalocar a memória do processo invocando
 * void MemoryManager::deallocateMemory(Partition* partition), excluir todas as threads desse processo, excluir o processo (destruir
 * o objeto Process), invocar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() e então
 * o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando
 * void Dispatcher::dispatch(Thread* previous, Thread* next).
 **/
void Process::exit(int status) { /*static*/
    Debug::cout(Debug::Level::trace, "Process::exit(" + std::to_string(status) + ")");
    // INSERT YOUR CODE HERE (Now it's up to you...)
    // ...
}
```

### Queue.cpp

```c++
/*
 * File:   Queue.cpp
 * Author: <preencher>
 *
 * Created on September 27, 2015, 11:28 AM
 */

#include "Queue.h"

template <class T> Queue<T>::Queue() : std::list<T>() {
}

template <class T> Queue<T>::Queue(const Queue& orig) : std::list<T>(orig) {
}

template <class T> Queue<T>::~Queue() {
}

// INSERT YOUR CODE HERE
// ...
```

### Scheduler.cpp

```c++
/*
 * File:   Scheduler.cpp
 * Author: <preencher>
 *
 * Created on September 27, 2015, 11:19 AM
 */

#include "Debug.h"
#include "Scheduler.h"

Scheduler::Scheduler() {
    _readyQueue = new Scheduling_Queue();
}

Thread* Scheduler::choosen() {
    return _choosen;
}

void Scheduler::insert(Thread* thread) {
    this->_readyQueue->insert(thread);
}

Thread* Scheduler::remove(Thread* thread) {
    // remove a specific thread from the readyQueue
    this->_readyQueue->remove(thread);
}

/*
 * Threads podem solicitar ao escalonador para escolher outra thread para executar, invocando o método
 * Thread* Scheduler::choose(), que deve ser implementado. A implementação desse método deve verificar se há alguma
 * thread na fila de threads prontas para executar. Se houver, deve retornar o primeiro elemento dessa fila, sem o excluir
 * da fila. Se a fila estiver vazia, deve retornar nulo.
 */
Thread* Scheduler::choose() {
    Debug::cout(Debug::Level::trace, "Scheduler::choose()");
    // INSERT YOUR CODE HERE
    // ...
}

/*
 * Quando o algoritmo de escalonamento usado for de prioridade dinâmica ou então quando algum mecanismo do tipo aging
 * for usado, pode ser necessário alterar algum atributo das threads que estão na fila de prontos e reordenar essa fila
 * conforme os novos valores dos atributos. Nesses casos, isso pode ser feito quando a fila de threads prontas for alterada,
 * invocando o método void Scheculer::reschedule(), que precisa ser implementado. Esse método precisa varrer a fila
 * de pronto ajustando os atributos necessários das threads na fila, possivelmente tendo que removê-las da fila e depois
 * reinseri-las para que o ordenamento ocorra, o que depende da estrutura de dados da fila sendo usada.
 */
void Scheduler::reschedule() {
    Debug::cout(Debug::Level::trace, "Scheduler::reschedule()");
    if (!this->_readyQueue->empty()) {
        // INSERT YOUR CODE HERE
        // ...

    }
}
```

### Scheduling_Queue.cpp

```c++
/*
 * File:   Scheduling_Queue.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 9:34 PM
 */

#include "Scheduling_Queue.h"

Scheduling_Queue::Scheduling_Queue() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    _queue = new std::list<Thread*>();
}

Scheduling_Queue::Scheduling_Queue(const Scheduling_Queue& orig) {
}

Scheduling_Queue::~Scheduling_Queue() {
}

bool Scheduling_Queue::empty() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    return _queue->empty();
}

void Scheduling_Queue::pop() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    if (!_queue->empty()) {
        _queue->erase(_queue->begin());
    }
}

void Scheduling_Queue::remove(Thread* t) {
    _queue->remove(t);
}

Thread* Scheduling_Queue::top() {
    // CHANGE AS NECESSARY IF YOU CHOOSE ANOTHER QUEUE
    return (*_queue->begin());
}

void Scheduling_Queue::insert(Thread* t) {
    // INSERT YOUR CODE HERE
    // ...
}
```

### Thread.cpp

```c++
/*
 * File:   Thread.cpp
 * Author: <preencher>
 *
 * Created on September 27, 2015, 10:30 AM
 */

#include "Thread.h"
#include "Debug.h"
#include "System.h"
#include "Simulator.h"
#include "CPU.h"

Thread* Thread::_running;

Thread::Thread(Process* task, Thread::State state) {
    this->_id = Thread::getNewId();
    this->_process = task;
    this->_state = state;
    // INSERT YOUR CODE HERE
    // ...
}

Thread::Thread(const Thread& orig) {
}

Thread::~Thread() {
}

Thread* Thread::running() {
    return _running;
    //return System::scheduler()->choosen();
}

Process* Thread::getProcess() {
    return _process;
}

int Thread::getPriority() const {
    return _accountInfo._priority;
}

void Thread::sleep(Queue * q) {
    Debug::cout(Debug::Level::trace, "Thread::sleep(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

void Thread::wakeup(Queue * q) {
    Debug::cout(Debug::Level::trace, "Thread::wakeup(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads podem decidir deixar a CPU invocando a chamada void Thread::yield(), que precisa ser implementada. A implementação
 * dessa chamada deve colocar a thread que está executando no estado READY, incluí-la na fila de threads prontas, chamar
 * o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() e então o despachador para iniciar
 * a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
void Thread::yield() {
    Debug::cout(Debug::Level::trace, "Thread::yield()");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads aguardarão pelo término de outras threads quando for invocada a chamada void Thread::join(), que precisa ser 
 * implementada. A implementação dessa chamada deve verificar se a thread this passada como parâmetro está no estado FINISHING 
 * ou não. Se estiver, então a thread que invocou a chamada (a que está executando --running--) simplesmente continua sua execução. 
 * Se não estiver, então a thread executando deve ser colocada no estado WAITING e colocada na fila de threads esperando pela 
 * thread this. Ainda nesse caso, deve-se chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() 
 * e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando 
 * static void Thread::dispatch(Thread* previous, Thread* next). Para melhor ilustrar essa chamada, se o código da thread T1 
 * tiver a chamada T2->join(), então T1 é quem está executando (running) e T2 é quem foi invocada (this), e é T1 que deve ser 
 * bloqueada esperando por T2 (se T2 não estiver FINISHING).
 **/
int Thread::join() {
    Debug::cout(Debug::Level::trace, "Thread::join()");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads serão finalizadas quando for invocada a chamada void Thread::exit(), que precisa ser implementada. A implementação
 * dessa chamada deve colocar a thread que está executando no estado FINISHED, verificar se há alguma thread na lista
 * de threads bloqueadas esperando por essa thread. Se houver, todas as threads na lista devem ser colocadas no estado
 * READY e colocadas na fila de threads pronas para executar. Em qualquer caso, deve-se ainda chamar o escalonador para
 * escolher outra thread, invocando Thread* Scheduler::choose() e então o despachador para iniciar a execução da thread
 * escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
void Thread::exit(int status) {
    Debug::cout(Debug::Level::trace, "Thread::exit(" + std::to_string(status) + ")");
    // INSERT YOUR CODE HERE
    // ...
}

/**
 * Threads são despachadas, ou seja, têm seus contextos trocados, quando se invoca a chamada 
 * static void Thread::dispatch(Thread* previous, Thread* next), que precisa ser implementada. A implementaçao desse  método 
 * deve inicialmente atualizar o atributo _running,  verificar se a próxima thread (next) é nula ou não. 
 * Se for, nada precisa ser feito (isso só ocorre quando  a fila de prontos é vazia e não há thread para ser escalonada). 
 * Se não for, então a thread a ser executada precisa ser  colocada no estado RUNNING e retirada da fila de prontos. 
 * Então deve ser verificado se a thread anterior  (previous) é diferente de nula e também se é diferente da próxma thread. 
 * Se não for, então basta restaurar o contexto da  próxima thread, invocando static void CPU::restore_context(Thread* next). 
 * Se for, então é preciso verificar se a thread  anterior estava no estado RUNNING e caso sim, então a thread anterior deve 
 * passar para o estado READY. Após esse teste  deve-se fazer a troca de contexto entre as threads, invocando o método 
 * static void CPU::switch_context(Thread* previous, Thread* next).
 **/
void Thread::dispatch(Thread* previous, Thread* next) {
     Debug::cout(Debug::Level::trace, "Thread::dispatch(" + std::to_string(reinterpret_cast<unsigned long> (previous)) + "," + std::to_string(reinterpret_cast<unsigned long> (next)) + ")");
    // INSERT YOUR CODE HERE
    // ...
    // o atributo _running deve ser atualizado
    _running = next;
    // verificar se a próxima thread (next) é nula ou não. Se for, nada precisa ser feito
    if (next != nullptr) {
        // a thread a ser executada precisa ser colocada no estado RUNNING
        next->_state = Thread::State::RUNNING;
        // e retirada da fila de prontos
        System::scheduler()->remove(next);
        // deve ser verificado se a thread anterior (previous) é diferente de nula e também se é diferente da próxma thread
        if ((previous != nullptr) && (previous != next)) {
            //  verificar se a thread anterior estava no estado RUNNING
            if (previous->_state == Thread::State::RUNNING) {
                // a thread anterior deve passar para o estado READY
                previous->_state = Thread::State::READY;
            }
            // fazer a troca de contexto entre as threads
            CPU::switch_context(previous, next);
        } else {
            // Se não for, então basta restaurar o contexto da próxima thread
            CPU::restore_context(next);
        }
    }
}

/**
 * Threads criarão novas threads quando for invocada a chamada static Thread* Thread::thread_create(), que precisa ser
 * implementada. A implementação dessa chamada deve criar um objeto Thread, inicializar seus novos atributos, colocá-la na
 * lista de threads no sistema, inseri-la na fila de prontos inovando o método void Scheduler::insert(Thread* thread), 
 * e se o escolador for preemptivo, então deve chamar o escalonador para escolher outra thread, invocando 
 * Thread* Scheduler::choose() e então o despachador para iniciar a execução da thread escolhida pelo escalonador, 
 * invocando static void Thread::dispatch(Thread* previous, Thread* next).
 **/
Thread* Thread::thread_create(Process* parent) {
    Debug::cout(Debug::Level::trace, "Thread::create(" + std::to_string(reinterpret_cast<unsigned long> (parent)) + ")");
    // INSERT YOUR CODE HERE
    // ...
}
```

### TimerMediator.cpp

```c++
/*
 * File:   TimerMediator.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 2:55 PM
 */

#include "TimerMediator.h"
#include "Simulator.h"
#include "Debug.h"
#include "System.h"
#include "Traits.h"
#include "Thread.h"

/**
 * O sistema simulado possui ainda um timer, que gera interrupções periodicamente. O timer foi inicialmente configurado
 * para gerar interrupções a cada 100 unidades de tempo (ver arquivo Traits.h). Sempre que uma interrupção do timer ocorre, automaticamente é
 * invocado o método static void TimerMediator::interrupt_handler(), que precisa ser implementado. Esse método trata uma
 * interrupção do timer, e sua implementação depende do algoritmo de escalonamento. Ele pode ser usado para preemptar
 * uma thread caso seu time slice tenha sido atingido, no caso do algoritmo round-robin, por exemplo.
 */
void TimerMediator::interrupt_handler() {
    Debug::cout(Debug::Level::trace, "TimerMediator::interrupt_handler()");
    // INSERT YOUR CODE HERE
    // ...
    if (System::scheduler()->preemptive) {
        // INSERT YOUR CODE HERE
        // ...
    }
}
```

### Traits.h

```c++
/*
 * File:   Traits.h
 * Author: cancian
 *
 * Created on September 27, 2015, 4:16 PM
 */

#ifndef TRAITS_H
#define   TRAITS_H

#include "Debug.h"
#include "CPU.h"
#include "Thread.h"
#include "Model.h"
#include "Scheduler.h"
#include "Process.h"

template<typename T>
struct Traits {
    static const bool enabled = true;
    static const bool debugged = true;
};

template<> struct Traits<Debug> { // CHANGE THE DEBUG LEVEL HERE SETTING THE LEVELS YOU WANT TO SHOW
    static const bool error = true;
    static const bool warning = true;
    static const bool trace = true; //false;
    static const bool info = true; //false;
    static const bool fine = false;
};

template<> struct Traits<CPU> {
    static constexpr double context_switch_overhead = 1.0; // time units
    static constexpr double timer_interrupt_period = 100.0; // time units
};

template<> struct Traits<Process> {
    static constexpr double timeBetweenCresations = 1000.0; // time units
};

template<> struct Traits<Thread> {
    static constexpr double minCpuBurst = 150.0; // time units
    static constexpr double maxCpuBurst = 150.0; // time units
    static constexpr int minThreadsPerProcess = 3;
    static constexpr int maxThreadsPerProcess = 3;
};

template<> struct Traits<Model> {
    static constexpr double simulationLength = 3950.0; // time units
    static constexpr double firstCreation = 10.0; // time units
};

template<> struct Traits<MemoryManager> {
    static constexpr unsigned int memorySize = 1e6;
};

template<> struct Traits<Scheduler> {
    static constexpr double timeSlice = 200.0; // time units
    // INSERT YOUR CODE HERE
    // ...
};


#endif    /* TRAITS_H */
```
