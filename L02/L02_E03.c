#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    pid_t pid;
    printf("P1%d\n", getpid());
    if (pid == 0) {
        printf("P3%d\n", getpid());
        if (pid == 0) {
            printf("P6%d\n", getpid());
            exit(0);
        }
        else {
            while (wait((int*)0) != pid);
        }
        printf("P8%d\n", getpid());
    }
    else {
        printf("P2%d\n", getpid());
        if (pid == 0) {
            printf("P5%d\n", getpid());
            exit(0);
        }
        else {
            printf("P4%d\n", getpid());
            while (wait((int*)0) != pid);
        }
        printf("P7%d\n", getpid());
        while (wait((int*)0) != pid);
        printf("P9%d\n", getpid());
    }
    return(1);
}
