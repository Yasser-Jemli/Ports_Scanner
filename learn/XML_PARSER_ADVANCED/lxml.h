#ifndef LITTLEXML_H
#define LITTLEXML_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef TRUE 
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

struct _XMLDocument{
    char* source;
};

typedef struct _XMLDocument XMLDocument;

int XMLDocument_load(XMLDocument *doc, const char *path);
void XMLDocument_free(XMLDocument *doc);


int XMLDocument_load(XMLDocument* doc, const char *path){
    FILE* file = fopen(path,"r");
    if(!file){
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return FALSE ; 
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);


    doc->source = (char*) malloc(sizeof(char) * size + 1);
    fread(doc->source ,1 , size, file);
    fclose(file);
    doc->source[size] = '\0';

    return TRUE;
}


void XMLDocument_free(XMLDocument *doc){
    free(doc->source);
}


#endif