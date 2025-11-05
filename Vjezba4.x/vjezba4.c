#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int push(struct Node** top, int value);
int pop(struct Node** top);
int isEmpty(struct Node* top);

struct Node {
    int data;
    struct Node* next;
};


int main() {
    FILE* f = fopen("postfiks.txt", "r");
    if (!f) {
        printf("Greska: ne mogu otvoriti datoteku!\n");
        exit(1);
    }

    struct Node* stog = NULL;
    char token[50];

    // ?itanje svakog tokena iz datoteke
    while (fscanf(f, "%s", token) == 1) {

        // Ako je broj (ili negativan broj)
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            int broj = atoi(token);  // pretvara string u int
            push(&stog, broj);
        }
        else {
            // Ako je operator
            int b = pop(&stog);
            int a = pop(&stog);
            int rez = 0;

            if (strcmp(token, "+") == 0)
                rez = a + b;
            else if (strcmp(token, "-") == 0)
                rez = a - b;
            else if (strcmp(token, "*") == 0)
                rez = a * b;
            else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    exit(2);
                }
                rez = a / b;
            }
            else {
                printf("Nepoznat operator: %s\n", token);
                exit(3);
            }

            push(&stog, rez);
        }
    }

    fclose(f);

    // Na kraju na stogu mora ostati samo rezultat
    if (isEmpty(stog)) {
        printf("Greska: prazan stog!\n");
        exit(4);
    }

    int rezultat = pop(&stog);

    if (!isEmpty(stog)) {
        printf("Greska: izraz nije ispravan!\n");
        exit(5);
    }

    printf("Rezultat: %d\n", rezultat);
    return 0;
}


// Dodavanje broja na stog
int push(struct Node** top, int value) {
    struct Node* novi = (struct Node*)malloc(sizeof(struct Node));
    if (!novi) {
        printf("Greska pri alokaciji memorije!\n");
        exit(6);
    }
    novi->data = value;
    novi->next = *top;
    *top = novi;
    return 0; // sve u redu
}

// Skidanje broja sa stoga
int pop(struct Node** top) {
    if (*top == NULL) {
        printf("Greska: stog je prazan!\n");
        exit(7);
    }
    struct Node* temp = *top;
    int value = temp->data;
    *top = temp->next;
    free(temp);
    return value;
}

// Provjera je li stog prazan
int isEmpty(struct Node* top) {
    return top == NULL;
}
