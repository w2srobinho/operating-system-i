## Recommended for 03/05: \[Problem Solving\] (SP2) Solve problem based on the unit "Memory Management"
**Due date:** terça, 28 junho 2016, 11:55

**Requested files:** `MemoryManager.cpp`, `MemoryManager.h`, `Traits.h`

**Maximum number of files:** 4

**Maximum upload file size:** 128 KiB

**Type of work:** Group work SP2 - Agrupamento 1: Grupo Único.

##### Esta é a avaliação de 'Solução de Problemas 2" (SP2).

Ela é uma avaliação extra classe em grupos (mesmo agrupamento) realizada em algumas horas (~2 a ~10) e corresponde basicamente a um exercício de programação encontrado na bibliografia, ao final de alguns capítulos. Ala avalia as "habilidades" dos acadêmicos na resolução de problemas práticos envolvendo os conhecimentos teóricos estudados, e também suas "habilidades" em projetar, modelar, simular, implementar e avaliar os principais componentes de um sistema operacional. Das cerca de 4 soluções de problemas realizadas, será desconsiderada aquela com menor nota.

A Solução de Problemas deve ser realziada exclusivamente através do editor de programas do Moodle, em grupos, e é avaliada conforme os seguintes critérios:
* (i) corretude e funcionamento do programa implementado, que deve seguir rigorosamente a descrição fornecida, resolvendo o problema proposto sem travamentos ou comportamentos inesperados;
* (ii) o projeto e modelagem do código orientado a objetos e as estruturas de dados utilizados;
* (iii) a documentação do sistema, conforme doxigen, realizada ao longo do código,
* (iv)  a capacidade de cooperar e trabalhar em grupo e a utilização de práticas de estudo independente e atitudes profissionais e éticas. Também será observada a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero. O restante deste texto descreve o que deve ser feito nesta Atividade Prática.

Para esta atividade prática você deve implementar um gerenciadorde processos e threads.

### SP2  — Contiguous Memory Allocation

PS: O professor ainda está ajustando esta atividade e a forma de correção automática

Para esta atividade prática você deve implementar um gerenciador de memória  baseado numa estratégia de alocação contínua, como partições estáticas ou dinâmicas. A alocação de memória deve ocorrer quando um processo for iniciado, e deve utilizar os algoritmo de alocação

a) First-Fit,
b) Next-Fit, 
c) Best-fit
d) Worst-fit.

Para a implementação desta Solução de Problemas cada aluno deverá submeter três arquivos pelo Moodle: "`MemoryMamager.h`", "`MemoryManager.cpp`" e "`Traits.h`". O aluno pode acrescentar atributos e métodos privados no header da classe **MemoryManager** que forem necessários em sua implementação, mas não pode modificar os métodos públicos. De forma geral, o aluno deve implementar os métodos:
```c++
Partition* MemoryManager::allocateMemory(unsigned int size);
```
,
```c++
void MemoryManager::deallocateMemory(Partition* partition);
```
e
```c++
void MemoryManager::showMemory();
```
, que alocam a memória necessária a um processo, liberam toda a memória do processo, e mostram informações sobre a memória alocada, respectivamente. Se houver necessidade de incluir outras classes (como uma lista ou fila, por exemplo), elas devem ficar nos próprios arquivos do **MemoryManager** (.h e .cpp). Os arquivos disponibilizados no Moodle indicam onde o código do aluno pode ser inserido, geralmente num comentário do tipo  _// INSERT YOUR CODE ..._

Para fins de testes automatizados (ainda em desenvolvimento), você precisará implementar dois outros métodos, que dependem das estruturas de dados que forem usadas. São eles:
```c++
unsigned int MemoryManager::getNumPartitions();
```
e
```c++
Partition* MemoryManager::getPartition(unsigned int index);
```
, que retornam a quantidade de partições existentes (alocadas ou não) e que retornam um ponteiro para a partição de número "index" (a index-ésima partição), respectivamente. O arquivo `MemoryManager.cpp` deixa claro o que precisa ser implementado. A classe **MemoryManager** precisa implementa todos os algoritmos de alocação de memória que podem ser usados, e que é passado como parâmetro ao construtor da classe. Você pode salvá-lo num atributo privado e usá-lo no método **allocateMemory**.

