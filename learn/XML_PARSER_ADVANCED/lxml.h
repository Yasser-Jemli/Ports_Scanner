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
void XMLNode_free(XMLNode* node);
int XMLDocument_load(XMLDocument *doc, const char *path);
void XMLDocument_free(XMLDocument *doc);

XMLNode* XMLNode_new(XMLNode* parent){
    XMLNode* node = (XMLNode*) malloc(sizeof(XMLNode));
    node->parent = parent;
    node->tag = NULL;
    node->inner_text = NULL;
    return node;
}

void XMLNode_free(XMLNode* node){
    if(node->tag) free(node->tag);
    if(node->inner_text) free(node->inner_text);
    free(node);
}

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

    char lex[256];
    int lexi = 0;
    int i = 0;

    XMLNode* curr_node = NULL ;
    return TRUE;
}


void XMLDocument_free(XMLDocument *doc){
    XMLNode_free(doc->root);
}

#endif