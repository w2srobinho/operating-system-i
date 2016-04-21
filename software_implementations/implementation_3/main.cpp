#include <iostream>
#include <pthread.h>

//nome do aluno: Willian de Souza


using namespace std;

/* declare a mutex */
pthread_mutex_t mymutex;

void *inc_(void *void_ptr) {
    /* increment x to 100 */
    int *ptr = (int *) void_ptr;
    int i = 0;
    for (; i < 100; i++) {
        /* enter critical region */
        pthread_mutex_lock(&mymutex);
        ++(*ptr);
        /* leave critical region */
        pthread_mutex_unlock(&mymutex);
    }
    cout << "increment finished" << endl;
    return NULL;
}

void *dec_(void *void_ptr) {
    /* decrement x to 100 */
    int *ptr = (int *) void_ptr;
    int i = 0;
    for (; i < 100; i++) {
        /* enter critical region */
        pthread_mutex_lock(&mymutex);
        --(*ptr);
        /* leave critical region */
        pthread_mutex_unlock(&mymutex);
    }
    cout << "decrement finished" << endl;
    return NULL;
}


int main() {
    int x = 0;
    cout << "x: " << x << endl;

    /* declare threads */
    pthread_t inc_thread, dec_thread;
    /* init mutexex */
    pthread_mutex_init(&mymutex, NULL);

    /* create a first thread which executes inc_(&x) */
    if (pthread_create(&inc_thread, NULL, &inc_, (void *) &x)) {
        cout << "Error creating inc_thread.";
        return -1;
    }

    /* create a second thread which executes dec_(&x) */
    if (pthread_create(&dec_thread, NULL, &dec_, (void *) &x)) {
        cout << "Error creating dec_thread.";
        return -1;
    }

    void *value;
    /* wait for the first thread to finish */
    if (pthread_join(inc_thread, &value)) {
        cout << "Error while waiting for inc_thread finish.";
        return -1;
    }

    /* wait for the second thread to finish */
    if (pthread_join(dec_thread, &value)) {
        cout << "Error while waiting for dec_thread finish.";
        return -1;
    }

    /* destroy miutex */
    pthread_mutex_destroy(&mymutex);

    cout << "x: " << x << endl;
    return 0;
}

/* Respostas do "Para pensar e responder junto ao código:"

    Q: Na implementação computacional 1 (IC 1) o que acontecia com a variável
    global count quando ela era incrementada pelos diferentes processos?
    R: Como cada processo possui sua area de memória (a memória não é 
    compartilhada entre eles), ambos criavam uma cópia dessa variável em sua
    area de memórira e incrementavam essa cópia.
    
    Q: O mesmo vai acontecer agora quando a variável global x for incrementada
    e decrementada pelas diferentes threads?
    R: Negativo, diferentemente dos proccessos, threads possuem memória 
    compartilhada. E por causa disso temos que sincronizar a threads usando 
    semáfaros na região crítica de cada uma. Assim evitamos condição de corrida.
    
    Q: Qual é o valor esperado para a variável x após o término do aplicativo?
    R: Como temos a operação de escrita na variável sincronizada, é de se 
    esperar que a variável x possua o valor 0 ao término do programa, já que 
    temos 100 repetições de cada operação (incremento e decremento).
    
    Q: Se não houver mutex, qual será o valor final da variável global x (ou 
    sua distribuição de probabilidade)?
    R A distribuição de probabilidade é de 50% para cada operação, como 
    discutido em sala de aula, com essa distribuição o valor final pode ser 50.
*/