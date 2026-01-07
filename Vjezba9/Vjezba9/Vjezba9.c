#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;


Node* insert(Node* root, int x);
int replace(Node* root);
int inorder(Node* root, FILE* f);


int main() {
    Node* root = NULL;
    int i, n = 10;
    int number;

    FILE* f = fopen("stablo.txt", "w");
    if (f == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    srand(time(NULL));

    /* a) generiranje slucajnih brojeva i izgradnja stabla */
    for (i = 0; i < n; i++) {
        number = rand() % 81 + 10;   // raspon <10, 90>
        root = insert(root, number);
    }

    fprintf(f, "Inorder nakon insert funkcije:\n");
    inorder(root, f);

    /* b) zamjena vrijednosti cvorova */
    replace(root);

    fprintf(f, "\nInorder nakon replace funkcije:\n");
    inorder(root, f);

    fclose(f);
    return 0;
}



/* dodavanje elementa u binarno stablo pretrazivanja */
Node* insert(Node* root, int x) {
    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->value = x;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (x < root->value)
        root->left = insert(root->left, x);
    else
        root->right = insert(root->right, x);

    return root;
}

/* zamjenjuje vrijednost cvora sumom elemenata njegovih potomaka */
int replace(Node* root) {
    if (root == NULL)
        return 0;

    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    int oldValue = root->value;
    root->value = leftSum + rightSum;

    return oldValue + leftSum + rightSum;
}

/* inorder prolazak i upis u datoteku */
int inorder(Node* root, FILE* f) {
    if (root == NULL)
        return;

    inorder(root->left, f);
    fprintf(f, "%d ", root->value);
    inorder(root->right, f);

    return 0;
}
