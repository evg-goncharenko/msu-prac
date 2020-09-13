/*
    Программа считывает последовательность целых чисел
    (длина не более 1024) и печатает их по возрастанию.
*/
#include <stdio.h>

const int inf = 1024;

// Сортировка при помощи Shell sort:
void shell_sort(int elements, int *array)
{
    /*
        inner - левый элемент, при сравнении в интервале
        outer - правый элемент, при сравнении в интервале (фикс.)
    */
    int inner, outer;
    int value_to_insert; /* значение для вставки */
    int interval = 1; /* интервал между элементами */
   
    /* Максимальный промежуток должен быть < кол-ву элементов: */
    while(3 * interval <= elements){
      interval = interval*3 +1;
    }

    while(interval > 0){
        for(outer = interval; outer < elements; outer++){
            value_to_insert = array[outer];
            inner = outer;
            
            while(inner > interval-1 && array[inner - interval]
            >= value_to_insert){
                array[inner] = array[inner - interval];
                inner -= interval;
            }
         
            array[inner] = value_to_insert;
        }
        
        /* Уменьшаем интервал. При interval=1 - классическая сортировка */
        interval = (interval - 1) / 3;
    }
}

int main()
{
    int numb;
    int array[inf];
    scanf("%d", &numb);
    
    for(int i = 0; i < numb; i++){
        scanf("%d", &array[i]);
    }
    
    printf("Initial array: ");
    for(int i = 0; i < numb; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    
    shell_sort(numb, array);
    
    printf("Sorted array: ");
    for(int i = 0; i < numb; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    return 0;
}
