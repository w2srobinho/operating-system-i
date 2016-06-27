## Processo

#### Criando
Processos chegarão ao sistema quando for invocada a chamada static Process* Process::exec(), que precisa ser implementada.

* A implementação dessa chamada de sistema deve criar um Process (assuma que o pai de todos os processos tem id=0)

* alocar memória para ele invocando a chamada de sistema Partition* MemoryManager::allocateMemory(unsigned int size)

* inicializar os novos atributos que porventura você tenha criado

* colocá-lo na lista de processos no sistema

* criar uma thread (correspondente a "main") para esse processo invocando a chamada static Thread* Thread::thread_create(Process* parent). 

* O método retorna o processo criado.



#### Finalizando
Processos serão finalizados quando for invocada a chamada static void Process::exit(int status = 0), que precisa ser implementada.

* A implementação dessa chamada deve desalocar a memória do processo que extá executando invocando void MemoryManager::deallocateMemory(Partition* partition)

* excluir todas as threads desse processo

* excluir o processo (destruir o objeto Process)

* invocar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose()

* e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando void Thread::dispatch(Thread* previous, Thread* next).



## Threads



#### Criando
Threads criarão novas threads quando for invocada a chamada static Thread* Thread::thread_create(Process* parent), que precisa ser implementada.

* A implementação dessa chamada deve criar um objeto Thread

* inicializar os novos atributos que porventura você tenha criado

* colocá-la na lista de threads no sistema

* e se o escalocador for preemptivo então deve chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose()

* e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next). 

* O método retorna a thread criada.



#### Término
Threads aguardarão pelo término de outras threads quando for invocada a chamada void Thread::join(), que precisa ser implementada. 

* A implementação dessa chamada deve verificar se a thread this passada como parâmetro está no estado FINISHING ou não.
    
    * Se estiver, então a thread que invocou a chamada (a que está executando --running--) simplesmente continua sua execução.
    
    * Se não estiver, 
        
        * então a thread executando deve ser colocada no estado WAITING e colocada na fila de threads esperando pela thread this.
        
        * Ainda nesse caso, deve-se chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose()
        
        * e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).
        
        * Para melhor ilustrar essa chamada, se o código da thread T1 tiver a chamada T2->join(), então T1 é quem está executando (running) e T2 é quem foi invocada (this), e é T1 que deve ser bloqueada esperando por T2 (se T2 não estiver FINISHING).



#### Término
Threads serão finalizadas quando for invocada a chamada void Thread::exit(), que precisa ser implementada.

* A implementação dessa chamada deve colocar a thread que está executando no estado FINISHING,

* verificar se há alguma thread na lista de threads bloqueadas esperando por essa thread.
    
    * Se houver,
    
        * todas as threads na lista devem ser colocadas no estado READY
    
        * e colocadas na fila de threads pronas para executar.
    
    * Em qualquer caso, deve-se ainda chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose()
    
    * e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).



#### Deixar CPU
Threads podem decidir deixar a CPU invocando a chamada void Thread::yield(), que precisa ser implementada. 

* A implementação dessa chamada deve colocar a thread que está executando no estado READY, incluí-la na fila de threads prontas,
* chamar o escalonador para escolher outra thread, invocando Thread* Scheduler::choose() 
* e então o despachador para iniciar a execução da thread escolhida pelo escalonador, invocando static void Thread::dispatch(Thread* previous, Thread* next).

* Threads são despachadas, ou seja, têm seus contextos trocados, quando se invoca a chamada static void Thread::dispatch(Thread* previous, Thread* next), que já está implementada. 

* A implementaçao desse  método deve inicialmente verificar se a próxima thread (next) é nula ou não. 
    
    * Se for, nada precisa ser feito (isso só ocorre quando a fila de prontos é vazia e não há thread para ser escalonada).
    
    * Se não for, então o atributo _running deve ser atualizado e a thread a ser executada precisa ser colocada no estado RUNNING e retirada da fila de prontos. 

* Então deve ser verificado se a thread anterior (previous) é diferente de nula e também se é diferente da próxma thread.

    * Se não for, então basta restaurar o contexto da próxima thread, invocando static void CPU::restore_context(Thread* next). 
    
    * Se for, então é preciso verificar se a thread anterior estava no estado RUNNING e 
    
        * caso sim, então a thread anterior deve passar para o estado READY e ser colocada na fila de threads prontas. 
    
    * Após esse teste deve-se fazer a troca de contexto entre as threads, invocando o método static void CPU::switch_context(Thread* previous, Thread* next).



#### Solicitar ao escalonador
Threads podem solicitar ao escalonador para escolher outra thread para executar, invocando o método Thread* Scheduler::choose(), que deve ser implementado. 

* A implementação desse método deve verificar se há alguma thread na fila de threads prontas para executar.
    
    * Se houver, deve retornar o primeiro elemento dessa fila, SEM O EXCLUIR da fila.
    
    * Se a fila estiver vazia, deve retornar nulo.
    
* A fila de escalonamento é a Scheduling_Queue, e ela deve ser ordenada pelo atributo _prioriry da Thread, independente do algoritmo de escalonamento, ou seja, o escalonador pode usar qualquer forma de ordenamento, mas o resultado do ordenamento deve estar no atributo _priority, e a fila Scheduling_Queue deve ser ordenada de modo que Threads com menor valor numérico de _priority estejam antes na fila.

Quando o algoritmo de escalonamento usado for de prioridade dinâmica ou então quando algum mecanismo do tipo aging for usado, pode ser necessário alterar algum atributo das threads que estão na fila de prontos e reordenar essa fila conforme os novos valores dos atributos. Nesses casos, isso pode ser feito quando a fila de threads prontas for alterada, invocando o método void Scheculer::reschedule(), que precisa ser implementado. 

* Esse método precisa varrer a fila de pronto ajustando os atributos necessários das threads na fila, 

* possivelmente tendo que removê-las da fila e depois reinseri-las para que o ordenamento ocorra, o que depende da estrutura de dados da fila sendo usada.



#### Timer
O sistema simulado possui ainda um timer, que gera interrupções periodicamente. O timer foi inicialmente configurado para gerar interrupções a cada 100 unidades de tempo (ver arquivo de configurações Traits.h). Sempre que uma interrupção do timer ocorre automaticamente é invocado o método static void TimerMediator::interrupt_handler(), que precisa ser implementado.

* Esse método trata uma interrupção do timer, e sua implementação depende do algoritmo de escalonamento.

* Ele pode ser usado para preemptar uma thread caso seu time slice tenha sido atingido, no caso do algoritmo round-robin, por exemplo.

As classes Scheduler e MemoryManager podem ser obtidas usando os métodos static Scheduler* System::scheduler()  e static MemoryManager* System::memoryManager().

Dependendo do algoritmo de escalonamento a ser implementado, pode ser necessário obter o tempo atual. Isso pode ser útil para salvar informações sobre quando uma thread foi escalonada e calcular quanto tempo tempo ela usou a CPU, por exemplo. Para obter o tempo atual, use o método double Simulator::getInstance()->getTnow().

Algumas configurações de depuração, de overhead, do timer e do modelo de simulação em si podem ser ajustados no arquivo Traits.h. Sua implementação deveria funcionar para diferentes configurações.