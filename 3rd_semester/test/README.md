## Test No. 2
<b> [task_1](./task1.c): </b><br>

    The program simulates the SHELL command: pr1 < f.dat; pr2 | pr3
    (here pr_i – process names, f.dat - input data file).
    The arguments required by this program are set on the command line:
    ./prog pr1 pr2 pr3 f.dat
    
<br> <b> [task_2](./task2.c): </b><br>

    The command line specifies the file name. Leave in the file only those lines
    of the specified file that contain at least two identical characters.
    No additional memory for storing strings and additional files can be used.
    Only low-level I/O system calls are allowed to work with the file.
    To "trim" the file to the desired length,
    use the truncate() or ftruncate() system calls.
    
<br> <b> [task_3](./task3.c): </b><br>
    
    The program spawns a child process.
    The father process opens a file (its name is set on the command line)
    that contains the names of other files, one per line,
    and passes these names to the child process through an unnamed channel.
    The child process opens the received file, prints its name, determines
    the length of the file, closes it, and passes the result to the father
    through the SAME channel. The father prints the length of the corresponding
    file and the line feed. When the source file ends, the father closes
    the channel and waits for the child process to finish, and the child process,
    after receiving the signal, prints the number of processed files and terminates.
    Synchronization of processes (when outputting to a standard channel
    and working with an unnamed channel) is carried out using signals.
