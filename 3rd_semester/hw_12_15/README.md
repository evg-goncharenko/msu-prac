## Homework by 15.12

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [task_1](./task_1.c): </b><br>
```
The command line specifies the file name. Leave in the file only those lines
of the specified file that contain at least two identical characters.
No additional memory for storing strings and additional files can be used.
Only low-level I/O system calls are allowed to work with the file.
To "trim" the file to the desired length,
use the truncate() or ftruncate() system calls.
```
    
<br> <b> [task_2](./task_2.c): </b><br>
```
The program spawns a child process.
The parent process opens a file (its name is set on the command line),
which contains the names of other files — one per line,
and through an unnamed pipe passes these names to the child process.
The child process opens the received file, prints its name,
determines the length of the file, closes it,
and passes the result to the parent via the SAME channel.
The parent prints the length of the corresponding file and the line feed.
When the source file ends, the parent closes the channel and waits for it to finish
the child process, and the child process, after receiving the signal, prints the quantity
processed files and completes. Synchronization of processes
(when outputting to a standard channel and working with an unnamed channel)
is carried out using signals.
```
    
<br> <b> [task_3](./task_3.c): </b><br>
```
In the program, the parent process generates 3 childs.
At the initial moment, all the childs are blocked using the pause () system call.
For each ctrl+C (SIGINT) press, the parent unlocks one childs
in turn in the loop (the others continue to be blocked at this moment).
In the active state, each childs prints its PID once a second.
After 10 seconds, the parent sends all the childs
a SIGKILL signal and completes itself. The "zombie" processes do not remain.
```
