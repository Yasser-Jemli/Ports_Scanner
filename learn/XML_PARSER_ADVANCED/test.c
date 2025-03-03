#include "lxml.h"

int main (){
    
    XMLDocument doc;
    
    if (XMLDocument_load(&doc, "test.xml")){
        printf("%s\n", doc.source);
        XMLDocument_free(&doc);
    }
    
    return 0;
}