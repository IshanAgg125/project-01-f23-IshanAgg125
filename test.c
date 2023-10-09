#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

//void userIDFromEachProcess(char *PID);
void getDirectoriesInProc(int argc);
char *printProcessInfo(char *PID);

int main(int argc, char **argv) {
   getDirectoriesInProc(argc);
   return 0;
}

char *printProcessInfo(char *PID) {
    int count = 0;
    int size = 10;
    char *toReturn = (char*)malloc(size * sizeof(char));
    if (toReturn == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    FILE *file;
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/status", PID);
    file = fopen(path, "r");
    if (file == NULL) {
        perror("Error in the path");
        exit(EXIT_FAILURE);
    }
    char entireLine[256];
    char uid[256];
    uid_t myUID = getuid();
    while (fgets(entireLine, sizeof(entireLine), file)) {
        if (sscanf(entireLine, "Uid: %s", uid) == 1) {
            printf("MY UID = %d\n", myUID);
            printf("atoi(uid) = %d\n", atoi(uid));
            if (atoi(uid) == myUID) {
                if (count >= size) {
                    size = size * 2;
                    toReturn = (char*)realloc(toReturn, size * sizeof(char));
                    if (toReturn == NULL) {
                        perror("Memory reallocation failed");
                        exit(EXIT_FAILURE);
                    }
                }
                count++;
                // toReturn[count] = char(PID);
                strcat(toReturn, PID);
                printf("This matches the UID %s\n", PID);
            }
        }
    }
    fclose(file);
    for (int i = 0; i < count; i++) {
        printf("Matches UID = %s\n", toReturn);
    }
    return toReturn;
}

void getDirectoriesInProc(int argc) {
    DIR *dirp;
    struct dirent *entryName;
    if (argc != 1) {
        perror("More than one arguments provided");
        exit(EXIT_FAILURE);
    }
    dirp = opendir("/proc");
    if (dirp == NULL) {
        perror("Error opening the directory");
        exit(EXIT_FAILURE);
    }
    while ((entryName = readdir(dirp)) != NULL) {
        if (entryName->d_type == DT_DIR && atoi(entryName->d_name) > 0) {
            // printf("Directory name = %s\n", entryName->d_name);
            printf("EntryName->d_name = %s\n", entryName->d_name);
            printProcessInfo(entryName->d_name);
        }
    }
    closedir(dirp);
}




