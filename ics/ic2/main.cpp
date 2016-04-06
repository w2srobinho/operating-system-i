#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define SHARED_OBJ_NAME     "/oioioi1234"
#define MAX_MSG_LENGTH      30 // max length

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

    cout << "Parent process " << getpid() << " is creating a child process" << endl;

    /* duplicate process */
    pid_t pid = fork();

    /* if error in duplicating */
    if(pid < 0) {
        return -1;
    } else if(pid == 0) { /* if child-process running then */
        cout << "Child process " << getpid() <<
                " is creating a shared memory object to write a message in" << endl;
        /* create shared object */
        shared_object = shm_open(SHARED_OBJ_NAME, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
        if (shared_object < 0) {
            cout << "Error "<< shared_object << " creating shared object" << endl;
            /* get error type */
            cout << "Errno "<< errno << endl;
            cout << "Str error: " << strerror(errno) << endl;
            return -1;
        }
        /*  make room for the shared object to fit a message*/
        ftruncate(shared_object, shared_seg_size);
        /* request the shared segment */
        shared_message = (struct message_struct *) mmap(NULL, shared_seg_size,
                                                        PROT_READ | PROT_WRITE,
                                                        MAP_SHARED, shared_object, 0);
        if (shared_message == NULL) {
            cout << "Error in memory map" << endl;
            return -1;
        }
        /* producing a message on the shared segment */
        shared_message->type = 1;
        shared_message->sender = getpid();/* process ID */
                strcpy(shared_message->content, "why so serious?");
        cout << "Child process " << getpid() <<
                " wrote message '" << shared_message->content <<
                "' in memory" << endl;
        return 0;
    } else { 
    /* if parent-process running then */
        cout << "Parent process " << getpid() << " is waiting for child to exit" << endl;
        int status;
        /* wait for child process to exit and get its status */
        /* if status is not success */
        if(wait(&status) < 0) {
            cout << "Parent process " << getpid() <<
                    " is exiting since child could not write message in memory" << endl;
            return -1;
        }

        cout << "Parent process " << getpid() <<
                " will read message from process " << pid <<
                " finished with status "<< status << endl;

        /* create the shared object to read from */
        shared_object = shm_open(SHARED_OBJ_NAME, O_RDWR, S_IRWXU | S_IRWXG);
        if (shared_object < 0) {
            cout << "Error in shm_open()" << endl;
            /* get error type */
            cout << "Errno "<< errno << endl;
            cout << "Str error: " << strerror(errno) << endl;
            return -1;
        }

        /* map the shared object to memory */
        shared_message = (struct message_struct *) mmap(NULL, shared_seg_size,
                                                        PROT_READ | PROT_WRITE,
                                                        MAP_SHARED, shared_object, 0);
        if (shared_message == NULL) {
            cout << "Error in memory map" << endl;
            return -1;
        }

        cout << "Parent process " << getpid() <<
                " read the message '" << shared_message->content <<
                "' from sender " << shared_message->sender <<
                " in memory " << endl;

        /* remove the shared object */
        int removed = shm_unlink(SHARED_OBJ_NAME);
        if (removed != 0) {
            cout << "Error removing the shared object" << endl;
            return -1;
        }

        return 0;
    }
}