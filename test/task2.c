#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

int count = 0;

int main(int argc, char **argv)
{
    int file, file2;
    
    if((file = open(argv[1], O_RDONLY)) == -1)
    {
        perror(argv[1]);
        exit(1);
    }
    
    if((file2 = open(argv[1], O_WRONLY)) == -1)
    {
        perror(argv[1]);
        exit(1);
    }
    
    int tmp_c;
    int flag = 0;
    
    while((read(file, &tmp_c, sizeof(char))) > 0)
    {
        
        if(tmp_c == '\n')
        {
            if(flag)
            {
                continue;
            }
            flag = 1;
        }
        else
        {
            flag = 0;
        }
        count++;
        write(file2, &tmp_c, sizeof(char));
    }
    
    count++;
    tmp_c = '\0';
    write(file2, &tmp_c, sizeof(char));
    ftruncate(file2, count);
    
    close(file);
    close(file2);
    
    return 0;
}
