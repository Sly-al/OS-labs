#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "string.h"
#include "errno.h"
#include "semaphore.h"
#include "signal.h"

#include "lab4.h"

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

int ParentRoutine(FILE* stream)
{
    const int MAXLENGTH = 100;
    const int SIZE = MAXLENGTH + sizeof(int);
    const int ZERO = 0;
    unlink("file1");
    unlink("file2");
    int  file1 = open("file1", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int  file2 = open("file2", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if ( file1 == -1 ||  file2 == -1) {
        perror("open error");
        return EXIT_FAILURE;
    }
    if ( ftruncate(file1, SIZE - 1) == -1 ) {
        perror("ftruncate");
        return EXIT_FAILURE;
    }
    if ( ftruncate(file2, SIZE - 1) == -1 ) {
        perror("ftruncate");
        return EXIT_FAILURE;
    }
    
    sem_t* sem1 = sem_open("semaphore1", O_CREAT, S_IRUSR | S_IWUSR, 0);
    sem_t* sem2 = sem_open("semaphore2", O_CREAT, S_IRUSR | S_IWUSR, 0);

    if (sem1 == SEM_FAILED || sem2 == SEM_FAILED){
        perror("sem_open error");
        return EXIT_FAILURE;
    }
    int id = fork();
    if ( id == -1) {
        perror("Parent: fork error");
        return EXIT_FAILURE;
    }

// child
    if (id == 0)
    {
        void* in = mmap(NULL, SIZE, PROT_READ, MAP_SHARED,  file1, 0);
        void* ans = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED,  file2, 0);
        if (in == MAP_FAILED || ans == MAP_FAILED)
        {
            perror("mmap error");
            return EXIT_FAILURE;
        }
        unlink("result.txt");
        int fout = open("result.txt", O_CREAT | O_WRONLY, S_IRUSR);
        if (fout == -1)
        {
            perror("open error");
            return EXIT_FAILURE;
        }
        char* str = calloc(MAXLENGTH, sizeof(char));
        if (str == NULL)
        {
            perror("calloc error");
            return EXIT_FAILURE;
        }
        while (1)
        {
            sem_wait(sem1);
            int n = 0;
            memcpy(&n, in, sizeof(int));
            if (n == 0){
                break;
            }
            memcpy(str, (void*)((char*)in + sizeof(int)), n);
            long long num = atol(str); 
            if (IsPrime(num) == 1)
            {
                int k = 404;
                memcpy(ans, &k, sizeof(int));
                sem_post(sem2);
                munmap(in, SIZE);
                munmap(ans, SIZE);
                close(fout);
                free(str);
                break;
            }
            else
            {
                memcpy(ans, &ZERO, sizeof(int));
                write(fout,(void*)((char*)in + sizeof(int)), n);
            }
            sem_post(sem2);
        }
        munmap(in, SIZE);
        munmap(ans, SIZE);
        close(fout);
        free(str);
    }

// parent 
    else
    {
        void* out = mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED,  file1, 0);
        void* ans = mmap(NULL, SIZE, PROT_READ, MAP_SHARED,  file2, 0);
        if (out == MAP_FAILED || ans == MAP_FAILED)
        {
            perror("mmap error");
            return EXIT_FAILURE;
        }
        char* err = calloc(MAXLENGTH, sizeof(char));
        if (err == NULL)
        {
            perror("calloc error");
            return EXIT_FAILURE;
        }
        char* str;
        size_t s = 0;
        int n = getline(&str, &s, stream);
        while (n > 0)
        {
            memcpy(out, &n, sizeof(int));
            memcpy((void*)((char*)out + sizeof(int)), str, n);
            sem_post(sem1);
            sem_wait(sem2);
            int k;
            memcpy(&k, ans, sizeof(int));
            if (k != 0)
            {
                break;
            }
            n = getline(&str, &s, stream);
        }
        kill(id, SIGKILL);
        memcpy(out, &n, sizeof(int));
        munmap(out, SIZE);
        munmap(ans, SIZE);
        free(err);
        sem_close(sem1);
        sem_close(sem2);
        close(file1);
        close(file2);
        unlink("file1");
        unlink("file2");
    }
    return EXIT_SUCCESS;
}
