// printf("Struct PID is %d\n", myProc.pid);
    // printf("struct command is %s\n", myProc.command);
    // printf("Struct state is %c\n", myProc.state);
    // printf("Struct user time is %d\n", myProc.utime);
    // printf("Struct System time is %d\n", myProc.stime);
    // printf("Struct virtual memmory is %d\n", myProc.vMemory);



// void userIDFromEachProcess(char *PID) {
//     FILE *file;
//     char pathName[200];
//     int text;
//     //char toReturn;

//     // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
//     snprintf(pathName, sizeof(pathName), "/proc/%s/status", PID);
//     file = fopen(pathName, "r");
//     if (file == NULL) {
//         perror("Virtual memory error opening the file");
//         exit(EXIT_FAILURE);
//     }
//     //printf("%s\n", "statm");
//     while ((text = fgetc(file)) != EOF) {
//         printf("%c", text);
//     }
//     fclose(file);
// }
