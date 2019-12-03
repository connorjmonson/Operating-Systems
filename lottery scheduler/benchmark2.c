#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>

int main() {
    int f = open("output", O_RDWR | O_CREAT | O_APPEND, 0777);
        if(f == -1) {
            perror("Error: open()");
        exit(0);
        }
        if(dup2(f, 1) == -1) {
            perror("Error: dup2()");
        exit(0);
    }

    int start;
    int end;
    start = 0;
    end = 0;
    start = (int)time(NULL);
    if(fork() == 0) {
        nice(20);
        volatile unsigned long long j;
        volatile unsigned long long k = 0;

        volatile unsigned long long loop_amt = 1000000000ULL/4;
        for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
        end = (int)time(NULL);
        printf("1: ");
        printf("%d\n",  end - start);
        exit(0);

    }
    else if(fork() == 0) {
        nice(15);
        volatile unsigned long long j;
        volatile unsigned long long k = 0;
        volatile unsigned long long loop_amt = 1000000000ULL/4;

        for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
        end = (int)time(NULL);
        printf("2: ");
        printf("%d\n", end - start);
        exit(0);

    }
    else if(fork() == 0) {
        nice(10);
        volatile unsigned long long j;
        volatile unsigned long long k = 0;
        volatile unsigned long long loop_amt = 1000000000ULL/4;

        for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
        end = (int)time(NULL);
        printf("3: ");
        printf("%d\n", end - start);
        exit(0);

    }
    else if(fork() == 0) {
        nice(5);
        volatile unsigned long long j;
        volatile unsigned long long k = 0;
        volatile unsigned long long loop_amt = 1000000000ULL/4;

        for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
        end = (int)time(NULL);
        printf("4: ");
        printf("%d\n", end - start);
        exit(0);

    }
    else if(fork() == 0) {
        nice(0);
        volatile unsigned long long j;
        volatile unsigned long long k = 0;
        volatile unsigned long long loop_amt = 1000000000ULL/4;

        for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
        end = (int)time(NULL);
        printf("5: ");
        printf("%d\n", end - start);
        exit(0);

    }
    else {
            start = (int)time(NULL);
            volatile unsigned long long j;
            volatile unsigned long long k = 0;
            volatile unsigned long long loop_amt = 1000000000ULL/4;

            for (j = 0; j < loop_amt; ++j) k = k * j % (1000);
            end = (int)time(NULL);
    }
    return 0;
}
