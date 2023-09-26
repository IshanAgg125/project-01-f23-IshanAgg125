#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

bool containsProcessInformation(char *argument);
void stateInformationUserTimeAndSystemTime(char *PID);
char *commandline(char *PID);


struct proc {
    int pid;
    char state; //-s
    char command[200];
    unsigned int utime;
    unsigned int vtime; //-v
};

int main(int argc, char **argv) {
    struct proc myProc;
    int locationOfPID = 0;
    int containsUtime = 0;
    int containsCommandLine = 0;
    printf("Number of arguments = %d\n", argc);
    for (int i = 1; i < argc; i++) {
        printf("Argument %d = %s\n", i, *(argv+i));
        if (containsProcessInformation(argv[i])) {
            locationOfPID = i+1;
            break;
        }
    }
    printf("The location of PID %d\n", locationOfPID);
    for (int i = 1; i < argc; i++) {
        //printf("%s\n", argv[i]);
        if (strcmp(argv[i], "-s") == 0) {
            printf("%s is at location %d\n", "Entering the -s", i);
            //stateInformation(argv[locationOfPID]);
        } else if (strcmp(argv[i], "-S") == 0) {
            printf("%s is at location %d\n", "Entering the -S", i);
            //systemTimeConsumed(argv[locationOfPID]);
        } else if (strcmp(argv[i], "-v") == 0) {
            printf("%s is at location %d\n", "Entering the -v", i);
            //virtualMemory(argv[i]);
        } else if (strcmp(argv[i], "-U") == 0) {
            containsUtime = 1;
        } else if (strcmp(argv[i], "-c") == 0) {
            containsCommandLine = 1;
        } else {
            perror("Invalid input");
        }
    }

    if (!containsUtime) {
        printf("%s\n", "Entering Utime");
        //userTime(argv[locationOfPID]);
    }
    if (!containsCommandLine) {
        printf("%s\n", "Entering command line");
        char *result = commandline(argv[locationOfPID]);
        strncpy(myProc.command, result, sizeof(myProc.command));
    }
    myProc.pid = argv[locationOfPID];
    printf("Everything is working and struct command is %s\n", myProc.command);

    //else if (containsUserTime(argv)) {
    //     userTime(argv[locationOfPID]);
    // }
   

    //commandline(argv[2]);

}

bool containsProcessInformation(char *argument) {
    if (strcmp(argument, "-p") == 0) {
        return true;
    } 
    return false;
}

char *commandline(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int size = 0;
    char *toReturn = NULL;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/cmdline", PID);
    printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    while ((text = fgetc(file)) != EOF) {
        char *temp = realloc(toReturn, size + 5);
        if (temp == NULL) {
            perror("Error reallocating memory");
            free(toReturn);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        toReturn = temp;
        toReturn[size++] = (char)text;
        toReturn[size] = '\0';
    }
    fclose(file);
    return toReturn;
}

void stateInformationUserTimeAndSystemTime(char *PID) {
    printf("%s\n", "Entering the function");
    FILE *file;
    char pathName[200];
    int text;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    while ((text = fgetc(file)) != EOF) {
        printf("%c", text);
    }
    printf("%s\n", "Finished the function");
    printf("\n");
    fclose(file);
}
