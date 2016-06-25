#include <iostream>

#include <stdio.h>
/* novos includes */
#include <cassert>
#include <signal.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

// Nome do Aluno: Willian de Souza

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
    /*!
     * sigaction, é uma estrutura de dados que guarda as informações que serão passadas
     * para a função \see sigaction()
     */
    struct sigaction action;
    /*!
     * itimerval, estrutura de dados onde é guardado os valores de tempo para reinialição
     * do temporizador e o tempo que resta para expirar e disparar um sinal
     */
    struct itimerval timer;
    /*!
     * Cada recurso tem limites atual e máximo associado definido por
     * rlimit, estrutura de dados que guarda os limites, atual e o limite máximo
     */
    struct rlimit limit;
    /*!
     * a estrutura rusage guarda as estatísticas de uso dos recursos
     */
    struct rusage usage;

    /* Instalar o tratador de sinal do alarme */

    /*!
     * Preenche uma n bytes da memória apontada por action com o
     * byte contante 0.
     *
     * O valor n é dado por sizeof(action).
     */
    memset(&action, 0, sizeof(action));
    /*!
     * coloca um ponteiro para a função que será invocada
     */
    action.sa_handler = &timer_handler;
    /*!
     * \brief Associa um alarme a uma ação, configurada atravédo do action
     * SIGALRM, é o sinal que será gerado
     * action, é o novo sinal que deverá ser involcado
     * nullptr, é para informar que não precisa salvar o sinal configurado anteriormente
     */
    sigaction(SIGALRM, &action, nullptr);

    /* Configurar o timer para disparar depois de 333ms e a cada 333ms depois disso*/

    /*!
     * Configuração expirar em 333ms e disparar o timer
     * tv_sec é uma unidade de tempo descrita em segundos
     * tv_usec é uma unidade de tempo descrita em microssegundos
     */
    timer.it_interval.tv_sec = 0;       ///< o valor é forçado para 0 por garantia
    timer.it_interval.tv_usec = 333000; ///< 1000 microssegundo == 1 milissegundos
    /*!
     * Configuração para expirar o timer a cada 333ms
     * tv_usec é uma unidade de tempo descrita em microssegundos
     */
    timer.it_value.tv_sec = 0;       ///< o valor é forçado para 0 por garantia
    timer.it_value.tv_usec = 333000; ///< 1000 microssegundo == 1 milissegundos

    /* Setar o timer para iniciar a contagem enquanto este processo estiver executando */

    /*!
     * \brief Configura um novo timer.
     *
     * O tipo ITIMER_REAL passado como paramêtro dispara um SIGALRM quando o tempo expera.
     * timer é o novo timer criado anteriormente
     * nullptr, é para informar que não é preciso guardar o timer anterior
     *
     * assert é uma verificação estática para garantir a execução do método setitimer
     */
    assert(!setitimer(ITIMER_REAL, &timer, nullptr) && "Erro ao definir o novo timer.");

    /* Obter os limites atuais deste processo */

    /*!
     * Pega os limites configurado para o recurso passado por parâmetro e coloca no
     * na estrutura apontada por limit
     *
     * O recurso requisitado é a cpu definido pela constante RLIMIT_CPU
     *
     * assert é uma verificação estática para garantir a execução do método getrlimit
     */
    assert(!getrlimit(RLIMIT_CPU, &limit) && "Erro ao pegar os limites do processo.");

    /* Ajustar o limite de uso da CPU para 2 segundos */

    /*!
     * Redefine a CPU para rodar por 2 segundos
     */
    limit.rlim_cur = 2;
    /*!
     * Redefine os limites para o recurso passado por parâmetro que estão
     * na estrutura apontada por limit, que foi alterada acima
     *
     * O recurso requisitado é a cpu definido pela constante RLIMIT_CPU
     *
     * assert é uma verificação estática para garantir a execução do método setrlimit
     */
    assert(!setrlimit(RLIMIT_CPU, &limit) && "Erro ao definir novos limites do processo.");

    /* Ficar preso num laço infinito */
    int i=0;
    while (1)
    {
        ++i;
        /* a cada 1 mihão de iterações, obter a utilização atual da CPU */
        if (i >= 1000000) {
            i = 0;
            /*!
             * Pegas as estastísticas de uso e coloca na estrutura apontada por usage
             *
             * RUSAGE_SELF, indica as estatísticas de uso do próprio processo
             */
            assert(!getrusage(RUSAGE_SELF, &usage) &&
                   "Erro ao pegar os recursos de uso do processo.");
            cout << "User CPU time: " << usage.ru_utime.tv_sec << "." << usage.ru_utime.tv_usec /* segundos.milissegundos usados pela aplicação */
                 << " - System CPU time: " << usage.ru_stime.tv_sec << "." << usage.ru_stime.tv_usec /* segundos.milissegundos usados pelo sistema operacional */
                 << endl;
        }
    }
    return 0;

    /*!
     * Respostas das perguntas "para pensar":
     *
     * Q - Qual é a precisão do timer?
     * R -
     *
     * Q - Ele disparou a execução do procedimento exatamente a cada 333ms?
     * R -
     *
     * Q - Como você verificou isso?
     * R -
     *
     * Q - Quando ele começou a contar?
     * R -
     *
     * Q - Como você sabe quanto tempo se passou desde que o sinal SIGALRM foi disparado
     *     pelo sistema operacional até o processo responder, executando o procedimento?
     * R -
     *
     * Q - Qual é a fração de tempo que o processo gastou executando instruções do usuário
     *     e instruções do sistema operacional?
     * R -
     */
}
