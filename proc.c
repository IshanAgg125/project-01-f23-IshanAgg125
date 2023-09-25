struct proc {
    int pid;
    char state; //-s
    char command[200];
    unsigned int utime;
    unsigned int vtime; //-v

} proc_data, *proc_data_ptr;
