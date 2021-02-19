/*
    The program reads a sequence of integers
    (no longer than 1024) and prints them in ascending order.
*/
#include <stdio.h>

#define MAX_SIZE 1024

/* Shell sort: */
void shell_sort(int elements, int *array) {
    /*
         Inner - left element, when comparing in the interval,
         Outer - the right element, when comparing in the interval (fixed)
    */
    int inner, outer;
    int value_to_insert; /* value to insert */
    int interval = 1; /* the spacing between elements */
   
    /* The maximum interval must be < number of elements: */
    while (3 * interval <= elements) {
        interval = interval*3 +1;
    }

    while (interval > 0) {
        for (outer = interval; outer < elements; outer++) {
            value_to_insert = array[outer];
            inner = outer;
            
            while (inner > interval - 1 && array[inner - interval] >= value_to_insert) {
                array[inner] = array[inner - interval];
                inner -= interval;
            }
            array[inner] = value_to_insert;
        }
        
        /* Reducing the interval. When interval=1 - classic sorting */
        interval = (interval - 1) / 3;
    }
}

int main() {
    int numb;
    int array[MAX_SIZE];
    for (numb = 0; numb < MAX_SIZE; numb++) {
        int tmp;
        tmp = scanf("%d", &array[numb]);
        
        if(tmp == EOF) {
            break;
        } else if (tmp == 0) {
            fprintf(stderr, "Input error\n");
            return 1;
        } else if (numb == MAX_SIZE) {
            fprintf(stderr, "Too much data\n");
            return 2;
        }
    }
    
    printf("Initial array: ");
    for (int i = 0; i < numb; i++) {
        printf("%d ", array[i]);
    }
    
    printf("\n");
    shell_sort(numb, array);
    printf("Sorted array: ");
    
    for (int i = 0; i < numb; i++) {
        printf("%d ", array[i]);
    }
    
    printf("\n");
    return 0;
}
