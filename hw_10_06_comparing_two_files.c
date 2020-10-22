/*
    The program compares two files and prints the first line in which they differ
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv)
{
    /*
        We need to store two arrays of strings str_tmp1 and str_tmp2 files file1 and file2
        To cover all options. And also to remember the dynamic size.
    */
    
    FILE *file1;
    FILE *file2;
    int tmp_c1 = 0;
    int tmp_c2 = 0;
    
    char *str_tmp1 = NULL;
    char *str_tmp2 = NULL;
    int str_size1 = 1;
    int str_size2 = 1;
    int str_index1 = 0;
    int str_index2 = 0;
    
    if(argc < 3)
    {
        fprintf(stderr, "Too few arguments command lines\n");
        return 1;
    }
    
    if((file1 = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "File '%s' not found\n", argv[1]);
        return 1;
    }
    
    if((file2 = fopen(argv[2], "r")) == NULL)
    {
        fprintf(stderr, "File '%s' not found\n", argv[2]);
        return 1;
    }
    
    while(((tmp_c1 = fgetc(file1)) != EOF) && ((tmp_c2 = fgetc(file2)) != EOF))
    {
        while((tmp_c1 != '\n') && (tmp_c1 != EOF))
        {
            if((str_index1 + 1) == str_size1)
            {
                str_size1 *= 2;
                str_tmp1 = realloc(str_tmp1, str_size1);
            }
            
            str_tmp1[str_index1] = tmp_c1;
            str_index1++;
            tmp_c1 = getc(file1);
        }
        
        while((tmp_c2 != '\n') && (tmp_c2 != EOF))
        {
            if((str_index2 + 1) == str_size2)
            {
                str_size2 *= 2;
                str_tmp2 = realloc(str_tmp2, str_size2);
            }
            
            str_tmp2[str_index2] = tmp_c2;
            str_index2++;
            tmp_c2 = getc(file2);
        }
        
        if(str_tmp1 || str_tmp2)
        {
            /* Checking for line differences between two files: */
            int flag = 0;
            
            /* Don't forget to set the end of line flag */
            if(str_tmp1) str_tmp1[str_index1] = 0;
            if(str_tmp2) str_tmp2[str_index2] = 0;
            
            if(str_tmp1 && !str_tmp2)
            {
                flag = 1;
                printf("Different line of the first file: '%s'\n", str_tmp1);
            }
            else if(!str_tmp1 && str_tmp2)
            {
                flag = 1;
                printf("Different line of the second file: '%s'\n", str_tmp2);
            }
            else if(strcmp(str_tmp1, str_tmp2))
            {
                flag = 1;
                printf("The first different line:\n");
                printf("The first file: '%s'\n", str_tmp1);
                printf("The second file: '%s'\n", str_tmp2);
            }
            
            if(str_tmp1)
            {
                free(str_tmp1);
                str_tmp1 = NULL;
                str_index1 = 0;
                str_size1 = 1;
            }
            
            if(str_tmp2)
            {
                free(str_tmp2);
                str_tmp2 = NULL;
                str_index2 = 0;
                str_size2 = 1;
            }
            
            if(flag) break;
        }
    }
    
    fclose(file1);
    fclose(file2);
    return 0;
}
