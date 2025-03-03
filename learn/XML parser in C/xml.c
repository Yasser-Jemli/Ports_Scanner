#include <stdio.h>
#include <stdlib.h>


#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>


char *read_file_ascii(const char* path){
    FILE *file = fopen(path, "r");

    if (file == NULL){
        fprintf(stderr , "Error: Could not open file %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buf = (char*)malloc(sizeof(char) * (size + 1));
    fread(buf, sizeof(char), size, file);

    buf[size] = '\0';
    fclose(file);
    return buf;
}

int main (int argc , char *argv[]){
    char *source = read_file_ascii("test/first.xml");
    printf("%s\n", source);
    free(source);
    return 0 ;
}