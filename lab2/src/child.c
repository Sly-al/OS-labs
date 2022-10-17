#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "fcntl.h"


int IsPrime(long long n) {
    if (n <= 1) {
        return 1;
    }
    for (long long i = 2; i*i <= n; i++) {
	    if (n%i == 0) {
		    return 0;
        }
    }
    return 1;
}

int main(const int argc, char* argv[]) {    
    int p1 = open("pipe1", O_RDONLY);
    int p2 = open("pipe2", O_WRONLY);
    if (argc != 2) {
        printf("Necessary arguments were not provided\n");
        exit(EXIT_FAILURE);
    }
    if (p1 == -1 || p2 == -1) {
        perror("Child: pipe open error");
        exit(EXIT_FAILURE);
    }
    unlink(argv[1]);
    int fout = open(argv[1], O_CREAT | O_WRONLY, S_IREAD | S_IWRITE); 
    if (fout == -1) {
        perror("Child: file error");
        exit(EXIT_FAILURE);
    }
    
    if (dup2(p1, 0) == -1 || dup2(fout, 1) == -1) {
        perror("Child: dup error");
        exit(EXIT_FAILURE);
    }

    char* str;
    size_t n = 0;
    int s = getline(&str, &n, stdin);
    long long num = atol(str);
    char chek[2] = "01";
    while (s > 0) {      
        if (IsPrime(num) == 1) {
            if (write(p2, &chek[0], 1) == -1) {
                perror("Child: write error");
            }
            close(fout);
            close(p1);
            close(p2);
            exit(EXIT_FAILURE);
        } else {
            printf("%s", str);
            fflush(stdout);
            if (write(p2, &chek[1], 1) == -1) {
                perror("Child: write error");
                exit(EXIT_FAILURE);
            }
        }
        s = getline(&str, &n, stdin);
        num = atol(str);
    } 
    close(p1);
    close(p2);
    close(fout);
}
