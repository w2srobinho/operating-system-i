## Recommended for 15/03: \[Software Implementation\] (IC1) Linux System Calls and API based on unit "Foundamentals of Operating Systems"

**Avaliable from:** terça, 15 março 2016, 12:00

**Requested files:** main.cpp

**Maximum number of files:** 2

**Maximum upload file size:** 128 KiB

**Type of work:** Individual work

##### Esta é a Implementação Computacional 1 (IC1).

Ela deve ser realizada individualmente e seu prazo de entrega pode ser definido pelo próprio acadêmico, desde que nenhuma outra atividade individual (ou em dupla) seja entregue na mesma semana e desde que seja entregue até a 16a semana de aula. Para avisar ao professor quando esta atividade estiver concluída, coloque uma mensagem no fórum adequado, conforme indicado nas regras da disciplina.

Esta avaliação deve ser realizada exclusivamente pelo ambiente Moodle, que permite a execução do aplicativo a ser desenvolvido. A avaliação da atividade também se dará pelo Moodle, e usará como critérios de correção:
* (i) Funcionamento do aplicativo conforme o previsto, sem travamentos ou erros;
* (ii) Utilização de estruturas de dados e estilo de codificação adequados;
* (iii) Documentação do aplicativo, incluído no próprio código-fonte (acima de cada nova chamada de sistema usada, explicando os parâmetros e flags utiliados). O professor também observará atentamente a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero, além de outras penalidades cabíveis.

##### O restante deste texto descreve o que deve ser feito nesta implementação computacional.

De forma geral, para esta atividade prática você deve implementar um aplicativo que crie cinco processos-filhos que executam concorrentemente e, quando todos terminarem, o processo-pai termina também, apresentando a soma dos resultados (status) dos processos-filhos. 

Mais especficamente, este aplicativo deve ter uma variável global count, inicializada com `0`. O processo-pai deve criar 5 processos-filhos. Quando um processo-filho estiver executando, ele deve incrementar a variável count, dormir por 1 segundo e depois retornar o valor atualizado de count.
Após os processos-filhos terem sido criados, o processo-pai deve esperar que cada processo-filho termine. Quando isso contecer, ele deve acumular o valor de retorno de cada filho numa variável `sum`. Quando todos os filhos terminarem, o processo-pai mostra o valor de `sum` e termina também.

Para implementar esse aplciativo, você vai precisar das seguintes funções que dão acesso a chamadas de sistema do SO: `fork`, `getpid`, `sleep`, `waitpid`, e das constantes `ECHILD` e `WEXITSTATUS`.  Pesquise no material disponibilizado (ou na internet) como essas funções operam e implemente o aplicativo solicitado, completando a estrutura básica do código que já é fornecida em `main.cpp`

**Para pensar:** Há uma variável global `count` acessível a todos os processos e que é inicializada com `0`. Cinco processos são criados e cada um incrementa o valor atual dessa variável, retornando o valor incrementado. Se somarmos todos os valores retornados, quanto você acha que será apresentado? Existe possibilidade de condição de corrida?

## Requested files

### main.cpp

```c++
#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys//* Lib where system call is found */>
#include <sys//* Lib where system call is found */>

using namespace std;
int main ()
{
   // include declarations you need
   int count = 0;
   /* Repeat 5 times */
      cout << "Parent process " << /* PID of parent-process */ << ": Creating child" << endl;
      /* Duplicate this process */
      /* If there was an error on duplication then */
            cout << "Error on fork()" << endl;
            return -1;
      /* If child-process is running then */
            cout << "Child process " << /* PID of child-process */ << ": Running" << endl;
            count++;
            /* Sleep for 1 second */
            cout << "Child process " << /* PID of child-process */ << ": Exiting with status " << count << endl;

   /* if this is the parent-process then */
      cout << "Parent process " << /* PID of parent-process */ << ": Waiting children to exit" << endl;
      int errno, status, sum = 0;
      /* Parent-process waits for all children to exit, adding each status to the sum variable */
      cout << "Parent process " << /* PID of parent-process*/ << ": Exiting with sum " <<sum << endl;

   /*  */
   return count;
}
```
