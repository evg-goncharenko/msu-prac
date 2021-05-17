#include <stdio.h>
#include <string.h>

/*
    Assigns t to s, returns s:
*/
char *sstrcat(char *s, char *t) {
    char *res = s;
    while (*s) {
        s++;
    }
    while ((*s++ = *t++)) {
    }
    *s = 0;
    return res;
}

/*
    Returns 1 if the string t is located at the end of the string s, and 0 otherwise:
*/
int sstrend(char *s, char *t) {
    for (long pos = strlen(s) - strlen(t), i = 0; pos < strlen(s); pos++, i++) {
        if (s[pos] != t[i]) return 0;
    }
    return (strlen(t) <= strlen(s));
}

/*
    Copies no more than n characters of the string t to s and returns s.
    Complements the result with '\0 ' characters if the characters in t are less than n.
*/
char *sstrncpy(char *t, char *s, int n) {
    char *tmp = s;
    while ((*tmp++ = *t++) && (n > 1)) {
        n--;
    }
    *tmp = 0;
    return s;
}

/*
    Assigns no more than n characters t to s, ending s with '\0'.
    Returns s.
*/
char *sstrncat(char *s, char *t, int n) {
    char *res = s;
    while (*s) {
        s++;
    }
    while ((n > 0) && (*s++ = *t++)) {
        n--;
    }
    *s = 0;
    return res;
}

/*
    Compares not more than n characters of cs and ct.
    Returns < 0 if cs < ct, 0 if cs == ct, and > 0 if cs > ct.
*/
int sstrncmp(char *cs, char *ct, int n) {
    for (; (n > 1) && (*cs == *ct); cs++, ct++, n--) {
        if (*cs == 0) return 0; /* when both lines are empty */
    }
    return (*cs - *ct);
}

int main() {
    return 0;
}