Além dos arquivos de cabeçalho e implementação da classe **MemoryManager** que estão disponíveis aos alunos, o sistema associado a esta Solução de Problemas usa outras muitas classes que o aluno não tem acesso, incluindo um simulador, que gera eventos de chegada e término de processos e que geram as demandas por alocação e desalocação de memória, respectivamente. O simulador basicamente cria entidades (que representam processos) periodicamente, invoca a alocação de memória para esses processos, simula a execução dos processos apenas esperando por um tempo, invoca a desalocação de memória para o processo e termina o processo. Isso se repete por um tempo que denota a duração da simulação.

Vários parâmetros da simulação podem ser ajustados no arquivo `Traits.h`, incluindo nível de depuração, duração da simulação, tempo entre chegadas dos processos, tamanho dos processos e vários outros. Os parâmetros mais mportantes que você pode alterar para testar mudanças no comportamento do sistema, estão listados abaixo.

```c++
template<> struct Traits<Process> {
    static constexpr double timeBetweenCreations = 50.0; // time units
    static constexpr unsigned int minAddressSpace = 10e3; // bytes
    static constexpr unsigned int maxAddressSpace = 200e3; // bytes
};

template<> struct Traits<Debug> { // CHANGE THE DEBUG LEVEL HERE SETTING THE LEVELS YOU WANT TO SHOW
    // debug levels
    static const bool error = true;
    static const bool warning = true;
    static const bool trace = true; //false;
    static const bool info = false; //true;
    static const bool fine = false; //true;
    //
    static const bool showEntityAttributes = false;
    static const bool showListOfEvents = false;
    static const bool pauseOnEveryEvent = false; //true;
};

template<> struct Traits<Model> {
    static constexpr double simulationLength = 5000.0; // time units
    static constexpr double firstCreation = 0.0;       // time units
};

template<> struct Traits<MemoryManager> {
    static constexpr unsigned int physicalMemorySize = 1e6; // bytes
};
```
Um exemplo de saída de um código funcional é apresentado abaixo, a título de exemplo.
```
Normal execution selected
Simulation is initing.
Replication 1 is starting.
[0]     <Trace> MemoryManager::allocateMemory(34993)
0-34992: ALLOCATED 34993
34993-999999: FREE 965008
[50]    <Trace> MemoryManager::allocateMemory(153571)
0-34992: ALLOCATED 34993
34993-188563: ALLOCATED 153571
188564-999999: FREE 811437
[100]   <Trace> MemoryManager::allocateMemory(97147)
0-34992: ALLOCATED 34993
34993-188563: ALLOCATED 153571
188564-285710: ALLOCATED 97147
285711-999999: FREE 714290
[150]   <Trace> MemoryManager::allocateMemory(111230)
0-34992: ALLOCATED 34993
34993-188563: ALLOCATED 153571
188564-285710: ALLOCATED 97147
285711-396940: ALLOCATED 111230
396941-999999: FREE 603060
[200]   <Trace> MemoryManager::allocateMemory(51604)
0-34992: ALLOCATED 34993
34993-188563: ALLOCATED 153571
188564-285710: ALLOCATED 97147
285711-396940: ALLOCATED 111230
396941-448544: ALLOCATED 51604
448545-999999: FREE 551456
        <Trace> MemoryManager::deallocateMemory(15825616)
0-34992: FREE 34994
34993-188563: ALLOCATED 153571
188564-285710: ALLOCATED 97147
285711-396940: ALLOCATED 111230
396941-448544: ALLOCATED 51604
448545-999999: FREE 551456
[250]   <Trace> MemoryManager::allocateMemory(18938)
0-18937: ALLOCATED 18938
18938-34992: FREE 16056
34993-188563: ALLOCATED 153571
188564-285710: ALLOCATED 97147
285711-396940: ALLOCATED 111230
396941-448544: ALLOCATED 51604
448545-999999: FREE 551456
        <Trace> MemoryManager::deallocateMemory(15826112)
0-18937: ALLOCATED 18938
18938-188563: FREE 169627
188564-285710: ALLOCATED 97147
285711-396940: ALLOCATED 111230
396941-448544: ALLOCATED 51604
448545-999999: FREE 551456
[...CONTINUA...
```
**Se houver algum problema com a execução do código-fonte dispobilizado, problemas ou dúvidas sobre o o funcionamento do sistema entre em contato com o professor com a maior antecedência possível. Sugere-se que se use o fórum de discussão reservado para isso.**

