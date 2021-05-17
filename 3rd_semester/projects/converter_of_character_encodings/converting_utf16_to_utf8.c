#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LITTLE_ENDIANS 0xFFFE
#define BIG_ENDIANS 0xFEFF

#define INPUT_ERROR 1
#define OUTPUT_ERROR 2

#define IS_LITTLE 0
#define IS_BIG 1

int main(int argc, char **argv) {
    FILE *file_in;
    FILE *file_out;

    char utf8_symb1, utf8_symb2, utf8_symb3;
    int bom = BIG_ENDIANS;
    int pair_bytes = 2;
    unsigned short utf16_symb;

    /* Initially, we initialize input / output as standard: */
    file_in = stdin;
    file_out = stdout;

    if (argc > 1) {
        if ((file_in = fopen(argv[1], "r")) == NULL) {
            perror(argv[1]);
            exit(INPUT_ERROR);
        }

        if (argc > 2) {
            if ((file_out = fopen(argv[2], "w")) == NULL) {
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
    if (utf16_symb == LITTLE_ENDIANS) {
        bom = IS_BIG;
    } else if (utf16_symb == BIG_ENDIANS) {
        bom = IS_LITTLE;
    } else {
        bom = IS_LITTLE;
        fseek(file_in, 0, SEEK_SET);
        fprintf(stderr, "BOM isn't found\n");
    }

    unsigned char tmp_byte1 = 0xEF;
    unsigned char tmp_byte2 = 0xBB;
    unsigned char tmp_byte3 = 0xBF;

    fwrite(&tmp_byte1, 1, 1, file_out);
    fwrite(&tmp_byte2, 1, 1, file_out);
    fwrite(&tmp_byte3, 1, 1, file_out);

    pair_bytes = fread(&utf16_symb, sizeof(char), 2, file_in);

    /*
     **********************************************************
     *  Number of octets |             Template               |
     *          1        |     0xxxxxxx                       |
     *          2        |     110xxxxx 10xxxxxx              |
     *          3        |     1110xxxx 10xxxxxx 10xxxxxx     |
     **********************************************************
    */

    while (pair_bytes == 2) {
        if (bom == IS_BIG) {
            utf8_symb1 = (char)(utf16_symb >> 8);
            utf16_symb = utf16_symb << 8;
            utf16_symb += utf8_symb1;
        }

        if (utf16_symb < 128) {
            utf8_symb1 = (char)utf16_symb;
            fwrite(&utf8_symb1, 1, 1, file_out);
        } else if (utf16_symb < 2048) {
            utf8_symb2 = (char)(utf16_symb & 0x3F);
            utf8_symb2 = utf8_symb2 + 128;
            utf16_symb = utf16_symb >> 6;

            utf8_symb1 = (char)(utf16_symb);
            utf8_symb1 = utf8_symb1 + 128 + 64;

            fwrite(&utf8_symb1, 1, 1, file_out);
            fwrite(&utf8_symb2, 1, 1, file_out);
        } else {
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

    if (pair_bytes == 1) fprintf(stderr, "Odd number of bytes\n");

    fclose(file_in);
    fclose(file_out);

    return 0;
}
