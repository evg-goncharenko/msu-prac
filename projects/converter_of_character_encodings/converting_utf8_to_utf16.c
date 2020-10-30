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
#define NO_BOM 2

int main(int argc, char **argv)
{
    FILE *file_in;
    FILE *file_out;

    unsigned char bom = IS_LITTLE, correct_output = 0;
    unsigned short bom_out = 0; /* output file BOM */
    unsigned short utf16_symb;
    unsigned char utf8_symb;
    
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
    
    unsigned char tmp_byte1 = 0;
    unsigned char tmp_byte2 = 0;
    unsigned char tmp_byte3 = 0;
    
    fread(&tmp_byte1, 1, 1, file_in);
    fread(&tmp_byte2, 1, 1, file_in);
    fread(&tmp_byte3, 1, 1, file_in);
    
    /* Shift to the beginning of the file, if it isn't a BOM: */
    if(!((tmp_byte1 == 0xEF) && (tmp_byte2 == 0xBB) && (tmp_byte3 == 0xBF)))
    {
        fseek(file_in, 0, SEEK_SET);
    }
    
    /* If it isn't stdout: */
    if(argc > 2)
    {
        /*
             Reading presumably the BOM:
             We close the file and open it for writing to completely clear the contents.
             If we counted the BOM, we should write it at the beginning of the file.
        */
        if(file_out)
        {
            fread(&bom_out, 2, 1, file_out);
            fclose(file_out);
        }
        
        /*
            The inversion of endian conditions is due
            to the inversion operation of fread/fwrite
        */
        if(bom_out == BIG_ENDIANS)
        {
            bom = IS_LITTLE;
        }
        else if(bom_out == LITTLE_ENDIANS)
        {
            bom = IS_BIG;
        }
        else
        {
            bom = NO_BOM;
        }
        
        if((file_out = fopen(argv[2], "w")) == NULL)
        {
            perror(argv[2]);
            exit(OUTPUT_ERROR);
        }
        if(bom != NO_BOM)
        {
            fwrite(&bom_out, 2, 1, file_out);
        }
    }
    
    fread(&utf8_symb, 1, 1, file_in);
    
    /*
     **********************************************************
     *  Number of octets |             Template               |
     *          1        |     0xxxxxxx                       |
     *          2        |     110xxxxx 10xxxxxx              |
     *          3        |     1110xxxx 10xxxxxx 10xxxxxx     |
     **********************************************************
    */
    while(!feof(file_in))
    {
        utf16_symb = 0;
        
        /* 1 symbol is 1 byte: */
        if((utf8_symb >> 7) == 0)
        {
            utf16_symb += utf8_symb;
            correct_output = 1;
        }
        /* 1 symbol is 2 bytes: */
        else if((utf8_symb >> 5) == 0x6)
        {
            utf8_symb = utf8_symb & 0x1F;
            utf16_symb += utf8_symb;

            utf16_symb = utf16_symb << 6;
            
            fread(&utf8_symb, sizeof(char), 1, file_in);

            if((utf8_symb >> 6) == 2)
            {
                utf8_symb = utf8_symb & 0x3F;
                utf16_symb += utf8_symb;
                correct_output = 1;
            }
            else
            {
                fprintf(stderr, "The second byte is incorrect in a two byte sequence, position: %ld\n", ftell(file_in));
            }

        }
        /* 1 symbol is 3 bytes: */
        else if((utf8_symb >> 4) == 0xE)
        {
            utf8_symb = utf8_symb & 0xF;
            utf16_symb += utf8_symb;
            utf16_symb = utf16_symb << 6;

            fread(&utf8_symb, sizeof(char), 1, file_in);

            if((utf8_symb >> 6) == 2)
            {
                utf8_symb = utf8_symb & 0x3F;
                utf16_symb += utf8_symb;
                utf16_symb = utf16_symb << 6;

                fread(&utf8_symb, sizeof(char), 1, file_in);

                if((utf8_symb >> 6) == 2)
                {
                    utf8_symb = utf8_symb & 0x3F;
                    utf16_symb += utf8_symb;
                    correct_output = 1;
                }
                else
                {
                    fprintf(stderr, "The third byte is incorrect in a three byte sequence, position: %ld\n", ftell(file_in));
                }
            }
            else
            {
                fprintf(stderr, "The second byte is incorrect in a three byte sequence, position: %ld\n", ftell(file_in));
            }
        }
        else
        {
            fprintf(stderr, "Byte is incorrect, position: %ld\n", ftell(file_in));
        }

        if(correct_output)
        {
            if(bom == IS_BIG)
            {
                utf8_symb = (char)(utf16_symb >> 8);
                fwrite(&utf8_symb, sizeof(utf8_symb), 1, file_out);
                utf8_symb = (char)(utf16_symb);
                fwrite(&utf8_symb, sizeof(utf8_symb), 1, file_out);
            }
            else
            {
                fwrite(&utf16_symb, 2, 1, file_out);
            }
            correct_output = 0;
        }
        fread(&utf8_symb, sizeof(char), 1, file_in);
    }

    fclose(file_in);
    fclose(file_out);
    
    return 0;
}
