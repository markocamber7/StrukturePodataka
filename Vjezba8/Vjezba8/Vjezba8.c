#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;



int insert(Node** root, int value);
int find(Node* root, int value);
int inorder(Node* root);
int preorder(Node* root);
int postorder(Node* root);
int levelOrder(Node* root);
int deleteNode(Node** root, int value);
int freeTree(Node* root);


int main() {
    Node* root = NULL;
    int choice, value;

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Unos elementa\n");
        printf("2 - Ispis INORDER\n");
        printf("3 - Ispis PREORDER\n");
        printf("4 - Ispis POSTORDER\n");
        printf("5 - Ispis LEVEL ORDER\n");
        printf("6 - Pronadi element\n");
        printf("7 - Obrisi element\n");
        printf("0 - Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesi broj: ");
            scanf("%d", &value);
            insert(&root, value);
            break;

        case 2:
            inorder(root);
            printf("\n");
            break;

        case 3:
            preorder(root);
            printf("\n");
            break;

        case 4:
            postorder(root);
            printf("\n");
            break;

        case 5:
            levelOrder(root);
            printf("\n");
            break;

        case 6:
            printf("Unesi broj: ");
            scanf("%d", &value);
            if (find(root, value))
                printf("Element postoji\n");
            else
                printf("Element ne postoji\n");
            break;

        case 7:
            printf("Unesi broj: ");
            scanf("%d", &value);
            deleteNode(&root, value);
            break;
        }
    } while (choice != 0);

    freeTree(root);
    return 0;
}


/*
   Ako je stablo prazno -> novi čvor postaje root
   Ako je manji -> ide lijevo
   Ako je veći -> ide desno
*/

int insert(Node** root, int value) {
    if (*root == NULL) {
        *root = (Node*)malloc(sizeof(Node));
        (*root)->value = value;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return 1;
    }

    if (value < (*root)->value)
        return insert(&(*root)->left, value);
    else if (value > (*root)->value)
        return insert(&(*root)->right, value);

    return 0; 
}

int find(Node* root, int value) {
    if (root == NULL)
        return 0;

    if (value == root->value)
        return 1;

    if (value < root->value)
        return find(root->left, value);

    return find(root->right, value);
}

int inorder(Node* root) {
    if (root == NULL) return 0;

    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);

    return 1;
}

int preorder(Node* root) {
    if (root == NULL) return 0;

    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);

    return 1;
}

int postorder(Node* root) {
    if (root == NULL) return 0;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);

    return 1;
}

int levelOrder(Node* root) {
    if (root == NULL) return 0;

    Node* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];
        printf("%d ", current->value);

        if (current->left)
            queue[rear++] = current->left;

        if (current->right)
            queue[rear++] = current->right;
    }
    return 1;
}

int deleteNode(Node** root, int value) {
    if (*root == NULL)
        return 0;

    if (value < (*root)->value)
        return deleteNode(&(*root)->left, value);

    if (value > (*root)->value)
        return deleteNode(&(*root)->right, value);

    
    Node* temp;

    if ((*root)->left == NULL) {
        temp = (*root)->right;
        free(*root);
        *root = temp;
    }
    else if ((*root)->right == NULL) {
        temp = (*root)->left;
        free(*root);
        *root = temp;
    }
    else {
        temp = (*root)->right;
        while (temp->left)
            temp = temp->left;

        (*root)->value = temp->value;
        deleteNode(&(*root)->right, temp->value);
    }
    return 1;
}

int freeTree(Node* root) {
    if (root == NULL)
        return 0;

    freeTree(root->left);
    freeTree(root->right);
    free(root);

    return 1;
}
