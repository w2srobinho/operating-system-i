## Recommended for 29/03: [Software Implementation] (IC3) Linux System Calls and API based on unit "Processes and Threads"
**Requested files:** main.cpp (Download)

**Maximum number of files:** 2

**Maximum upload file size:** 128 KiB

**Type of work:** Individual work

##### Esta é a Implementação Computacional 3 (IC3).

Ela deve ser realizada individualmente e seu prazo de entrega pode ser definido pelo próprio acadêmico, desde que nenhuma outra atividade individual (ou em dupla) seja entregue na mesma semana e desde que seja entregue até a 16a semana de aula. Para avisar ao professor quando esta atividade estiver concluída, coloque uma mensagem no fórum adequado, conforme indicado nas regras
da disciplina.

Esta avaliação deve ser realizada exclusivamente pelo ambiente Moodle, que permite a execução do aplicativo a ser desenvolvido. A avaliação da atividade também se dará pelo Moodle, e usará como critérios de correção:
* (i) Funcionamento do aplicativo conforme o previsto, sem travamentos ou erros;
* (ii) Utilização de estruturas de dados e estilo de codificação adequados;
* (iii) Documentação do aplicativo, incluído no próprio código-fonte (acima de cada nova chamada de sistema usada, explicando os parâmetros e flags utiliados). O professor também observará atentamente a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero, além de outras penalidades cabíveis.

O restante deste texto descreve o que deve ser feito nesta implementação computacional.

De forma geral, para esta atividade prática você deve implementar um aplicativo que crie duas threads. Uma delas deve incrementar uma variável global `x` 100 vezes, enquanto a outra deve decrementar essa mesma variável 100 vezes. No final, o valor da variável deve ser apresentada. Como essa variável corresponde a uma região crítica, deve-se garantir que não ocorrerá o problema da condição de corrida.

Mais especificamente, inicializa-se uma variável global `x` com `0`. Deve-se declarar as threads e o mutex que fará a sincronização. Deve-se então criar as duas threads (uma para incremento e outra para decremento), aguardar que ambas executem até terminar, destruir o mutex e apresentar o novo valor da variável x.

Para implementar esse aplciativo, você vai precisar das seguintes funções que dão acesso a chamadas de sistema do SO: `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_init`, `pthread_create`, `pthread_join`, `pthread_mutex_destroy`. Você também usará os tipos `pthread_t` e `pthread_mutex_t`. Pesquise no material disponibilizado (ou na internet) como essas funções operam e implemente o aplicativo solicitado, completando a estrutura básica do código que já é fornecida em `main.cpp`

**Para pensar e responder junto ao código:** Na implementação computacional 1 (IC 1) o que acontecia com a variável global `count` quando ela era incrementada pelos diferentes processos? O mesmo vai acontecer agora quando a variável global `x` for incrementada e decrementada pelos difereeentes threads? Qual é o valor esperado para a variável `x` após o término do aplicativo? Se não houver mutex, qual será o valor final da variável global `x` (ou sua distribuição de probabilidade)?

Após terminar a implementação, escolha a opção "Evaluate" para uma primeira avaliação (testes automatizados).

## Requested files

### main.cpp

```c++
#include <iostream>
#include <pthread.h>
//nome do aluno: 


using namespace std;

/* declare a mutex */ mymutex;

void *inc_(void *void_ptr)
{
  /* increment x to 100 */
  int *ptr = (int *)void_ptr;
  int i=0;
  for (; i<100; i++) 
  {
      /* enter critical region */
      ++(*ptr);
      /* leave critical region */
  }
  cout << "increment finished" << endl;
  return NULL;
}

void *dec_(void *void_ptr)
{
    /* decrement x to 100 */
    int *ptr = (int *)void_ptr;
  int i=0;
  for (; i<100; i++)
  {
      /* enter critical region */
      --(*ptr);
      /* leave critical region */
  }        
    cout << "decrement finished" << endl;
    return NULL;
}


int main()
{
  int x = 0;
  cout << "x: " << x << endl;

  /* declare threads */ inc_thread, dec_thread;
  /* init mutexex */

    /* create a first thread which executes inc_(&x) */

  /* create a second thread which executes dec_(&x) */

    /* wait for the first thread to finish */

  /* wait for the second thread to finish */

  /* destroy miutex */

  cout << "x: " << x << endl;
  return 0;
}

/* Respostas do "Para pensar e responder junto ao código:"
   ...
*/
```