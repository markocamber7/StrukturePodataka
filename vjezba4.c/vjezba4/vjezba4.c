#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int coef;
    int exp;
    struct Node *next;
} Node;

// deklaracije funkcija
Node *readPolynomial(FILE *f);
Node *addPolynomials(Node *p1, Node *p2);
Node *multiplyPolynomials(Node *p1, Node *p2);
void printPolynomial(Node *p);
void insertSorted(Node **head, int coef, int exp);
void freeList(Node *p);

int main() {
    FILE *f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        exit(1);
    }

    Node *p1 = readPolynomial(f);
    Node *p2 = readPolynomial(f);
    fclose(f);

    Node *sum = addPolynomials(p1, p2);
    Node *product = multiplyPolynomials(p1, p2);

    printf("Prvi polinom: ");
    printPolynomial(p1);
    printf("Drugi polinom: ");
    printPolynomial(p2);
    printf("Zbroj: ");
    printPolynomial(sum);
    printf("Umnozak: ");
    printPolynomial(product);

    // oslobađanje memorije
    freeList(p1);
    freeList(p2);
    freeList(sum);
    freeList(product);

    return 0;
}

// funkcija za umetanje člana u sortiranu listu (po eksponentu, opadajuće)
void insertSorted(Node **head, int coef, int exp) {
    if (coef == 0) return;
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = NULL;

    // prazna lista ili najveći eksponent
    if (*head == NULL || (*head)->exp < exp) {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    Node *curr = *head, *prev = NULL;
    while (curr && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }

    if (curr && curr->exp == exp) {
        curr->coef += coef;
        free(newNode);
        if (curr->coef == 0) { // izbaci ako je 0
            if (prev) prev->next = curr->next;
            else *head = curr->next;
            free(curr);
        }
    } else {
        newNode->next = curr;
        if (prev) prev->next = newNode;
        else *head = newNode;
    }
}

// čitanje polinoma iz datoteke
Node *readPolynomial(FILE *f) {
    Node *head = NULL;
    int c, e;
    char ch;
    while (fscanf(f, "%d %d", &c, &e) == 2) {
        insertSorted(&head, c, e);
        ch = fgetc(f);
        if (ch == '\n' || ch == EOF) break;
        else ungetc(ch, f);
    }
    return head;
}

// zbrajanje polinoma
Node *addPolynomials(Node *p1, Node *p2) {
    Node *result = NULL;
    while (p1) {
        insertSorted(&result, p1->coef, p1->exp);
        p1 = p1->next;
    }
    while (p2) {
        insertSorted(&result, p2->coef, p2->exp);
        p2 = p2->next;
    }
    return result;
}

// množenje polinoma
Node *multiplyPolynomials(Node *p1, Node *p2) {
    Node *result = NULL;
    for (Node *a = p1; a != NULL; a = a->next) {
        for (Node *b = p2; b != NULL; b = b->next) {
            insertSorted(&result, a->coef * b->coef, a->exp + b->exp);
        }
    }
    return result;
}

// ispis polinoma
void printPolynomial(Node *p) {
    if (!p) {
        printf("0\n");
        return;
    }
    while (p) {
        printf("%+dx^%d ", p->coef, p->exp);
        p = p->next;
    }
    printf("\n");
}

// oslobađa listu iz memorije
void freeList(Node *p) {
    while (p) {
        Node *tmp = p;
        p = p->next;
        free(tmp);
    }
}
