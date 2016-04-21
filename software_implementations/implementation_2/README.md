## Recommended for 22/03: \[Software Implementation\] (IC2) Linux System Calls and API based on unit "Memory Management"
**Requested files:** main.cpp (Download)

**Maximum number of files:** 2

**Maximum upload file size:** 128 KiB

**Type of work:** Individual work

##### Esta é a Implementação Computacional 2 (IC2).

Ela deve ser realizada individualmente e seu prazo de entrega pode ser definido pelo próprio acadêmico, desde que nenhuma outra atividade individual (ou em dupla) seja entregue na mesma semana e desde que seja entregue até a 16a semana de aula. Para avisar ao professor quando esta atividade estiver concluída, coloque uma mensagem no fórum adequado, conforme indicado nas regras da disciplina.

Esta avaliação deve ser realizada exclusivamente pelo ambiente Moodle, que permite a execução do aplicativo a ser desenvolvido. A avaliação da atividade também se dará pelo Moodle, e usará como critérios de correção:
* (i) Funcionamento do aplicativo conforme o previsto, sem travamentos ou erros;
* (ii) Utilização de estruturas de dados e estilo de codificação adequados;
* (iii) Documentação do aplicativo, incluído no próprio código-fonte (acima de cada nova chamada de sistema usada, explicando os parâmetros e flags utiliados). O professor também observará atentamente a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero, além de outras penalidades cabíveis.

O restante deste texto descreve o que deve ser feito nesta implementação computacional.

De forma geral, para esta atividade prática você deve implementar um aplicativo que tenha dois processos, o pai (main) e um novo processo-filho. Quando o processo-filho executa, ele deve criar um segmento de memória compartilhada do tamanho de uma mensagem específica e escrever uma mensagem nessa memória compartilhada, e então terminar. Quando o process-pai executar, ele deve primeiro esperar até que o processo-filho tenha terminado e então deve obter acesso ao segmento de memória compartilhada criado pelo filho e ler a mensagem dessa memória.

Mais especificamente, ao criar o objeto compartilhado, o processo-filho deve garantir que as flags indiquem que ele seja criado (já que ele não existe ainda), que deve rertornar erro caso um objeto com aquele nome já exista e deve ser criado para leitura e para escrita. O modo de abertura (`mode_t`) deve permitir que ele seja lido, escrito e executado pelo dono e pelo grupo. Ao mapear o objeto compartilhado na memória, o processo-filho deve especificar proteção para leitura e escrita da página na memória, e as flags devem especificar que a memória mapeada deve ser compartilhada. Já o processo-pai, ao criar (abrir) o objeto compartilhado, deve garantir que as flags indiquem que o objeto deve ser aberto com acesso de leitura e escrita (o objeto já deve existir, pois foi criado pelo filho). Todo o resto deve ser igual ao processo-filho.

Para implementar esse aplciativo, você vai precisar das seguintes funções que dão acesso a chamadas de sistema do SO: `fork`, `getpid`, `waitpid`, `shm_open`, `ftruncate`, `mmap`, `shm_unlink`, e várias constantes, principalmente as flags para abertura/criação do objeto compartilhado e mapeamento na memória, como `PROT_READ`, `O_RDWR`, `S_IRWXU`, `MAP_SHARED`, etc. Pesquise no material disponibilizado (ou na internet) como essas funções operam e implemente o aplicativo solicitado, completando a estrutura básica do código que já é fornecida em `main.cpp`

**Para pensar:** e responder junto ao código: As funções usadas criam um segmento de memória de tamanho especificado (truncado) e depois há proteção de páginas. Qual é o modelo de gerenciamneto de memória usado? Esse mecanismo pode ser usado para implementar sincronização de processos, como uma alternativa aos semáforos? Os processos que se comunicam via memória compartilhada precisam estar no mesmo arquivo executável como nesse exemplo ou eles podem estar em arquivos separados? Eles precisam estar executando no mesmo processador ou podem estar executando em processadores diferentes? Eles precisam estar executando no mesmo computador ou podem estar executando em computadores diferentes? Por fim, se o objetivo é comunicar dois processos no mesmo computador, tente fazer o mesmo usando pipes.

## Requested files

### main.cpp

```c++
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHARED_OBJ_NAME     "/oioioi1234"
#define MAX_MSG_LENGTH      /* max length */

struct message_struct {
    pid_t sender;
    int type;
    char content[MAX_MSG_LENGTH];
};

using namespace std;
int main() {
    int shared_object;
    int shared_seg_size = sizeof(struct message_struct);
    struct message_struct *shared_message;

    cout << "Parent process " << /* process ID */ << " is creating a child process" << endl;

    /* duplicate process */

    /* if error in duplicating */
        return -1;

    /* if child-process running then */
        cout << "Child process " << /* process ID */ << " is creating a shared memory object to write a message in" << endl;
        shared_object = /* create shared object */
        if (shared_object < 0) {
            cout << "Error "<< shared_object << " creating shared object" << endl;
            return -1;
        }
        /*  make room for the shared object to fit a message*/
        //...
        shared_message = (struct message_struct *) /* map the shared onject to memory */
        if (shared_message == NULL) {
            cout << "Error in memory map" << endl;
            return -1;
        }
        /* producing a message on the shared segment */
        shared_message->type = 1;
      shared_message->sender = /* process ID */
      strcpy(shared_message->content, /* message to be sent */);
        cout << "Child process " << /* process ID */ << " wrote message '" << shared_message->content << "' in memory" << endl;
        return 0;

    /* if parent-process running then */
        cout << "Parent process " << /* process ID */ << " is waiting for child to exit" << endl;
        /* wait for child process to exit and get its status */
      /* if status is not success */
          cout << "Parent process " << /* process ID */ << " is exiting since child could not write message in memory" << endl;
          return -1;

        cout << "Parent process " << /* process ID */ << " will read message from process " << /* child process ID */ << " finished with status "<< /* status of finished child process */ << endl;
        shared_object = /* create the shared object to read from */
        if (shared_object < 0) {
            cout << "Error in shm_open()" << endl;
            return -1;
        }

        shared_message = (struct message_struct *)/* map the shared object to memory */
        if (shared_message == NULL) {
            cout << "Error in memory map" << endl;
            return -1;
        }

        cout << "Parent process " << /* process ID */ << " read the message '" << shared_message->content << "' from sender " << shared_message->sender << " in memory " << endl;

        int removed = /* remove the shared object */
        if (removed != 0) {
            cout << "Error removing the shared object" << endl;
            return -1;
        }
    }
}
```
