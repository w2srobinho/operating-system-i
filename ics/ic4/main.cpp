#include <iostream>
#include <stdio.h>
#include <stdlib.h>
/* ... other includes ... */
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

// Nome do aluno: Willian de Souza


using namespace std;

/** 
 * declare whenever global variables you need to synchronize pthreads using posix semaphores
 * 
 * auto descriptive variables
 */
const int NUMBER_OF_CHILDREN = 7;
const int NUMBER_OF_BALLS = 3;

/**
 * basket_full is a symbolic representation of state from basket to take ball
 * from basket;
 * A child could take the ball in basket if there is one;
 * This is basket with balls
 * 
 * basket_full is full initialized, with all balls in bascket (with 
 * NUMBER_OF_BALLS)
 */
 sem_t basket_full;

/**
 * basket_empty is a symbolic representation of state from basket for frop the
 * ball in basket;
 * A child could drop the ball in basket if there is room for it;
 * This is basket with limits to drop balls
 * 
 * basket_empty is empty initialized, without balls in bascket (with 0)
 */
 sem_t basket_empty;


void *child(void *void_ptr)
{
    unsigned short *childNum = (unsigned short *)void_ptr;
    for (unsigned short i=1; i<=5; i++) {
        cout << "Child " << *childNum << 
                " wants to play with the ball for the " << i << 
                "th time" << endl;
        /**
         * if the child has no ball, need to take one from the basket if there
         * is one, or will wait until there is a ball in the basket;
         * 
         * Use sem_wait (was called P by Dijkstra), passing the semaphore (
         * basket_full) as parameter;
         * Decrement the number of semaphore (the ball)
         * 
         * Use sem_post (was called V by Dijkstra), passing the semaphore (
         * basket_empty) as parameter;
         * Increment the number of semaphore (the ball), representing the full
         * or with room;
         * 
         * The child take a one ball from basket, but if is no ball he/she
         * must wait.
         */
        sem_wait(&basket_full);
        sem_post(&basket_empty);
        cout << " Child " << *childNum << " wants to take a ball from the basket" << endl;
 
        /* once the child has a ball, he/she starts to play */
        cout << "  Child " << *childNum << " is playing with the ball" << endl;
        /**
         * play with the ball for 1 second
         * 
         * Simulating that the child has playing with the ball for one second.
         */
        sleep(1);
    
    
        cout << "  Child " << *childNum << " wants to leave the ball in the basket" << endl;

        /**
         * when the child is tired of playing, he/she has to drop the ball into
         * the basket, if there is room for it (basket holds only 3 balls), or
         * will wait until another hild to take a ball
         * 
         * If is nor room in the basket he/she wait until another child to take
         * a ball, then drop the ball in basket (this is represented for 
         * sem_wait);
         * 
         * If the child dropped the ball in basket, increase the basker_full
         * using the sem_post (repesenting the basket for child take ball)
         */
        sem_wait(&basket_empty);
        sem_post(&basket_full);
        
        cout << " Child " << *childNum << " has droped the ball in the basket" << endl;
    }
    cout << "Child " << *childNum << " will no longer play" << endl;
    
    /* exit the thread */
    pthread_exit(nullptr);
}

