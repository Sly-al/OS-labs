#include "unistd.h"
#include "stdio.h"
#include "sys/stat.h"
#include "sys/wait.h"
#include "fcntl.h"

#include "parent.h"

int ParentRoutine(FILE* stream){
    unlink("pipe1");
    unlink("pipe2");
    if (mkfifo("pipe1", S_IREAD | S_IWRITE) == -1 || mkfifo("pipe2", S_IREAD | S_IWRITE) == -1) {
        perror("Parent: pipe create error");
        return -1;
    }
    char* fout;
    size_t k = 0;
    int fout_n = getline(&fout, &k, stream);
    if (fout_n <= 0) {
        perror("Parent: file name error");
        return -1;
    }

    int id = fork();
    if (id == -1) {
        perror("Parent: fork error");
        return -1;
    }
    if (id != 0) {
        int p1 = open("pipe1", O_WRONLY);
        int p2 = open("pipe2", O_RDONLY);
        if (p1 == -1 || p2 == -1) {
            perror("Parent: pipe open error");
            return -1;
        }
        char* str;
        size_t n = 0;
        int s = getline(&str, &n, stream);
        while (s > 0) {
            if (write(p1, str, s) == -1) {
                perror("Parent: write error");
                return -1;
            }
            char chek;
            if (read(p2, &chek, 1) <= 0) {
                perror("Parent: read error");
                return -1;
            }
            if (chek == '0') {
                printf("Parent: number is prime or negative\n");
                return 0;
            }
            
            s = getline(&str, &n, stream);
            
        } 
        wait(NULL);   
        close(p1);
        close(p2);
        unlink("pipe1");
        unlink("pipe2");
        
    } else {
        fout[fout_n - 1] = '\0';
        char* argv[3];
        argv[0] = "child.c";
        argv[1] = fout;
        argv[2] = NULL;
        if (execv("child.out", argv) == -1) {
            perror("Child: exec error");
            return -1;
        }
    }
    return 0;
}
