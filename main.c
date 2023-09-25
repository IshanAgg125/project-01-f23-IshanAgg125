#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

void commandline(char *PID);
void stateInformationUserTimeAndSystemTime(char *PID);

int main(int argc, char **argv) {
    char *arguments[argc];
    printf("Number of arguments = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d = %s\n", i, *(argv+i));
        arguments[i] = argv[i];
    }
    commandline(argv[2]);
    stateInformationUserTimeAndSystemTime(argv[2]);

}

void commandline(char *PID) {
    FILE *file;
    char pathName[200];
    int text;

    // snprintf(path, sizeof(path), "%s/%s", argument, entryName -> d_name);
    snprintf(pathName, sizeof(pathName), "/proc/%s/cmdline", PID);
    printf("%s\n", pathName);
    file = fopen(pathName, "r");
    if (file == NULL) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }
    while ((text = fgetc(file)) != EOF) {
        printf("%c", text);
    }
    printf("\n");
    fclose(file);
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