## Requested files

### MemoryManager.cpp

```c++
 /*
  * File:   MemoryManager.cpp
  * Authors: INSERT YOUR NAME HERE
  * Strategy: INSERT THE MEMORY MANAGEMENT SCHEMA HERE (THE ALGORITHM YOU ARE IMPLEMENTING -- RANDOMLY GENERATED BY MOODLE AS A VARIATION)
  *
  * Created on ...
  */
 
 
 #include "Debug.h"
 #include "MemoryManager.h"
 #include "Simulator.h"
 #include "Traits.h"
 
 #include <iostream>
 
 
 MemoryManager::MemoryManager(MemoryAllocationAlgorithm algorithm) {
     // INSERT YOUR CODE TO INITIALIZE MEMORY
     // ...   
 }
 
 MemoryManager::MemoryManager(const MemoryManager& orig) {
 }
 
 MemoryManager::~MemoryManager() {
 }
 
 Partition* MemoryManager::allocateMemory(unsigned int size) {
     Debug::cout(Debug::Level::trace, "MemoryManager::allocateMemory(" + std::to_string(size) + ")");
     // INSERT YOUR CODE TO ALLOCATE MEMOTY FOR THE PROCESS
     // ...
  
     return nullptr;
 }
 
 void MemoryManager::deallocateMemory(Partition* partition) {
     Debug::cout(Debug::Level::trace, "MemoryManager::deallocateMemory(" + std::to_string(reinterpret_cast<unsigned long> (partition)) + ")");
     // INSERT YOUR CODE TO DEALLOCATE MEMORY OF THE PROCESS
     // ...
 
 }
 
 unsigned int MemoryManager::getNumPartitions() {
     // INSERT YOUR CODE TO RETURN THE QUANTITY OF ALLOCATED PARTITIONS
     // ...
     return 0;  // CHANGE TO THE CORRET RETURN VALUE
 }
 
 Partition* MemoryManager::getPartition(unsigned int index) {
     // INSERT YOUR CODE TO RETURN THE PARTITION AT INDEX index
     // ...
 
     return 0; // CHANGE TO THE CORRET RETURN VALUE
 }
 
 void MemoryManager::showMemory() {
     // INSERT YOUR CODE TO SHOW THE MEMORY MAP, IN THE FOLLOWING FORMAT
     // <beginAddress>-<endAddress>: <FREE|ALLOCATED> <size>
     
     // Exemplo:
     /*
       0-1499:FREE 1500
       1500-1999:ALLOCATED 500
       2000-2999:ALLOCATED 1000
       3000-9999:FREE 7000
       10000-19999:ALLOCATED 10000
       20000-1000000:FREE 800000
     */
 
 
     // INSERT YOUR CODE TO SHOW THE MEMORY MAP
     // ...
 }
```

### MemoryManager.h

