//
//  main.c
//  file
//
//  Created by Jarry Shaw on 05/31/2018.
//  Copyright © 2018 Jarry Shaw. All rights reserved.
//

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH "/Users/jarryshaw/Documents/GitHub/OperatingSystem/file/file/"

void simMain(void);
void simHelp(void);
void simMode0(void);
void simMode1(void);
void simMode2(void);
void simMode3(void);
void simMode4(void);
void simMode5(void);
void simMode6(void);
void simMode7(void);

uint64_t fd_test(char *fd, size_t nbytes);
uint64_t fp_test(char *fp, size_t size, size_t nitems);

static __inline uint64_t rdtsc(void) __attribute__((always_inline));
static __inline uint64_t rdtsc() {
    uint64_t tsc;
    __asm __volatile("rdtsc": "=A" (tsc));
    return tsc;
}

uint64_t fd_test(char *fd, size_t nbytes) {
    int fd_main, fd_file;
    fd_main = open(PATH "main.c", O_RDONLY);
    fd_file = open(fd, O_WRONLY|O_CREAT);

    char *buf = (char *)malloc(nbytes);
    ssize_t n;
    uint64_t tsc = rdtsc();
    while ((n=read(fd_main, buf, nbytes)) > 0) {
        write(fd_file, buf, n);
    }
    uint64_t time = rdtsc() - tsc;

    free(buf);
    close(fd_main);
    close(fd_file);
    return time;
}

uint64_t fp_test(char *fp, size_t size, size_t nitems) {
    FILE *fp_main, *fp_file;
    fp_main = fopen(PATH "main.c", "rb");
    fp_file = fopen(fp, "wb");

    struct stat st;
    stat(PATH "main.c", &st);
    size_t restsize = st.st_size % size;

    char *buf = (char *)malloc(size);
    uint64_t tsc = rdtsc();
    while (fread(buf, size, nitems, fp_main) > 0) {
        fwrite(buf, size, nitems, fp_file);
    }
    if (restsize)
        fwrite(buf, restsize, 1, fp_file);
    uint64_t time = rdtsc() - tsc;

    free(buf);
    fclose(fp_main);
    fclose(fp_file);
    return time;
}

void simMode0() {
    uint64_t time_1, time_2;
    time_1 = fd_test(PATH "read-write@1.c", 1);
    time_2 = fp_test(PATH "fread-fwrite@1.c", 1, 1);
    printf("read/write@1 = %llu\n", time_1);
    printf("fread/fwrite@1 = %llu\n", time_2);
}

void simMode1() {
    uint64_t time_1, time_2;
    time_1 = fd_test(PATH "read-write@1024.c", 1024);
    time_2 = fp_test(PATH "fread-fwrite@1024.c", 1024, 1);
    printf("read/write@1024 = %llu\n", time_1);
    printf("fread/fwrite@1024 = %llu\n", time_2);
}

void simMode2() {
    FILE *fp_main, *fp_file;
    fp_main = fopen(PATH "main.c", "r");
    fp_file = fopen(PATH "fscanf-fprintf.c", "w");

    char buf;
    uint64_t tsc = rdtsc();
    while (fscanf(fp_main, "%c", &buf) > 0) {
        fprintf(fp_file, "%c", buf);
    }
    uint64_t time = rdtsc() - tsc;

    fclose(fp_main);
    fclose(fp_file);
    printf("fscanf/fprintf = %llu\n", time);
}

void simMode3() {
    FILE *fp_main, *fp_file;
    fp_main = fopen(PATH "main.c", "r");
    fp_file = fopen(PATH "fgetc-fputc.c", "w");

    int c;
    uint64_t tsc = rdtsc();
    while ((c=fgetc(fp_main)) != EOF) {
        fputc(c, fp_file);
    }
    uint64_t time = rdtsc() - tsc;

    fclose(fp_main);
    fclose(fp_file);
    printf("fgetc/fputc = %llu\n", time);
}

void simMode4() {
    FILE *fp_main, *fp_file;
    fp_main = fopen(PATH "main.c", "r");
    fp_file = fopen(PATH "fgets-fputs.c", "w");

    char buf[1024];
    uint64_t tsc = rdtsc();
    while (fgets(buf, 1024, fp_main)) {
        fputs(buf, fp_file);
    }
    uint64_t time = rdtsc() - tsc;

    fclose(fp_main);
    fclose(fp_file);
    printf("fgets/fputs = %llu\n", time);
}

