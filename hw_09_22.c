#include <stdio.h>
#include <math.h>

#define N 1024

/*
    Функция возвращает n-e число Фибоначчи
*/
 int fibonacci(int n);

/*
    Функция определяет является ли число простым
*/
int prime(int x);

/*
    Функция печатает все простые числа Фибоначчи, меньшие n
*/
void fibprimes(int n);

/*
    Переставляет числа в целом массиве так, чтобы вначале шли
    отрицательные элементы (в любом порядке), затем нулевые,
    затем положительные (также в любом порядке).
*/
void remap(int a[N]);

/*
    Функция печатает все простые числа, меньшие n
*/
void primes(int n);

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void remap(int a[N])
{
    int p_neg = -1, p_zero = -1;
    for(int i = 0; i < N; i++){
        if(a[i] == 0){
            ((p_neg + 1) > (p_zero + 1))?(p_zero = (p_neg + 1)):(p_zero++);
            swap(&a[p_zero], &a[i]);
        } else if(a[i] < 0){
            p_neg++;
            swap(&a[p_neg], &a[i]);
            if(p_zero != -1){
                p_zero++;
                swap(&a[p_zero], &a[i]);
            }
        }
    }
    
    printf("\nResult remap(): ");
    for(int i = 0; i < N; i++){
        printf("%d ", a[i]);
    }
    printf("\n");
}

void primes(int n)
{
    if ( n <= 1 ) {
        printf("Not exist\n");
        return;
    } else {
        printf("Result: ");
        
        int flag, tmp, numb;
        for(int i = 2; i <= n; i++){
            numb = i;
            flag = 1;
            tmp = 2;
            
            while((tmp < pow(numb, 0.5) + 1) && (flag)){
                if((numb % tmp == 0) && (numb != tmp)){
                    numb /= tmp;
                    flag = 0;
                } else{
                    tmp++;
                }
            }
            if(flag) printf("%d ", i);
        }
        printf("\n");
    }
}

void fibprimes(int n)
{
    if ( n <= 2 ) {
        printf("Not exist\n");
        return;
    } else {
        printf("Result: ");
 
        int cnt = 3; /* порядковый номер ЧФ */
        for(;;){
            int fib = fibonacci(cnt); /* находим очередное ЧФ */
 
            if(fib >= n) break;
 
            if(prime(fib)){
                printf("%d ", fib);
            }
            cnt++;
        }
        printf("\n");
    }
}

int fibonacci(int n)
{
    int i;
    int fib = 0; /* F(n) */
    int n1 = 1; /* F(n + 1) или F(n - 1) */
 
    if (n > 0){ /* F(n) = F(n - 2) + F(n - 1) */
        if (n <= 2) return 1;
 
        int n2 = 1; /* F(n - 2) */
        for (i = 3; i <= n; i++){
            fib = n2 + n1;
            n2 = n1;
            n1 = fib;
        }
 
    } else if(n < 0){ /* F(n) = F(n + 2) - F(n + 1) */
        if(n == -1) return 1;
 
        int n2 = 0; /* F( n + 2 ) */
        for (i = -2; i >= n; i--){
            fib = n2 - n1;
            n2 = n1;
            n1 = fib;
        }
    }
    
    return fib;
}
 
int prime(int n)
{
    if (n <= 1) return 0;
 
    int tmp = 2;
    while(tmp < pow(n, 0.5) + 1){
        if(n % tmp == 0){ /* если делитель найден, то число непростое */
            n /= tmp;
            return 0;
        } else{
            tmp++;
        }
    }
    
    return 1; /* если делитель не найден, то число простое */
}

int main()
{
    /*
    int mass[N];
    for(int i = 0; i < N; i++){
        scanf("%d", &mass[i]);
    }
    remap(mass);
    */
    
    /*
    primes(100);
    */
    
    /*
    fibprimes(50);
    */
    return 0;
}
