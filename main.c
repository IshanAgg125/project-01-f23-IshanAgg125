#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>


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




int main(const int argc, char *const argv[]) {
    struct proc myProc;
    int locationOfPID = 0;
    int containsUtime = 0;
    int containsCommandLine = 0;
    int option;
    char PIDpath[4097];
    //printf("Number of arguments = %d\n", argc);
    for (int i = 1; i < argc; i++) {
        //printf("Argument %d = %s\n", i, *(argv+i));
        if (containsProcessInformation(argv[i])) {
            locationOfPID = i+1;
            break;
        }
    }
    strcpy(PIDpath, argv[locationOfPID]);
    myProc.pid = atoi(PIDpath);
 
    while ((option = getopt(argc, argv, "sSUvcp")) != -1) {
        switch(option) {
            case 's':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                char result = stateInformation(PIDpath);
                //printf("result -s: %c\n", result);
                myProc.state = result;
                break;
            case 'S':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                int resultS = systemTime(PIDpath);
                //printf("result -S: %d\n", resultS);
                myProc.stime = resultS;
                break;
            case 'v':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                int resultV = virtualMemory(PIDpath);
                //printf("result -v: %d\n", resultV);
                myProc.vMemory = resultV;
                break;
            case 'U':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                containsUtime = 1;
                break;
            case 'c':
                //printf("[%d] PID is = %s\n",locationOfPID ,PIDpath);
                containsCommandLine = 1;
                break;
            case 'p':
                continue;
            default:
                fprintf(stderr, "Usage: %s [-p PID] [-s] [-S] [-v] [-U] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    

    if (!containsUtime) {
        int result = userTime(PIDpath);
        myProc.utime = result;
    }
    // printf("%s\n", "printing struct before command line");
    // printElementsOfStruct(myProc);
    // printf("\n");
    if (!containsCommandLine) {
        //printf("%s\n", "Entering command line");
        char *result = commandline(PIDpath);
        strncpy(myProc.command, result, sizeof(myProc.command));
        free(result);
    }

    printElementsOfStruct(myProc);
    return 0;

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
        perror("Virtual memory error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    //printf("%s\n", "statm");
    while ((text = fgetc(file)) != EOF) {
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

    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Stat Information error opening the file");
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

    snprintf(pathName, sizeof(pathName), "/proc/%s/stat", PID);
    file = fopen(pathName, "r");
    //printf("%s\n", pathName);
    if (file == NULL) {
        perror("User time Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
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
    //printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("System time Error opening the file");
        exit(EXIT_FAILURE);
    }
    int count = 1;
    while ((text = fgetc(file)) != EOF) {
        //printf("%c", text);
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
        perror("Command line Error opening the file");
        exit(EXIT_FAILURE);
    }
    while ((text = fgetc(file)) != EOF) {
        printf("%c", text);
        char *temp = (char *)realloc(toReturn, size + 2);
        if (temp == NULL) {
            perror("Error reallocating memory");
            free(toReturn);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        toReturn = temp;
        toReturn[size] = (char)text;
        size++;
    }
    toReturn[size] = '\0';
    fclose(file);
    return toReturn;
}

void printElementsOfStruct(struct proc myProc) {
    printf("%s", "Output: ");
    if (myProc.pid != -1) {
        printf("%d: ", myProc.pid);
    }
    if (myProc.state != '\0') { 
        printf("%c ", myProc.state);
    }
    if (myProc.utime != -1) { 
        printf("utime=%d ", myProc.utime);
    }
    if (myProc.stime != -1) {
        printf("stime=%d ", myProc.stime);
    }
    if (myProc.vMemory != -1) {
        printf("vMemory=%d ", myProc.vMemory);
    }
    if (myProc.command[0] != '\0') {
        printf("%s\n", myProc.command);
    }
    printf("\n");
}

