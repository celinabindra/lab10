//Celina Bindra 04/18/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

//Trie node structure
struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

//Trie structure
struct Trie {
    struct TrieNode* root;
};

//Creates new Trie node
struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    newNode->count = 0;
    return newNode;
}

//Inserts word to trie structure
void insert(struct Trie* pTrie, char* word) {
    struct TrieNode* curr = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->count++;
}

//Computes num of occurrences of word
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    struct TrieNode* curr = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (curr->children[index] == NULL) {
            return 0;
        }
        curr = curr->children[index];
    }
    return curr->count;
}

//Deallocates trie node
struct TrieNode* deallocateTrieNode(struct TrieNode* pNode) {
    if (pNode == NULL) {
        return NULL;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->children[i] = deallocateTrieNode(pNode->children[i]);
    }
    free(pNode);
    return NULL;
}

//Deallocates trie structure
struct Trie* deallocateTrie(struct Trie* pTrie) {
    pTrie->root = deallocateTrieNode(pTrie->root);
    free(pTrie);
    return NULL;
}

//Initializes trie structure
struct Trie* createTrie() {
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

//Function reads dictionary file
int readDictionary(char* filename, char** pInWords) {
    FILE* fp;
    int numWords = 0;
    char word[256];

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fscanf(fp, "%d", &numWords); //Reads numb of words from file

    for (int i = 0; i < numWords; ++i) {
        fscanf(fp, "%s", word);
        pInWords[i] = strdup(word);
    }

    fclose(fp);
    return numWords;
}

int main(void) {
    char* inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);

    //Reads dictionary words
    printf("%d\n", numWords);
    for (int i = 0; i < numWords; ++i)
        printf("%s\n", inWords[i]);

    struct Trie* pTrie = createTrie();

    //Inserts dictionary words into trie
    for (int i = 0; i < numWords; i++)
        insert(pTrie, inWords[i]);

    //Tests occurrences of some words
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
        printf("\t%s %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));

    pTrie = deallocateTrie(pTrie);

    if (pTrie != NULL)
        printf("There is an error in this program\n");

    //Deallocates memory for dictionary words
    for (int i = 0; i < numWords; ++i)
        free(inWords[i]);

    return 0;
}