```c++
 /*
  * File:   MemoryManager.h
  * Authors: INSERT YOUR NAME HERE
  * Strategy: INSERT THE MEMORY MANAGEMENT SCHEMA HERE (THE ALGORITHM YOU ARE IMPLEMENTING -- RANDOMLY GENERATED BY MOODLE AS A VARIATION)
  *
  * Created on ...
  */
 
 #ifndef MEMORYMANAGER_H
 #define   MEMORYMANAGER_H
 
 class Partition {
 public:
 
     Partition(unsigned int beginAddress, unsigned int endAddress, bool isFree) {
         _beginAddress = beginAddress;
         _endAddress = endAddress;
         _isFree = isFree;
     }
 public: // do not change
 
     unsigned int getBeginAddress() const {
         return _beginAddress;
     }
 
     unsigned int getEndAddress() const {
         return _endAddress;
     }
 
     unsigned int getLength() const {
         return _endAddress - _beginAddress + 1;
     }
 private: // do not change
     unsigned int _beginAddress;
     unsigned int _endAddress;
     bool _isFree;
 private:
     // INSERT YOUR CODE HERE
     // ...
 };
 
 enum MemoryAllocationAlgorithm {FirstFit, NextFit, BestFit, WorstFit};
 
 class MemoryManager {
 public: // do not change
     MemoryManager(MemoryAllocationAlgorithm algorithm);
     MemoryManager(const MemoryManager& orig);
     virtual ~MemoryManager();
 public: // do not change
     Partition* allocateMemory(unsigned int size);
     void deallocateMemory(Partition* partition);
     void showMemory();
     unsigned int getNumPartitions();
     Partition* getPartition(unsigned int index);
 private: // private attributes and methods
     // INSERT YOUR CODE HERE
     // ...
 };
 
 #endif    /* MEMORYMANAGER_H */
#endif    /* SCHEDULER_H */
```

### Traits.h

```c++
 /*
  * File:   Traits.h
  * Author: cancian
  *
  * Created on September 27, 2015, 4:16 PM
  */
 
 
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
 
 template<> struct Traits<Process> {
     static constexpr double timeBetweenCreations = 50.0; // time units
     static constexpr unsigned int minAddressSpace = 10e3; // bytes
     static constexpr unsigned int maxAddressSpace = 200e3; // bytes
 };
 
 template<> struct Traits<Debug> { // CHANGE THE DEBUG LEVEL HERE SETTING THE LEVELS YOU WANT TO SHOW
     // debug levels
     static const bool error = true;
     static const bool warning = true;
     static const bool trace = true; //false;
     static const bool info = false; //true;
     static const bool fine = false; //true;
     //
     static const bool showEntityAttributes = false;
     static const bool showListOfEvents = false;
     static const bool pauseOnEveryEvent = false; //true;
 };
 
 template<> struct Traits<CPU> {
     static constexpr double context_switch_overhead = 1.0; // time units
     static constexpr double timer_interrupt_period = 100.0; // time units
 };
 
 template<> struct Traits<Thread> {
     static constexpr double minCpuBurst = 200.0;   // time units
     static constexpr double maxCpuBurst = 500.0;   // time units
     static constexpr int maxBursts = 5;            // CPUBurst
     static constexpr int minThreadsPerProcess = 1; // threads
     static constexpr int maxThreadsPerProcess = 1; // threads
 };
 
 template<> struct Traits<Model> {
     static constexpr double simulationLength = 5000.0; // time units
     static constexpr double firstCreation = 0.0;       // time units
 };
 
 template<> struct Traits<MemoryManager> {
     static constexpr unsigned int physicalMemorySize = 1e6; // bytes
     static constexpr MemoryAllocationAlgorithm allocationAlgorithm = MemoryAllocationAlgorithm::BestFit;
 };
 
 template<> struct Traits<Scheduler> {
     static constexpr double timeSlice = 300.0; // time units
 };
 
 
 #endif    /* TRAITS_H */
```
