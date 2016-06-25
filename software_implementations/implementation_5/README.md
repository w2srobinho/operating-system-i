## Recommended for 31/05: \[Software Implementation\] (IC5) Linux System Calls and API based on unit "Input/Output System"
**Requested files:** main.cpp (Download)

**Maximum upload file size:** 128 KiB

**Type of work:** Individual work

##### Esta é a Implementação Computacional 5 (IC5).

Ela deve ser realizada individualmente e seu prazo de entrega pode ser definido pelo próprio acadêmico, desde que nenhuma outra atividade individual (ou em dupla) seja entregue na mesma semana e desde que seja entregue até a 16a semana de aula. Para avisar ao professor quando esta atividade estiver concluída, coloque uma mensagem no fórum adequado, conforme indicado nas regras da disciplina. Você deve utilizar exclusivamente o editor do Moodle para esta implementação, para que informações pedagógicas sobre a realização desta atividade sejam coletadas.

Esta avaliação deve ser realizada exclusivamente pelo ambiente Moodle, que permite a execução do aplicativo a ser desenvolvido. A avaliação da atividade também se dará pelo Moodle, e usará como critérios de correção:
* (i) Funcionamento do aplicativo conforme o previsto, sem travamentos ou erros;
* (ii) Utilização de estruturas de dados e estilo de codificação adequados;
* (iii) Documentação do aplicativo, incluído no próprio código-fonte (acima de cada nova chamada de sistema usada, explicando os parâmetros e flags utiliados). O professor também observará atentamente a análise de "Similaridade" realizada automaticamente pelo Moodle, e trabalhos copiados receberão nota zero, além de outras penalidades cabíveis.

O restante deste texto descreve o que deve ser feito nesta implementação computacional.

De forma geral, para esta atividade prática você deve implementar um aplicativo que limite sua utilização de CPU a apenas 2 segundos, mesmo executando um laço infinito. Esse programa deve criar um alarme que dispara automaticamente um procedimento a cada 333ms e, para verificar se isso ocorre, dentro do laço infinito ele deve coltar informações sobre a quantidade de tempo de CPU já gasto pelo programa, tanto executando instruções do usuário quando executando instruções do sistema.

Mais especificamente, seu programa deve criar um tratador de sinal para o sinal `SIGALRM`, e associa-lo ao procedimento a ser invocado. Então é preciso configurar um timer (do tipo `itimerval`) para gerar um sinal depois do processo ter executado por 333ms e a cada 333ms de execução do processo depois disso. Note que pode levar mais de 333ms para o processo executar por 333ms.  Então, antes de entrar num laço infinito, seu programa deverá limitar seu tempo máximo de utilização de CPU para 2s. Para isso ele precisa obter os limites atuais impostos a ele, mudar o limite de CPU para 2s então mandar setar os novos limites. Então seu programa inicia um laço infinito. Dentro ele ele deve ficar mostrando o tempo de uso atual da CPU, tanto gasto pelo próprio programa quanto o tempo gasto pelo sistema operacional, quando executa chamadas de sistema invocadas pelo programa. Para limitar a quantidade de informação apresentada, mostre isso apenas a cada 1 milhão de iterações.

Para implementar esse aplicativo, você vai precisar das seguintes funções que dão acesso a chamadas de sistema do SO: `memset`, `sigaction`, `settimer`, `getrlimit`, `setrlimit`, `getrusage`. A documentação do código deve seguir o padrão doxygen ou outro styleguide, e deve aprentar a documentação completa da chamada de sistema do SO, seus parâmetros, flags, retornos, etc, demonstrando que o acadêmico pesquisou sobre a totalidade de suas funções. Não é permitido que você modifique a estrutura do código e principalmente as saídas que o sistema gera (cout), pois as saídas geradas pelo programa são base para sua correção automatizada. Se você criar ou modificar "couts", seu programa não irá passar nos testes e receberá nota zero.

**Para pensar e para responder no código:** Qual é a precisão do timer? Ele disparou a execução do procedimento exatamente a cada 333ms? Como você verificou isso? Quando ele começou a contar? Como você sabe quanto tempo se passou desde que o sinal `SIGALRM` foi disparado pelo sistema operacional até o processo responder, executando o procedimento? Qual é a fração de tempo que o processo gastou executando instruções do usuário e instruções do sistema operacional?

## Requested files

### main.cpp

```c++
#include <iostream>

#include <stdio.h>
/* novos includes */
// Nome do Aluno:

using namespace std;

void timer_handler(int signalNumber)
{
    static int counter = 0;
    counter++;
    cout << "Timer interrupt invoked "<< counter << " times" << endl;
}


int main ()
{
    /* declarações */

    /* Instalar o tratador de sinal do alarme */

    /* Configurar o timer para disparar depois de 333ms e a cada 333ms depois disso*/

    /* Setar o timer para iniciar a contagem enquanto este processo estiver executando */

    /* Obter os limites atuais deste processo */

    /* Ajustar o limite de uso da CPU para 2 segundos */

    /* Ficar preso num laço infinito */
    int i=0;
    while (1)
    {
        /* a cada 1 mihão de iterações, obter a utilização atual da CPU */
        cout << "User CPU time: " <<  /* segundos.milissegundos usados pela aplicação */ <<
        " - System CPU time: " <<  /* segundos.milissegundos usados pelo sistema operacional */  << endl;

    }
    return 0;
}

```