int main()
{
    cout << "The kindengarten is open" << endl;
    
    /**
     * declare local variable, such as threads
     * 
     * Declaring array of threads called th_child, with size = 7
     */
    pthread_t th_child[NUMBER_OF_CHILDREN];
    // Use for passing child number as argument for thread;
    unsigned short childNumArg[NUMBER_OF_CHILDREN];

    /**
     * init semaphores to synchronize the threads
     * 
     * 1o parameter, is the semaphore that will be initialize in this case
     *               "basket_full" and "basket_empty";
     * 2o parameter, indicates whether this semaphore is to be shared between
     *               the threads of a process, or berween processes. In our
     *               case the "0" indicate that will be shared between threads;
     * 3o parameter, specifies the initial value for the semaphore. For 
     *               basker_full semaphore is NUMBER_OF_BALLS, for basket_empty
     *               semaphore is 0. This is used for to specific the balls
     *               in basket;
     */
    sem_init(&basket_full, 0, NUMBER_OF_BALLS);
    sem_init(&basket_empty, 0, 0);

    /**
     * create 7 threads for the children, passing to each one a different
     * number (child 0 to 6)
     */
    for(auto i = 0; i < NUMBER_OF_CHILDREN; ++i) {
        /**
         * Create each thread passing:
         * 1o parameter, a pointer to thread;
         * 2o parameter, not used it is a pointer to attributes passed to 
         *               thread;
         * 3o parameter, is a pointer to function child that go run thread;
         * 4o parameter, is a void pointer to argument passed in child function;
         * 
         * If thread could not create, print a message and return a error;
         * 
         * childNumArg[i] = i; - Copy value of the child number for other memory
         *                       address, for thread safe argument.
         */
        childNumArg[i] = i;
        int status = pthread_create(&th_child[i], nullptr,
                                    &child, (void *)&childNumArg[i]);
        if(status) { // check status != 0
            cout << "Error when create th_child " << i;
            return -1;
        }
    }
    
    /* wait for all children to finish */
    for(auto i = 0; i < NUMBER_OF_CHILDREN; ++i) {
        /**
         * For waiting children finish is used pthread_join:
         * 1o parameter, the thread that wait;
         * 2o parameter, a void pointer "value" that get the thrad return, we
         *               are using nullptr, beacause does not using the
         *               thread return;
         * 
         * If there a error while wait, print a message and return a error.
         */
        if(pthread_join(th_child[i], nullptr)) {
            cout << "Error while waiting for th_child " << i << " finish.";
            return -1;
        }
    }
    
    /**
     * Destroy semaphores
     */
     sem_destroy(&basket_full);
     sem_destroy(&basket_empty);

    cout << "The kindengarten is closed" << endl;
    return 0;
    
    /**
     * Respostas das perguntas "para pensar":
     * 
     * Q - Os semáfaros podem ser variáveis locais?
     * R - Não, os semáforos estão sendo usados no escopo global. Declarando-os
     *     localmente causaria erro de variável não declarado em algum escopo
     *     diferente ao que ela foi declarada (por ex. no escopo da função onde
     *     a thread é executada).
     * 
     * Q - Todas as threads "child" terminam? Justifique.
     * R - Sim, como o numero de bolas usadas pelas crianças (children), é igual
     *     ao numero de bolas suportadas pelo cesto, não é possível que uma
     *     criança fique bloqueada, pois todas as crianças sempre conseguirão
     *     colocar as bolas no cesto, ou seja, nenhuma thread será bloqueada.
     * 
     * Q - E se no cesto couber uma única bola, como na versão original do
     *     problema?
     * R - Só muda que terá apenas uma thread sendo executada por vez (uma
     *     criança jogando bola por vez). Agora, se o numero de bolas continuar
     *     sendo de 3, 2 threads ficarão bolqueadas.
     * 
     * Q - Há algum erro de programação para que as threads não terminem?
     * R - Nesta implementação, não consegui visualizar (até peço que se houver,
     *     o professor me dê um feedback) nessa implementação, acho que o
     *     problema ocorreria se o semáforo que cuida das bolas que são
     *     retiradas do cesto e o semáforo que cuida do limite de bolas que o
     *     cesto comporta, gerenciassem valores diferentes.
     * 
     * Q - O que poderia ser feito para detectar que um evento nunca ocorrerá e
     *     fazer com que o programa finalize com sucesso, encerrando todas suas
     *     threads?
     * R - Colocar um condicional forçando o comportamento das threads de
     *     maneira correta, por exemplo, no caso de uma única bola no cesto,
     *     era só limitar que uma única criança por vez pudesse jogar bola,
     *     dessa forma o cesto só teria uma bola e nenhuma criaça ficaria
     *     aguardando para colocar a bola no cesto (sempre teria espaço).
     */
}
