//
//  main.c
//  fork
//
//  Created by Jarry Shaw on 05/10/2018.
//  Copyright © 2018 Jarry Shaw. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SHMKEY  123456  /* shared memory keyword */
#define SHMSIZE 70      /* shared memory lengths */
#define SEMKEY1  1234
#define SEMKEY2  4321
#define SEMSIZE 1

void simMain(void);
void simHelp(void);

void procComm(void);
void procFork(void);
void procHelp(void);

static int parent = 1;
static int child = 1;
static int semid1 = 0;
static int semid2 = 0;
static int shmid = 0;
static char *shmaddr = NULL;

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(int semid);
static int semaphore_v(int semid);

static int set_semvalue() {
    semun_t semun1;
    semun_t semun2;
    
    semun1.val = 1;
    semun2.val = 0;
    if (semctl(semid1, 0, SETVAL, semun1) == -1
        || semctl(semid2, 0, SETVAL, semun2) == -1) return 1;
    return 0;
}

static void del_semvalue() {
    if (semctl(semid1, 0, IPC_RMID) == -1
        || semctl(semid2, 0, IPC_RMID) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(int semid) {
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = -1;  //P()
    sem_b.sem_flg = SEM_UNDO;

    if (semop(semid, &sem_b, 1) == -1) return 1;
        // fprintf(stderr, "semaphore_p failed\n");
    return 0;
}

static int semaphore_v(int semid) {
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;   //V()
    sem_b.sem_flg = SEM_UNDO;
    
    if (semop(semid, &sem_b, 1) == -1) return 1;
        // fprintf(stderr, "semaphore_v failed\n");
    return 0;
}

void procComm(void){
    printf("\n================= Process Communication ==================\n\n");
    
    // create semaphores
    semid1=semget(SEMKEY1, SEMSIZE, 0666|IPC_CREAT);
    semid2=semget(SEMKEY2, SEMSIZE, 0666|IPC_CREAT);
    
    // map shared memory
    if ((shmid=shmget(SHMKEY, SHMSIZE, 0600|IPC_CREAT)) == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    if ((shmaddr=shmat(shmid, NULL, 0)) == NULL) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shmaddr[65] = 0;
    
    // initilisation
    if (set_semvalue()) {
        fprintf(stderr, "Failed to initialise semaphore\n");
        exit(EXIT_FAILURE);
    }
    
    if (fork()) {
        FILE *file = NULL;
        printf("Open source file.\n");
        file = fopen("./main.c", "r");
        if (file == NULL) return;
        while (parent) {
            if (semaphore_p(semid1)) exit(EXIT_FAILURE);
    
            if (fscanf(file, "%c", shmaddr) != EOF) {
                if (*shmaddr == '\n')
                    printf("[%lu] Sent `\\n' to child process.\n", clock());
                else
                    printf("[%lu] Sent `%s' to child process.\n", clock(), shmaddr);
            } else {
                printf("Parent process sending completed.\n");
                shmaddr[65] = EOF;
                parent--;
            }
            
            if (semaphore_v(semid2)) exit(EXIT_FAILURE);
        }
        fclose(file);
        shmdt(file);
    } else {
        FILE *file = NULL;
        printf("Open destination file.\n");
        file = fopen("./fork.c", "w");
        if (file == NULL) return;
        while (child) {
            if (semaphore_p(semid2)) exit(EXIT_FAILURE);
            if (shmaddr[65] != EOF) {
                fwrite(shmaddr, sizeof(char), 1, file);
                if (*shmaddr == '\n')
                    printf("[%lu] Received `\\n' from parent process.\n", clock());
                else
                    printf("[%lu] Received `%s' from parent process.\n", clock(), shmaddr);
            } else {
                printf("Child process receiving completed.\n");
                child--;
            }

            if (semaphore_v(semid1)) exit(EXIT_FAILURE);
        }
        fclose(file);
    }
    
    // aftermath
    del_semvalue();
    printf("[%d] - finished\n", getpid());
    exit(EXIT_SUCCESS);
}

void procFork() {
    printf("\n====================== Process Fork ======================\n\n");
    
    char input = '\0';
    procHelp();
    while (1) {
        printf("Please type the command: ");
        if (scanf("%1s", &input) == EOF) return;
        switch (input) {
            case 'Q': case 'q': return;
            default: {
                int i, pid, status = 1;
                while ((i=fork()) == -1);
                if (i) {    /* parent process */
                    printf("\n===================== Parent Process =====================\n\n");
                    pid = wait(&status);
                    printf("\n================== End of Child Process ==================\n\n");
                    printf("Child process ID %d finished with status %d.\n", pid, status);
                } else {    /* child process */
                    printf("\n===================== Child Process ======================\n\n");
                    switch (input) {
                        case 'D': case 'd':
                            execl("/usr/local/opt/python/Frameworks/Python.framework/Versions/3.6/bin/jsdaily", "jsdaily", "--help", (char*)0);
                        case 'P': case 'p':
                            execl("/usr/local/opt/python/Frameworks/Python.framework/Versions/3.6/bin/jspcapy", "jspcapy", "--help", (char*)0);
                        case 'H': case 'h': procHelp();
                        default: printf("Invalid command.\n");
                    }
                    exit(2);
                }
                printf("\n================= End of Parent Process ==================\n\n");
                procHelp();
            }
        }
    }
}

void procHelp() {
    printf("\n=================== Available Commands ===================\n\n");
    
    printf(" Command |                   Description                  \n");
    printf("---------|------------------------------------------------\n");
    printf("    d    |                 run `jsdaily`                  \n");
    printf("    p    |                 run `jspcapy`                  \n");
    printf("    h    |             show this help message             \n");
    printf("    q    |                quit simulation                 \n");
    
    printf("\n==========================================================\n\n");
}

void simMain() {
    char input;
    simHelp();
    while (1) {
        printf("Please type the command: ");
        if (scanf("%1s", &input) == EOF) return;
        switch (input) {
            case 'F': case 'f': procFork(); return;
            case 'S': case 's': procComm(); return;
            case 'H': case 'h': simHelp(); continue;
            case 'Q': case 'q': return;
            default: printf("Invalid command.\n"); continue;
        }
    }
}

void simHelp() {
    printf("\n=================== Available Commands ===================\n\n");
    
    printf(" Command |                   Description                  \n");
    printf("---------|------------------------------------------------\n");
    printf("    f    |          fork, wait, change and etc.           \n");
    printf("    s    |         signal, share memory and etc.          \n");
    printf("    h    |             show this help message             \n");
    printf("    q    |                quit simulation                 \n");
    
    printf("\n==========================================================\n\n");
}

int main(int argc, const char * argv[]) {
    printf("=================== Process Management ===================\n\n");
    simMain();
    printf("\nQuitting...\n");
    printf("\n==========================================================\n\n");
    return 0;
}
