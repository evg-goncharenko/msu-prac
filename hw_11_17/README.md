## Homework by 17.11

<b> [task_1](./task_1.c): </b><br>
    
    The program simulates the SHELL command:
    (here pr_i – process names, arg_j-process arguments, f.dat - input data file, f.res - results file).
    The arguments required by this program are set on the command line:
        a) pr1 arg1 arg2 | pr2; pr3 >> f.res
        b) pr1 < f.dat | pr2 > f.res; pr3
        c) pr1 | pr2 | ... | pr_n
        d) pr1 arg1 > f.res; pr2 | pr3 | pr4 >> f.res

<br> <b> [task_2](./task_2.c): </b><br>
    
    The program counts the received SIGTRAP signals between the 2nd and 4th pressing of CTRL+C.
    Exits on the 7th press of CTRL+C.
