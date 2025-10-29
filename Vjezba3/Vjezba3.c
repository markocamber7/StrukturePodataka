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

    new->next = head;
    return new;
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

    if (head == NULL) return new;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = new;
    return head;
}

// Ispis liste
Person* printList(Person* head) {
    Person* temp = head;
    if (head == NULL) {
        printf("Lista je prazna.\n");
        return head;
    }

    while (temp != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rodjenja: %d\n",
            temp->name, temp->username, temp->year);
        temp = temp->next;
    }
    return head;
}

// Pronađi osobu po prezimenu
Person* findPerson(Person* head, char prezime[]) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, prezime) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
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
        return head;
    }

    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
    printf("Osoba obrisana.\n");
    return head;
}

// A) Dodaj novi element iza određenog prezimena
Person* addAfter(Person* head, char prezime[]) {
    Person* current = findPerson(head, prezime);
    if (current == NULL) {
        printf("Osoba s prezimenom %s nije pronađena.\n", prezime);
        return head;
    }

    Person* new = (Person*)malloc(sizeof(Person));
    printf("Unesite ime nove osobe: ");
    scanf("%s", new->name);
    printf("Unesite prezime nove osobe: ");
    scanf("%s", new->username);
    printf("Unesite godinu rodjenja: ");
    scanf("%d", &new->year);

    new->next = current->next;
    current->next = new;

    printf("Osoba dodana iza %s.\n", prezime);
    return head;
}

// B) Dodaj novi element ispred određenog prezimena
Person* addBefore(Person* head, char prezime[]) {
    Person* new = (Person*)malloc(sizeof(Person));
    Person* current = head;
    Person* prev = NULL;

    printf("Unesite ime nove osobe: ");
    scanf("%s", new->name);
    printf("Unesite prezime nove osobe: ");
    scanf("%s", new->username);
    printf("Unesite godinu rodjenja: ");
    scanf("%d", &new->year);

    if (head != NULL && strcmp(head->username, prezime) == 0) {
        new->next = head;
        return new;
    }

    while (current != NULL && strcmp(current->username, prezime) != 0) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Osoba s prezimenom %s nije pronađena.\n", prezime);
        free(new);
        return head;
    }

    new->next = current;
    prev->next = new;

    printf("Osoba dodana ispred %s.\n", prezime);
    return head;
}

// C) Sortiraj listu po prezimenima
Person* sortList(Person* head) {
    if (head == NULL) return head;

    int swapped;
    Person* ptr1;
    Person* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->username, ptr1->next->username) > 0) {
                char tempName[50], tempUsername[50];
                int tempYear;

                strcpy(tempName, ptr1->name);
                strcpy(tempUsername, ptr1->username);
                tempYear = ptr1->year;

                strcpy(ptr1->name, ptr1->next->name);
                strcpy(ptr1->username, ptr1->next->username);
                ptr1->year = ptr1->next->year;

                strcpy(ptr1->next->name, tempName);
                strcpy(ptr1->next->username, tempUsername);
                ptr1->next->year = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    printf("Lista je sortirana po prezimenima.\n");
    return head;
}

// D) Upis u datoteku
Person* writeToFile(Person* head, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Greška: datoteka se ne može otvoriti!\n");
        return head;
    }

    Person* temp = head;
    while (temp != NULL) {
        fprintf(f, "%s %s %d\n", temp->name, temp->username, temp->year);
        temp = temp->next;
    }

    fclose(f);
    printf("Lista zapisana u datoteku %s.\n", filename);
    return head;
}

// E) Čitanje iz datoteke
Person* readFromFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Greška: datoteka %s se ne može otvoriti!\n", filename);
        return NULL;
    }

    Person* head = NULL;
    Person* tail = NULL;
    Person temp;

    while (fscanf(f, "%s %s %d", temp.name, temp.username, &temp.year) == 3) {
        Person* new = (Person*)malloc(sizeof(Person));
        strcpy(new->name, temp.name);
        strcpy(new->username, temp.username);
        new->year = temp.year;
        new->next = NULL;

        if (head == NULL)
            head = tail = new;
        else {
            tail->next = new;
            tail = new;
        }
    }

    fclose(f);
    printf("Lista učitana iz datoteke %s.\n", filename);
    return head;
}

int main() {
    Person* head = NULL;
    int izbor;
    char prezime[50];

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj na pocetak\n");
        printf("2. Dodaj na kraj\n");
        printf("3. Ispisi listu\n");
        printf("4. Pronadi po prezimenu\n");
        printf("5. Obrisi po prezimenu\n");
        printf("6. Dodaj iza prezimena\n");
        printf("7. Dodaj ispred prezimena\n");
        printf("8. Sortiraj listu\n");
        printf("9. Spremi u datoteku\n");
        printf("10. Ucitaj iz datoteke\n");
        printf("0. Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            head = addonTop(head);
            break;
        case 2:
            head = addonEnd(head);
            break;
        case 3:
            head = printList(head);
            break;
        case 4:
        {
            printf("Unesite prezime: ");
            scanf("%s", prezime);
            Person* p = findPerson(head, prezime);
            if (p != NULL)
                printf("Pronasli smo: %s %s %d\n", p->name, p->username, p->year);
            else
                printf("Osoba nije pronadena.\n");
            break;
        }
        case 5:
            printf("Unesite prezime: ");
            scanf("%s", prezime);
            head = deletePerson(head, prezime);
            break;
        case 6:
            printf("Unesite prezime iza kojeg zelite dodati novu osobu: ");
            scanf("%s", prezime);
            head = addAfter(head, prezime);
            break;
        case 7:
            printf("Unesite prezime ispred kojeg zelite dodati novu osobu: ");
            scanf("%s", prezime);
            head = addBefore(head, prezime);
            break;
        case 8:
            head = sortList(head);
            break;
        case 9:
            head = writeToFile(head, "osobe.txt");
            break;
        case 10:
            head = readFromFile("osobe.txt");
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor.\n");
        }
    } while (izbor != 0);

    while (head != NULL) {
        Person* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
