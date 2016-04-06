#include <string>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;
int main ()
{
    // include declarations you need
    int count = 0;
    pid_t pid;
    /* Repeat 5 times */
    for(int i = 0; i < 5; ++i) {
        cout << "Parent process " << getpid() << ": Creating child" << endl;
        /* Duplicate this process */
        pid = fork();
        /* If there was an error on duplication then */
        if(pid < 0) {
            cout << "Error on fork()" << endl;
            return -1;
        }  
        /* If child-process is running then */
        if(pid == 0) {
            cout << "Child process " << getpid() << ": Running" << endl;
            count++;
            /* Sleep for 1 second */
            sleep(1);
            cout << "Child process " << getpid() << ": Exiting with status " << count << endl;
            return count;
        }
    }
    /* if this is the parent-process then */
    if(pid > 0) {
        cout << "Parent process " << getpid() << ": Waiting children to exit" << endl;
        int errno, status, sum = 0;

        /* ECHILD define no child processes */
        /* Parent-process waits for all children to exit, adding each status to the sum variable */
        while(errno != ECHILD) {            
            /**
             * The waitpid(pid_t pid, int *stat_loc, int options) when parameter pid is equal
             *     to -1, status is requested for any child process. In this respect,
             *     waitpid() is equivalent to wait(). 
             */
            if (wait(&status) > 0) {
                /**                 
                 * If is child process
                 * Increase return child process to sum variable
                 */
                sum += WEXITSTATUS(status);
            }
        }
        cout << "Parent process " << getpid() << ": Exiting with sum " << sum << endl;
    }
    return count;
}