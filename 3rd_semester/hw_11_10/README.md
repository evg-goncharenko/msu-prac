## Homework by 10.11
<b> [task_1](./task_1.c): </b><br>

    The program simulates the SHELL command pr1 || pr2
    (execute pr1; in the case of a failure to comply with pr1 to pr2, or to complete the work).
    The process names are set on the command line.
    A failed termination is either a failure from the OS's point of view or a failure from the programmer's point of view.
<br> <b> [task_2](./task_2.c): </b><br>

    The program simulates the SHELL command pr1 & & pr2
    (execute pr1; if successful, execute pr1, otherwise terminate).
    The process names are specified by command-line arguments.
<br> <b> [task_3](./task_3.c): </b><br>
    
    The program simulates the SHELL command: pr1; pr2; ...; pr_n
    (run pr1, wait for it to finish (in any way), then run pr2, and so on).
    The process names are specified by command-line arguments.
<br> <b> [task_4](./task_4.c): </b><br>
    
    The program starts all processes in parallel, waits for them to finish, and outputs the number of processes,
    completed normally from the point of view of the programmer, normally from the point of view of the OS,
    but not the programmer, and the number of processes that failed
    (the sum of these 3 numbers should be equal to the number of running processes).
<br> <b> [task_5](./task_5.c): </b><br>
    
    The program simulates the SHELL command:
    (here pr_i – process names, arg_j-process arguments, f.dat - input data file, f.res - results file).
    The arguments required by this program are set on the command line:
        a) pr1 | pr2 | pr3
        b) pr1 | pr2 > f.res
        c) pr1 arg11 arg12 < f.dat | pr2 arg21 agr22
        d) pr1 < f.dat > f.res
        e) pr1 < f.dat | pr2 | pr3 > f.res
        f) pr1 | pr2 >> f.res
        g) pr1; pr2 | pr3 > f.res
