#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main() {
    char *element = "Hello how are you?";
    char *str;
    str = element;
    for (int i = 0; i < strlen(element); i++) {
        printf("%s", str);
    }
    printf("\n");
}


        // printf("loop number = %d\n", i);
        // if (strcmp(entireLine, "") != 0) {
        //     rest = entireLine;
        //     while((token = strtok_r(rest, ":", &rest))) {
        //         if (token != NULL) {
        //             char *trimmedToken = strtok(token, " \t\n\r");
        //             printf("token=%s\n", trimmedToken);
        //             if (strcmp(trimmedToken, "processor") == 0) {
        //                 //printf("%s\n", "Found the processor");
        //                 token = strtok_r(NULL, ":", &rest);
        //                 if (token != NULL) {
        //                     numberOfTheProcessor = atoi(token);
        //                     printf("Number of Porocessor = %d\n", numberOfTheProcessor);
        //                 } else {
        //                     continue;
        //                 }
        //             } else if (strcmp(trimmedToken, "cache") == 0) {
        //                 token = strtok_r(NULL, ":", &rest);
        //                 if (token != NULL) {
        //                     strcpy(cacheSize, token);   
        //                     printf("cache size = %s\n", cacheSize);
        //                 }
        //             }
        //             //printf("Processor %d with a cache size of %s\n", numberOfTheProcessor, cacheSize);
        //             if (numberOfTheProcessor >= 0 && strlen(cacheSize) > 0) {
        //                 printf("Processor %d with a cache size of %s\n", numberOfTheProcessor, cacheSize);
        //                 numberOfTheProcessor = -1;
        //                 cacheSize[0] = '\0';
        //             }
        //         }
        //     }
        // }
        // i++;