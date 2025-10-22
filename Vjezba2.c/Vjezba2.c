#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED (-1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char name[50];
    char username[50];
    int year;
    struct Person* next;
} Person;

// Dodaj na početak
Person* addonTop(Person* head) {
    Person* new = (Person*)malloc(sizeof(Person));

    printf("Unesite ime: ");
    scanf("%s", new->name);

    printf("Unesite prezime: ");
    scanf("%s", new->username);

    printf("Unesite godinu rodjenja: ");
    scanf("%d", &new->year);

    new->next = head;  // novi pokazuje na stari početak
    return new;        // novi je sada head
}

// Dodaj na kraj
Person* addonEnd(Person* head) {
    Person* new = (Person*)malloc(sizeof(Person));
    Person* temp = head;

    printf("Unesite ime: ");
    scanf("%s", new->name);

    printf("Unesite prezime: ");
    scanf("%s", new->username);

    printf("Unesite godinu rodjenja: ");
    scanf("%d", &new->year);

    new->next = NULL;

    if (head == NULL) return new;  // ako je lista prazna

    while (temp->next != NULL) temp = temp->next;  // dođi do kraja
    temp->next = new;                             // spoji novi na kraj
    return head;
}

// Ispis liste
void printList(Person* head) {
    Person* temp = head;
    while (temp != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rodjenja: %d\n", temp->name, temp->username, temp->year);
        temp = temp->next;
    }
}

// Pronađi osobu po prezimenu
Person* findPerson(Person* head, char prezime[]) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, prezime) == 0)
            return temp; // pronađena osoba
        temp = temp->next;
    }
    return NULL; // nije pronađena
}

// Obrisi osobu po prezimenu
Person* deletePerson(Person* head, char prezime[]) {
    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->username, prezime) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba sa prezimenom %s nije pronađena.\n", prezime);
        return head; // ništa nije obrisano
    }

    if (prev == NULL) {
        // brisanje prvog elementa
        head = temp->next;
    }
    else {
        prev->next = temp->next;
    }

    free(temp); // oslobodi memoriju
    printf("Osoba obrisana.\n");
    return head;
}


int main() {
    Person* head = NULL;
    int izbor;
    char prezime[50];

    do {
        printf("\n1. Dodaj na pocetak\n2. Dodaj na kraj\n3. Ispisi listu\n4. Pronadi po prezimenu\n5. Obrisi po prezimenu\n0. Izlaz\nIzbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            head = addonTop(head);
            break;
        case 2:
            head = addonEnd(head);
            break;
        case 3:
            printList(head);
            break;
        case 4:
            printf("Unesite prezime: ");
            scanf("%s", prezime);
            Person* p = findPerson(head, prezime);
            if (p != NULL)
                printf("Pronasli smo: %s %s %d\n", p->name, p->username, p->year);
            else
                printf("Osoba nije pronadena.\n");
            break;
        case 5:
            printf("Unesite prezime: ");
            scanf("%s", prezime);
            head = deletePerson(head, prezime);
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor.\n");
        }
    } while (izbor != 0);

    // oslobodi memoriju prije izlaza
    while (head != NULL) {
        Person* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}

