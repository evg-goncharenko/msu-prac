#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

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
    
    long pos_last_wr;     /* the file offset indicating the start of writing a new satisfying string */
    long pos_file = 0;    /* the file offset indicating the beginning of the current line */
    int numb_in_line = 0; /* number of characters in the current line */
    char tmp_l, tmp_r;    /* left and right characters to compare for equality in a string */
    int total_numb = 0;   /* total number of characters to write */
    int flag_new_line = 1;
    
    pos_last_wr = lseek(file, 0, SEEK_SET);
    
    while((read(file, &tmp_l, sizeof(char))) > 0)
    {
        int flag = 0; /* flag is true if the string contains at least two identical characters */
        if(tmp_l != '\n')
        {
            if(flag_new_line)
            {
                numb_in_line = 0; /* analysis of a new line */
            }
            int cur_numb_in_line = 1;
            
            /* Go to the end of the line: */
            while((read(file, &tmp_r, sizeof(char))) > 0)
            {
                cur_numb_in_line++;
                
                if(flag_new_line)
                {
                    numb_in_line++;
                }
                
                if(tmp_l == tmp_r)
                {
                    flag = 1;
                }
                
                if(tmp_r == '\n')
                {
                    break;
                }
            }
            flag_new_line = 0;
            if(!flag)
            {
                if(cur_numb_in_line == 2)
                {
                    flag_new_line = 1;
                }
                else if(cur_numb_in_line != 2)
                {
                    lseek(file, -numb_in_line, SEEK_CUR);
                }
                continue;
            }
            
            total_numb += numb_in_line + 1;
            lseek(file, - numb_in_line - 1, SEEK_CUR);
            
            for(int i = 0; i <= numb_in_line; i++)
            {
                read(file, &tmp_l, sizeof(char));
                pos_file = lseek(file, 0, SEEK_CUR);
                
                lseek(file, pos_last_wr, SEEK_SET);
                pos_last_wr += sizeof(char);
                write(file2, &tmp_l, sizeof(char));
                
                lseek(file, pos_file, SEEK_SET);
            }
            flag_new_line = 1;
        }
        else
        {
            flag_new_line = 1;
        }
        
        /* Otherwise, we ignore the string and move on */
    }
    
    ftruncate(file2, total_numb);
    
    close(file);
    close(file2);
    
    return 0;
}
