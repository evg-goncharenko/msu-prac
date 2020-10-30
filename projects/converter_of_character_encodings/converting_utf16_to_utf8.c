#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define LITTLE_ENDIANS 0xFFFE
#define BIG_ENDIANS 0xFEFF

#define INPUT_ERROR 1
#define OUTPUT_ERROR 2

#define IS_LITTLE 0
#define IS_BIG 1

int main(int argc, char **argv)
{
    FILE *file_in;
    FILE *file_out;

    char utf8_symb1, utf8_symb2, utf8_symb3, bom;
    int pair_bytes = 2;
    unsigned short utf16_symb;

    /* Initially, we initialize input / output as standard: */
    file_in = stdin;
    file_out = stdout;

    if(argc > 1)
    {
        if((file_in = fopen(argv[1], "r")) == NULL)
        {
            perror(argv[1]);
            exit(INPUT_ERROR);
        }

        /* Open BOM output file for reading: */
        if(argc > 2)
        {
            if((file_out = fopen(argv[2], "r")) == NULL)
            {
                /* If there isn't output file - create and make a warning: */
                perror(argv[2]);
            }
        }
    }

    pair_bytes = fread(&utf16_symb, sizeof(char), 2, file_in);

    /*
        The inversion of endian conditions is due
        to the inversion operation of fread/fwrite
    */
    if(utf16_symb == LITTLE_ENDIANS)
    {
        bom = IS_BIG;
    }
    else if(utf16_symb == BIG_ENDIANS)
    {
        bom = IS_LITTLE;
    }
    else
    {
        bom = IS_LITTLE;
        fseek(file_in, 0, SEEK_SET);
        fprintf(stderr, "BOM isn't found\n");
    }
    
    /* If it isn't stdout: */
    if(argc > 2)
    {
        unsigned char tmp_symb1 = 0;
        unsigned char tmp_symb2 = 0;
        unsigned char tmp_symb3 = 0;
        
        /*
             Reading presumably the BOM:
             We close the file and open it for writing to completely clear the contents.
             If we counted the BOM, we should write it at the beginning of the file.
        */
        if(file_out)
        {
            fread(&tmp_symb1, 1, 1, file_out);
            fread(&tmp_symb2, 1, 1, file_out);
            fread(&tmp_symb3, 1, 1, file_out);
            fclose(file_out);
        }
        
        if((file_out = fopen(argv[2], "w")) == NULL)
        {
            perror(argv[2]);
            exit(OUTPUT_ERROR);
        }
        
        /* If it is a BOM: */
        if((tmp_symb1 == 0xEF) && (tmp_symb2 == 0xBB) && (tmp_symb3 == 0xBF))
        {
            fwrite(&tmp_symb1, 1, 1, file_out);
            fwrite(&tmp_symb2, 1, 1, file_out);
            fwrite(&tmp_symb3, 1, 1, file_out);
        }
    }
    
    pair_bytes = fread(&utf16_symb, sizeof(char), 2, file_in);
    
    /*
     **********************************************************
     *  Number of octets |             Template               |
     *          1        |     0xxxxxxx                       |
     *          2        |     110xxxxx 10xxxxxx              |
     *          3        |     1110xxxx 10xxxxxx 10xxxxxx     |
     **********************************************************
    */
    
    while(pair_bytes == 2)
    {
        if(bom == IS_BIG)
        {
            utf8_symb1 = (char)(utf16_symb >> 8);
            utf16_symb = utf16_symb << 8;
            utf16_symb += utf8_symb1;
        }

        if(utf16_symb < 128)
        {
            utf8_symb1 = (char)utf16_symb;
            fwrite(&utf8_symb1, 1, 1, file_out);
        }
        else if(utf16_symb < 2048)
        {
            utf8_symb2 = (char)(utf16_symb & 0x3F);
            utf8_symb2 = utf8_symb2 + 128;
            utf16_symb = utf16_symb >> 6;

            utf8_symb1 = (char)(utf16_symb);
            utf8_symb1 = utf8_symb1 + 128 + 64;
            
            fwrite(&utf8_symb1, 1, 1, file_out);
            fwrite(&utf8_symb2, 1, 1, file_out);
        }
        else
        {
            utf8_symb3 = (char)(utf16_symb & 0x3F);
            utf8_symb3 = utf8_symb3 + 128;
            utf16_symb = utf16_symb >> 6;

            utf8_symb2 = (char)(utf16_symb & 0x3F);
            utf8_symb2 = utf8_symb2 + 128;
            utf16_symb = utf16_symb >> 6;

            utf8_symb1 = (char)(utf16_symb);
            utf8_symb1 = utf8_symb1 + 128 + 64 + 32;
            
            fwrite(&utf8_symb1, 1, 1, file_out);
            fwrite(&utf8_symb2, 1, 1, file_out);
            fwrite(&utf8_symb3, 1, 1, file_out);
        }
        pair_bytes = fread(&utf16_symb, sizeof(char), 2, file_in);
    }
    
    if(pair_bytes == 1) fprintf(stderr, "Odd number of bytes\n");

    fclose(file_in);
    fclose(file_out);
    
    return 0;
}
