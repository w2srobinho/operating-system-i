## Recommended for 10/05: [Software Implementation] (IC4) Linux System Calls and API for Parallel and Concurrent Programming
**Requested files:** main.cpp (Download)

**Maximum number of files:** 2

**Maximum upload file size:** 128 KiB

**Type of work:** Individual work

##### Esta é a Implementação Computacional 4 (IC4).

Ela deve ser realizada individualmente e seu prazo de entrega pode ser definido pelo próprio acadêmico, desde que nenhuma outra atividade individual (ou em dupla) seja entregue na mesma semana e desde que seja entregue até a 16a semana de aula. Para avisar ao professor quando esta atividade estiver concluída, coloque uma mensagem no fórum adequado, conforme indicado nas regras
da disciplina.

Esta avaliação deve ser realizada exclusivamente pelo ambiente Moodle, que permite a execução do aplicativo a ser desenvolvido. A avaliação da atividade também se dará pelo Moodle, e usará como critérios de correção:
* (i) Funcionamento do aplicativo conforme o previsto, sem travamentos ou erros;
* (ii) Utilização de estruturas de dados e estilo de codificação adequados;
* (iii) Documentação do aplicativo, incluído no próprio código-fonte (acima de cada nova chamada de sistema usada, explicando os parâmetros e flags utiliados). O professor também observará atentamente a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero, além de outras penalidades cabíveis.

O restante deste texto descreve o que deve ser feito nesta implementação computacional.

De forma geral, para esta atividade prática você deve implementar um aplicativo que implemente uma solução para o problema de programação concorrente da brincadeira das crianças.

**Mais especificamente, suponha o seguinte problema:** Imagine 7 crianças, que estão, a princípio, quietas. Três delas inicialmente possuem uma bola e as outras quatro, não. De repente, sentem vontade de brincar com uma bola. Com esse desejo incontrolável, as que já estão com a bola simplesmente brincam. As que não têm bola, correm ao cesto de bolas, que está inicialmente vazio e que suporta na verdade apenas três bolas (a versão original do problema informa que no cesto cabe uma única bola). Se o cesto estiver cheio, uma criança pega a bola e vai brincar feliz. Se o cesto estiver vazio, ela fica esperando até outra criança coloque uma bola no cesto. Quando uma criança termina de brincar, ela tem que colocar a bola no cesto, mas se o cesto já estiver cheio, ela segura a bola até que outra criança retire a bola que já está no cesto, e então solta sua bola no cesto e volta a ficar quieta. Implemente uma solução para esse problema sincronizando as threads "child" usando semáforos do padrão posix.

Para implementar esse aplciativo, você vai precisar das seguintes funções que dão acesso a chamadas de sistema do SO: `pthread_create`, `pthread_join`, `pthread_exit`, `sem_init`, `sem_wait` e `sem_post`.

**Para pensar e para responder no código:** Os semáforos podem ser variáveis locais? Todas as threads "child" terminam? Justifique. E se no cesto couber uma única bola, como na versão original do problema? Há algum erro de programação para que as threads não terminem? O que poderia ser feito para detectar que um evento nunca ocorrerá e fazer com que o programa finalize com sucesso, encerrando todas suas threads?

## Requested files

### main.cpp

```c++
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
/* ... other includes ... */
// Nome do aluno: ...


using namespace std;

/* declare whenever global variables you need to synchronize pthreads using posix semaphores */

void *child(void *void_ptr)
{
  unsigned short *childNum = (unsigned short *)void_ptr;
  for (unsigned short i=1; i<=5; i++) {
    cout << "Child " << *childNum << " wants to play with the ball for the " << i << "th time" << endl;
    /* if the child has no ball, need to take one from the basket if there is one, or will wait until there is a ball in the basket */
       cout << " Child " << *childNum << " wants to take a ball from the basket" << endl;

    /* once the child has a ball, he/she starts to play */
    cout << "  Child " << *childNum << " is playing with the ball" << endl;
    /* play with the ball for 1 second */


    cout << "  Child " << *childNum << " wants to leave the ball in the basket" << endl;

    /* when the child is tired of playing, he/she has to drop the ball into the basket, if there is room for it (basket holds only 3 balls), or will wait until another hild to take a ball */
    cout << " Child " << *childNum << " has droped the ball in the basket" << endl;

  }
  cout << "Child " << *childNum << " will no longer play" << endl;
  /* exit the thread*/

}

int main()
{
    cout << "The kindengarten is open" << endl;

    /* declare local variable, such as threads */

    /* init semaphores to synchronize the threads */

    /* create 7 threads for the children, passing to each one a different number (child 0 to 6) */

    /* wait for all children to finish */

    cout << "The kindengarten is closed" << endl;
    return 0;

    /* Respostas das perguntas "para pensar":
    ...
    */
}
```