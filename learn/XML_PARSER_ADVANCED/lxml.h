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
    char* root;
};
typedef struct _XMLDocument XMLDocument;

struct _XMLNode 
{
    char* tag ;
    char* inner_text; 
    struct _XMLNode* parent;
};
typedef struct _XMLNode XMLNode;


XMLNode* XMLNode_new(XMLNode* parent);
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


    char* buf = (char*) malloc(sizeof(char) * size + 1);
    fread(buf ,1 , size, file);
    fclose(file);
    buf[size] = '\0';

    doc->root = XMLNode_new(NULL);
    return TRUE;
}


void XMLDocument_free(XMLDocument *doc){
    XMLNode_free(doc->root);
}

#endif