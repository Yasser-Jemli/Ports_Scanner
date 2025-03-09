#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10  // Size of hash table

// Structure for a key-value pair (node in linked list)
typedef struct Node {
    char key[50];  
    int value;  
    struct Node* next;  // Pointer to handle collisions (chaining)
} Node;

// Hash table (array of linked list pointers)
Node* hashTable[TABLE_SIZE] = {NULL};

// Hash function (simple sum of ASCII values modulo table size)
unsigned int hashFunction(char* key) {
    unsigned int hash = 0;
    while (*key)
        hash += *key++;  
    return hash % TABLE_SIZE;
}

// Insert key-value pair into hash table
void insert(char* key, int value) {
    unsigned int index = hashFunction(key);

    // Create a new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->key, key);
    newNode->value = value;
    newNode->next = hashTable[index];  // Insert at head (chaining)
    hashTable[index] = newNode;
}

// Search for a value by key
int search(char* key) {
    unsigned int index = hashFunction(key);
    Node* temp = hashTable[index];

    while (temp) {
        if (strcmp(temp->key, key) == 0)
            return temp->value;  
        temp = temp->next;
    }
    return -1;  // Key not found
}

// Delete a key-value pair
void delete(char* key) {
    unsigned int index = hashFunction(key);
    Node* temp = hashTable[index];
    Node* prev = NULL;

    while (temp) {
        if (strcmp(temp->key, key) == 0) {
            if (prev)
                prev->next = temp->next;
            else
                hashTable[index] = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

// Display hash table
void display() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node* temp = hashTable[i];
        while (temp) {
            printf("[%s -> %d] -> ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

// Main function
int main() {
    insert("Alice", 25);
    insert("Bob", 30);
    insert("Charlie", 35);
    insert("David", 40);
    
    display();
    
    printf("\nSearching for 'Bob': %d\n", search("Bob"));  // Output: 30
    printf("Searching for 'Eve': %d\n", search("Eve"));  // Output: -1 (not found)
    
    delete("Bob");
    printf("\nAfter deleting 'Bob':\n");
    display();

    return 0;
}