void simMode5() {
    int conn[2];
    pipe(conn);

    if (fork()) {
        int fd;
        fd = open(PATH "main.c", O_RDONLY);
        close(conn[0]);

        char buf[1024];
        ssize_t n;
        uint64_t tsc = rdtsc();
        while ((n=read(fd, buf, 1024)) > 0) {
            write(conn[1], buf, n);
        }
        uint64_t time = rdtsc() - tsc;
        printf("pipe[parent] = %llu\n", time);

        close(conn[1]);
        close(fd);
    } else {
        int fd;
        fd = open(PATH "pipe.c", O_WRONLY|O_CREAT);
        close(conn[1]);

        char buf[1024];
        ssize_t n;
        uint64_t tsc = rdtsc();
        while ((n=read(conn[0], buf, 1024)) > 0) {
            write(fd, buf, n);
        }
        uint64_t time = rdtsc() - tsc;
        printf("pipe[child] = %llu\n", time);

        close(conn[0]);
        close(fd);
    }
    exit(EXIT_SUCCESS);
}

void simMode6() {
    char pathname[] = PATH "fifos";
    mknod(pathname, S_IFIFO|0666, 0);

    if (fork()) {
        int fd_fifo, fd_main;
        fd_fifo = open(pathname, O_WRONLY);
        fd_main = open(PATH "main.c", O_RDONLY);

        char buf[1024];
        ssize_t n;
        uint64_t tsc = rdtsc();
        while ((n=read(fd_main, buf, 1024)) > 0) {
            write(fd_fifo, buf, n);
        }
        uint64_t time = rdtsc() - tsc;
        printf("fifo[parent] = %llu\n", time);

        close(fd_fifo);
        close(fd_main);
    } else {
        int fd_fifo, fd_file;
        fd_fifo = open(pathname, O_RDONLY);
        fd_file = open(PATH "fifo.c", O_WRONLY|O_CREAT);

        char buf[1024];
        ssize_t n;
        uint64_t tsc = rdtsc();
        while ((n=read(fd_fifo, buf, 1024)) > 0) {
            write(fd_file, buf, n);
        }
        uint64_t time = rdtsc() - tsc;
        printf("fifo[child] = %llu\n", time);

        close(fd_fifo);
        close(fd_file);
    }
    unlink(pathname);
    exit(EXIT_SUCCESS);
}

void simMode7() {
    int fd_main, fd_file;
    fd_main = open(PATH "main.c", O_RDONLY);
    fd_file = open(PATH "mmap.c", O_RDWR|O_CREAT, 0666);

    struct stat st;
    stat(PATH "main.c", &st);
    ftruncate(fd_file, st.st_size);

    char *buf_main, *buf_file;
    buf_main = (char *)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd_main, 0);
    buf_file = (char *)mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_file, 0);

    uint64_t tsc = rdtsc();
    memcpy(buf_file, buf_main, st.st_size);
    uint64_t time = rdtsc() - tsc;
    printf("mmap = %llu\n", time);

    munmap(buf_main, st.st_size);
    munmap(buf_file, st.st_size);
    close(fd_main);
    close(fd_file);
}

void simMain() {
    char input;
    simHelp();
    while (1) {
        printf("\nPlease type the command: ");
        if (scanf("%1s", &input) == EOF) return;
        switch (input) {
            case '0': simMode0(); continue;
            case '1': simMode1(); continue;
            case '2': simMode2(); continue;
            case '3': simMode3(); continue;
            case '4': simMode4(); continue;
            case '5': simMode5(); continue;
            case '6': simMode6(); continue;
            case '7': simMode7(); continue;
            case 'h': case 'H': simHelp(); continue;
            case 'q': case 'Q': printf("Quitting...\n\n"); return;
            default: printf("Invalid command.\n"); continue;
        }
    }
}

void simHelp() {
    printf("=================== Available Commands ===================\n");
    printf(" Command |                   Description                  \n");
    printf("---------|------------------------------------------------\n");
    printf("    0    |       read/write & fread/fwrite @ 1,1,1        \n");
    printf("    1    |    read/write & fread/fwrite @ 1024,1024,1     \n");
    printf("    2    |                 fscanf/fprintf                 \n");
    printf("    3    |                  fgetc/fputc                   \n");
    printf("    4    |                  fgets/fputs                   \n");
    printf("    5    |                      pipe                      \n");
    printf("    6    |                      FIFO                      \n");
    printf("    7    |                      mmap                      \n");
    printf("    h    |             show this help message             \n");
    printf("    q    |                quit simulation                 \n");
    printf("==========================================================\n");
}

int main(int argc, const char * argv[]) {
    printf("===================== File Interface =====================\n\n");
    simMain();
    printf("==========================================================\n\n");
    return 0;
}
