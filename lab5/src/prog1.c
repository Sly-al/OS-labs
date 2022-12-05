#include<stdio.h>
#include <stdlib.h>
#include"lab5.h"


int main(){

    int t;
    printf("1 - translation, 2 - sort of array\n");

    while(scanf("%d", &t) != EOF){  

        switch (t) {
            case 1:{

                printf("Enter x = ");
                int x;
                scanf("%d", &x);
                char* result = Translation(x);
                printf("Translation is %s\n", result);
                free(result);
                break;

            }
            case 2:{

                printf("Enter size of array = ");
                unsigned long size;
                scanf("%lu", &size);
                int* arr = malloc(size * sizeof(int));
                printf("Start input:\n");

                for (int i = 0; i < size; ++i){
                    scanf("%d", arr + i);
                }

                printf("Sorted: \n");
                int* ans = Sort(arr, size);

                for (int i = 0; i < size; ++i){
                    printf("%d ", ans[i]);
                }

                printf("\n");
                free(ans);
                free(arr);
                break;

            }
            default:{

                printf("1 - translation, 2 - sort\n");
                break;

            }

        }
        
    }
    return 0;
}