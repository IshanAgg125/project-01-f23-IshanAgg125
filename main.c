#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

struct proc {
    int pid;
    char state; //-s
    char command[200];
    unsigned int utime;
    unsigned int stime; //-S
    unsigned int vMemory;
};

bool containsProcessInformation(char *argument);
char *commandline(char *PID);
char stateInformation(char *PID);
int userTime(char *PID);
int systemTime(char *PID);
int virtualMemory(char *PID);
void printElementsOfStruct(struct proc myProc);




int main(int argc, char **argv) {
    struct proc myProc;
    int locationOfPID = 0;
    int containsUtime = 0;
    int containsCommandLine = 0;
    //printf("Number of arguments = %d\n", argc);
    for (int i = 1; i < argc; i++) {
        //printf("Argument %d = %s\n", i, *(argv+i));
        if (containsProcessInformation(argv[i])) {
            locationOfPID = i+1;
            break;
        }
    }
    myProc.pid = atoi(argv[locationOfPID]);
    //printf("The location of PID %d\n", locationOfPID);
    for (int i = 1; i < argc; i++) {
        //printf("%s\n", argv[i]);
        if (strcmp(argv[i], "-s") == 0) {
            //printf("%s is at location %d\n", "Entering the -s", i);
            char result = stateInformation(argv[locationOfPID]);
            myProc.state = result;
        } else if (strcmp(argv[i], "-S") == 0) {
            //printf("%s is at location %d\n", "Entering the -S", i); //15
            int result = systemTime(argv[locationOfPID]);
            myProc.stime = result;
        } else if (strcmp(argv[i], "-v") == 0) {
            //printf("%s is at location %d\n", "Entering the -v", i);
            int result = virtualMemory(argv[locationOfPID]);
            myProc.vMemory = result;
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
        //printf("%s\n", "Entering Utime"); //14th field
        int result = userTime(argv[locationOfPID]);
        myProc.utime = result;
    }
    if (!containsCommandLine) {
        //printf("%s\n", "Entering command line");
        char *result = commandline(argv[locationOfPID]);
        strncpy(myProc.command, result, sizeof(myProc.command));
    }

    printElementsOfStruct(myProc);


}

bool containsProcessInformation(char *argument) {
    if (strcmp(argument, "-p") == 0) {
        return true;
    } 
    return false;
}
int virtualMemory(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/statm", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        printf("%c", text);
         if (text == ' ') {
            count++;
            continue;
        }
        if(count == 1) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

char stateInformation(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    char processState = '\0';
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    //printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 3) {
            processState = (char)text;
        }
    }
    fclose(file);
    return processState;
}

int userTime(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 14) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

int systemTime(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int result = 0;
    //char toReturn;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        if (text == ' ') {
            count++;
            continue;
        }
        if(count == 15) {
            result = result * 10 + (text - '0'); //return '5' - '0' = 0 * 10 + 5 = 5
        }
    }
    fclose(file);
    return result;
}

char *commandline(char *PID) {
    FILE *file;
    char pathName[200];
    int text;
    int size = 0;
    char *toReturn = NULL;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/cmdline", PID);
    //printf("%s\n", pathName);
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
        toReturn[size] = (char)text;
        size++;
        toReturn[size] = '\0';
    }
    fclose(file);
    return toReturn;
}

void printElementsOfStruct(struct proc myProc) {
    printf("%s", "Output: ");
    if (myProc.pid != 0) {
        printf("%d: ", myProc.pid);
    }
    if (myProc.state != 0) { 
        printf("%c ", myProc.state);
    }
    if (myProc.utime != 0) { 
        printf("utime=%d ", myProc.utime);
    }
    if (myProc.stime != 0) {
        printf("stime=%d ", myProc.stime);
    }
    if (myProc.vMemory != 0) {
        printf("%d ", myProc.vMemory);
    }
    if (myProc.command[0] != '\0') {
        printf("%s\n", myProc.command);
    }
    // printf("Struct PID is %d\n", myProc.pid);
    // printf("struct command is %s\n", myProc.command);
    // printf("Struct state is %c\n", myProc.state);
    // printf("Struct user time is %d\n", myProc.utime);
    // printf("Struct System time is %d\n", myProc.stime);
    // printf("Struct virtual memmory is %d\n", myProc.vMemory);
    
}

