## Development of UTF16 - UTF8 encoding converter programs

The first program reads UTF-16 encoded text from the input channel, converts it to UTF-8 encoding, and outputs the recoded text to the output channel ([converting_utf16_to_utf8.c](./converting_utf16_to_utf8.c)). The second program reads UTF-8 encoded text from the input channel, converts it to UTF-16 encoding, and outputs the transcoded text to the output channel ([converting_utf8_to_utf16.c](./converting_utf8_to_utf16.c)). <br><br>
If the command — line argument-the file name-is specified at program startup, then the text is taken from this file instead of the standard input. If two arguments are given, the first is treated as the input file name and the second as the output file name. Standard I/O streams are not used in this case. If the file name is incorrect, the program returns an error message and terminates. <br>
Both programs correctly process the byte order mark (BOM) - the character with the code `0xFEFF` - at the beginning of the file. The first program reads the text in UTF-16, and the second program generates the text in UTF-16 according to the "read" marker. If there is no marker, the LE-order is assumed (that is, by default, the BOM is represented by the bytes `0xFF 0xFE` at the beginning of the file). <br><br>
The programs also handle cases of incorrect representation of the input text — an odd number of bytes in UTF-16, incorrect sequences in UTF-8. In these cases, programs issue diagnostics to the standard error message output channel (stderr), which includes the value of an incorrect character (sequence), as well as its offset relative to the beginning of the file. After that, the programs are "restored" and continue reading and recoding the input sequence.

---

The unsigned short data type is used to store UTF-16 characters, and the char data type is used for UTF-8 characters.

The binary image of the file (byte-by-byte) can be viewed by the od (octal dump) program. `od le. utf` will display the contents of the `le.utf` file in 2-byte chunks in octal format. To output a file in 16-bit format, the following set of options is used:
```
od -A x -t x1z -v le.utf
```
In octal byte format:
```
od -c le.utf
```


If the system is set to UTF-8 encoding (which is true for many installations), then the file in UTF-8 encoding can simply be displayed with the command  `cat file_name` or viewed in any text editor.
To view the contents of a file in any encoding, you can use the vim editor. After opening the file with the `vim file_name` command, the file encoding is specified with the command:

```
:set fileencoding=ucs-2
```
Note that the ucs-2 encoding uses direct byte order (big-endian). When reading a file with reverse byte order (little-endian), use the name `ucs-2le`. Instead of `ucs-2` and `ucs-2le`, you can use the names `utf-16` and `utf-16le`. The differences in these encodings do not appear in the test files.

The `xxd` command creates a representation of the file as hexadecimal codes or performs the reverse conversion. <br>
Example:
```
xxd tmp.utf
```

The `hexedit` command views and edits files in hexadecimal or ASCII format. Usage Example:
```
hexedit tmp.utf
```

The `diff` command looks for differences between two files. The `-s` flag indicates that the two files are the same. <br>
Example:
```
diff ./tests/letext.ucs tmp.ucs -s
```

### Test files

For testing and debugging, the following (minimal) set of files is written, which is located in the `/tests` directory.

<b> UTF-16 files </b>

    - letext.ucs - text in UTF-16 in an inverted view (LE-order) with a BOM
    - betext.ucs - text in UTF-16 live view (BE-order) with a BOM
    - letextbad1.ucs - text in UTF-16 in an inverted view (LE-right) without
    tags BOM
    - betextbad1.ucs - text in UTF-16 in direct representation (BE-order)
    without a label BOM
    - letextbad2.ucs - text in UTF-16 in an inverted view (LE-order) labeled
    BOM, but with the wrong character (BE-order)
    - betextbad2.ucs - text in UTF-16 live view (BE-order) with a BOM,
    but with the wrong character (BE-order)
    - leempty.ucs – empty text in UTF-16 in an inverted view (LE-order) with BOM
    - beempty.ucs – empty text in UTF-16 live view (BE-order) labelled BOM
    - le30.ucs – a single character (code=0x30 symbol 0) the text in UTF-16
    in an inverted the performance (LE-order) with a BOM
    - be30.ucs – a single character (code=0x30 symbol 0) the text in UTF-16 live
    performance (BE-order) with a BOM
    - le42f.ucs – a single character (code=0x042F – symbol I) text to UTF-16
    in an inverted the performance (LE-right) with a BOM
    - be42f.ucs – a single character (code=0x042F – symbol I) the text in UTF-16
    live performance (BE-order) with a BOM
    - le263A.ucs – a single character (code=0x263A symbol ☺) text to UTF-16
    the inverted representation (LE-order) with a BOM
    - be262A.ucs – a single character (code=0x263A symbol ☺) text in UTF-16
    live performance (BE-order) with a BOM
    
<b> UTF-8 files </b>

    - text.utf - text in UTF-8 with the BOM label (encoded)
    - text2.utf - text in UTF-8 without the BOM label
    - textbad1.utf - text in UTF-8 with the wrong sequence (starts with a byte
    continuation) without the BOM label
    - textbad2.utf - text in UTF-8 with the wrong sequence (missing byte
    continuation) without the label BOM
    - empty.utf - empty text in UTF-8 with the label BOM
    - 30.utf - single-character (code=0x30 – character 0) text in UTF-8
    - 42f.utf - single-character (code=0x042F - character I) text in UTF-8
    - 263A.utf - single-character (code=0x263A - character ☺) text in UTF-8
