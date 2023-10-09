#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX_SIZE 100


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
char **getSameUserID();
char *printProcessInfo(char *PID);
struct proc printAllTheInformation(char* PIDpath, int argc, char **argv);



int main(int argc, char **argv) {
    struct proc myProc;
    int locationOfPID = 0;
    char PIDpath[4097];
    bool flag = false;
  
    for (int i = 1; i < argc; i++) {
        //printf("Argument %d = %s\n", i, *(argv+i));
        if (containsProcessInformation(argv[i])) {
            locationOfPID = i+1;
            flag = true;
            break;
        }
    } 
    if (flag) {
        strcpy(PIDpath, argv[locationOfPID]);
        myProc = printAllTheInformation(PIDpath, argc, argv);
        printElementsOfStruct(myProc);
    } else {
        printf("%s\n", "Entering in else");
        char** result = getSameUserID();
        int i = 0;
        for (i = 0; result[i] != NULL; i++) {
            printf("Here are the same user ID %s\n", result[i]);
            myProc = printAllTheInformation(result[i], argc, argv);
            printElementsOfStruct(myProc);
        }
        free(result);
    }
    return 0;
}

bool containsProcessInformation(char *argument) {
    if (strcmp(argument, "-p") == 0) {
        return true;
    } 
    return false;
}

struct proc printAllTheInformation(char* PIDpath, int argc, char **argv) {
    int containsUtime = 0;
    int containsCommandLine = 0;
    int option;
    struct proc myProc;
    myProc.pid = atoi(PIDpath);

   

    while ((option = getopt(argc, argv, "sSUvcp")) != -1) {
        printf("%s %c\n", "option = ", option);
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

    if (!containsCommandLine) {
        char *result = commandline(PIDpath);
        strncpy(myProc.command, result, sizeof(myProc.command));
        free(result);
    }
    return myProc;
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
        //printf("%c", text);
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


char *printProcessInfo(char *PID) {

    FILE *file;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", PID);
    file = fopen(path, "r");
    if (file == NULL) {
        printf("path = %s, PID = %s\n", path, PID);
        perror("Error in the path");
        exit(EXIT_FAILURE);
    }
    char entireLine[256];
    char uid[256];
    uid_t myUID = getuid();
    //printf("MY UID = %d\n", myUID);
    while (fgets(entireLine, sizeof(entireLine), file)) {
        if (sscanf(entireLine, "Uid: %s", uid) == 1) {
            if (atoi(uid) == myUID) {
                fclose(file);
                return PID;
            }
        }
    }
    fclose(file);
    return NULL;
}

char **getSameUserID() {
    DIR *dirp;
    struct dirent *entryName;
    char **toReturn = (char**)malloc(MAX_SIZE * sizeof(char*));
    int i = 0;
    dirp = opendir("/proc");
    if (dirp == NULL) {
        perror("Error opening the directory");
        exit(EXIT_FAILURE);
    }
    while ((entryName = readdir(dirp)) != NULL) {
        if (entryName->d_type == DT_DIR && atoi(entryName->d_name) > 0) {
            // printf("Directory name = %s\n", entryName->d_name);
            char *PID = printProcessInfo(entryName->d_name);
            //printf("Returned PID is = %s\n", PID);
            if (PID != NULL) {
                toReturn[i] = PID;
                i++;
            }
        }
    }
    closedir(dirp);
    toReturn[i] = NULL;
    return toReturn;
}

