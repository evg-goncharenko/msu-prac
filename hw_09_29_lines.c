#include <stdio.h>
#include <string.h>

/* Приписывает t к s, возвращает s */
char* sstrcat(char *s, char *t)
{
    char *res = s;
    while(*s) { s++; }
    while((*s++ = *t++)) { }
    *s = 0;
    return res;
}

/* Выдает 1, если строка t расположена в конце строки s, и 0 иначе */
int sstrend(char *s, char *t)
{
    for(long pos = strlen(s) - strlen(t), i = 0; pos < strlen(s); pos++, i++){
        if(s[pos] != t[i]) return 0;
    }
    return (strlen(t) <= strlen(s)) ;
}

/*
    Копирует не более n символов строки t в s, возвращает s.
    Дополняет результат символами '\0', если символов в t меньше n
*/
char* sstrncpy(char *t, char *s, int n)
{
    char *tmp = s;
    while((*tmp++ = *t++) && (n > 1)) { n--; }
    *tmp = 0;
    return s;
}

/*
    Приписывает не более n символов t к s, завершая s символом '\0'.
    Возвращает s.
*/
char* sstrncat(char *s, char *t, int n)
{
    char *res = s;
    while(*s) { s++; }
    while((n > 0) && (*s++ = *t++)) { n--; }
    *s = 0;
    return res;
}

/*
    Cравнивает не более n символов cs и ct.
    Возвращает < 0, если cs < ct, 0, если cs == ct, и > 0, если cs > ct.
*/
int sstrncmp(char *cs, char *ct, int n)
{
    for(; (n > 1) && (*cs == *ct); cs++, ct++, n--){
        if(*cs == 0) return 0; /* когда обе строки пустые */
    }
    return (*cs - *ct);
}

int main()
{
    
    return 0;
}
