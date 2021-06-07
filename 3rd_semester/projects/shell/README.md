## Model of Shell Interpreter

An interactive command interpreter running on Unix OS reads the command line from standard input in a loop, analyzes and executes the corresponding actions. <br>
<br>
The first version of the command interpreter executes a single OS command in a loop. The program reads a string from standard input and splits it into separate words separated by spaces or tabs. Any number of consecutive whitespace characters are treated the same as a single space. <br>
Text enclosed in double quotes is treated as a single word or part of a word, i.e. inside double quotes, whitespace characters are treated as normal characters. For example:

```
vi "text with space"
gcc -o "ccc ddd" t1.c
```

Strings of any length are allowed, meaning, that the program will enter itself correctly regardless of how long the string is submitted for input. <br>
After splitting the string into words, the first received word is perceived as a command (that is, the name of the file with the executable program), the rest - as command parameters – that is, command line arguments. The interpreter runs the specified command with the specified arguments, assuming that the file with the executable code must be located on disk in the directories listed in the PATH variable, or specified by the full name-an absolute or relative path. <br>
<br>

Two commands `cd` for changing the current directory, and `exit` - for exiting the program, are implemented as built-in commands, that is, the program executes them itself, and does not call (non-existent) external commands. <br>
The program exits in the "end of file" situation on standard input or when the user enters the `exit` command. End-of-file processing is implemented correctly. <br>
<br>

Implemented modification of reading the command and arguments so that the characters `<`, `>` or `>>` were perceived as delimiters. Implemented input and output redirection, defined by the characters `<`, `>` or `>>`. And reading the command and arguments in such a way that the `|` character is perceived as a separator. Implemented the execution of commands by the pipeline without restrictions on the length.<br>
<br>

Implemented modification of reading the command and arguments in such a way that it perceives the `&` character (if it is encountered outside the quotation marks) as a separator character (i.e., a character that is a separate word in itself). Implemented the execution of commands in the background. If the `&` character is not found at the end, an error message is displayed. When a command that was executed in the background is completed, a message about its completion and a completion code will be displayed. <br>
<br>

Also implemented:
- A bunch of `;` (first one command is executed, then the second one)
- The `||` bundle (the first command is executed first, and if it fails, the second command is executed)
- The `&&` bundle (the first command is executed first, and if it succeeds, the second command is executed)
- Parentheses (the contents of the parentheses, which have an arbitrary complexity, are executed as a single command). This option can only be selected together with one of the previous three, otherwise it is impossible to check its availability.
