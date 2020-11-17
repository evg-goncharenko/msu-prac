#include <stdio.h>
#include <math.h>

#define N 1024

/*
    The function returns the nth Fibonacci number
*/
 int fibonacci(int n);

/*
    The function determines whether a number is prime
*/
int prime(int x);

/*
    The function prints all Fibonacci primes less than n
*/
void fibprimes(int n);

/*
    Rearranges the numbers of the whole array so
    that negative elements (in any order) come first, then zero,
    then positive (also in any order)
*/
void remap(int a[N]);

/*
    The function prints all prime numbers less than 'n'
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
    for(int i = 0; i < N; i++)
    {
        if(a[i] == 0)
        {
            ((p_neg + 1) > (p_zero + 1))?(p_zero = (p_neg + 1)):(p_zero++);
            swap(&a[p_zero], &a[i]);
        }
        else if(a[i] < 0)
        {
            p_neg++;
            swap(&a[p_neg], &a[i]);
            if(p_zero != -1)
            {
                p_zero++;
                swap(&a[p_zero], &a[i]);
            }
        }
    }
    
    printf("\nResult remap(): ");
    for(int i = 0; i < N; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void primes(int n)
{
    if(n <= 1)
    {
        printf("Not exist\n");
        return;
    }
    else
    {
        printf("Result: ");
        
        int flag, tmp, numb;
        for(int i = 2; i <= n; i++)
        {
            numb = i;
            flag = 1;
            tmp = 2;
            
            while((tmp < pow(numb, 0.5) + 1) && (flag))
            {
                if((numb % tmp == 0) && (numb != tmp))
                {
                    numb /= tmp;
                    flag = 0;
                }
                else
                {
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
    if(n <= 2)
    {
        printf("Not exist\n");
        return;
    }
    else
    {
        printf("Result: ");
 
        int cnt = 3; /* the ordinal number of the Fibonacci number */
        for(;;)
        {
            int fib = fibonacci(cnt); /* find the next Fibonacci number */
 
            if(fib >= n) break;
 
            if(prime(fib))
            {
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
    
    if(n > 0) /* F(n) = F(n - 2) + F(n - 1) */
    {
        if (n <= 2) return 1;
 
        int n2 = 1; /* F(n - 2) */
        for (i = 3; i <= n; i++)
        {
            fib = n2 + n1;
            n2 = n1;
            n1 = fib;
        }
    }
    else if(n < 0) /* F(n) = F(n + 2) - F(n + 1) */
    {
        if(n == -1) return 1;
 
        int n2 = 0; /* F(n + 2) */
        for (i = -2; i >= n; i--)
        {
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
    while(tmp < pow(n, 0.5) + 1)
    {
        if(n % tmp == 0) /* if the divider is found, the number isn't simple */
        {
            n /= tmp;
            return 0;
        }
        else
        {
            tmp++;
        }
    }
    
    return 1; /* if the divisor isn't found, then the number is prime */
}

int main()
{
    int mass[N];
    for(int i = 0; i < N; i++)
     {
        scanf("%d", &mass[i]);
    }
    remap(mass);
    
    primes(100);
    
    fibprimes(50);
    
    return 0;
}
