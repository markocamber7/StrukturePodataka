#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Item {
    char name[50];     
    int qty;            
    float price;        
    struct Item* next;  
} Item;

typedef struct Bill {
    char date[11];      
    Item* items;        
    struct Bill* next;  
} Bill;



int addBillSorted(Bill** head, const char* date) {
    Bill* b = (Bill*)malloc(sizeof(Bill));
    if (!b) return -1;

    strcpy(b->date, date);
    b->items = NULL;
    b->next = NULL;

    // ako je lista prazna ili novi račun ide prije prvog
    if (*head == NULL || strcmp(date, (*head)->date) < 0) {
        b->next = *head;
        *head = b;
        return 0;
    }

    // inače ga tražimo pravo mjesto u listi
    Bill* t = *head;
    while (t->next != NULL && strcmp(t->next->date, date) < 0)
        t = t->next;

    b->next = t->next;
    t->next = b;
    return 0;
}

int addItemSorted(Item** head, const char* name, int qty, float price) {
    Item* i = (Item*)malloc(sizeof(Item));
    if (!i) return -1;

    strcpy(i->name, name);
    i->qty = qty;
    i->price = price;
    i->next = NULL;

    // ako je lista prazna ili ide prije prvog
    if (*head == NULL || strcmp(name, (*head)->name) < 0) {
        i->next = *head;
        *head = i;
        return 0;
    }

    // tražimo pravo mjesto u sortiranoj listi
    Item* t = *head;
    while (t->next != NULL && strcmp(t->next->name, name) < 0)
        t = t->next;

    i->next = t->next;
    t->next = i;
    return 0;
}

int loadBill(Bill** head, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return -1;

    char date[20];

    // učitavanje datuma
    if (fgets(date, sizeof(date), f) == NULL) { fclose(f); return -1; }
    date[strcspn(date, "\r\n")] = 0;   // briše \n

    // ubaci račun sortiran po datumu
    if (addBillSorted(head, date) != 0) { fclose(f); return -1; }

    // pronalazi zadnji umetnuti račun
    Bill* b = *head;
    while (b->next != NULL) b = b->next;

    // čitanje artikala iz datoteke
    char line[200];
    char name[50];
    int qty;
    float price;

    while (fgets(line, sizeof(line), f)) {
        // format: naziv,kolicina,cijena
        if (sscanf(line, "%49[^,],%d,%f", name, &qty, &price) == 3) {
            addItemSorted(&b->items, name, qty, price);
        }
    }

    fclose(f);
    return 0;
}

int loadAll(Bill** head) {
    FILE* f = fopen("racuni.txt", "r");
    if (!f) return -1;

    char filename[200];

    while (fgets(filename, sizeof(filename), f)) {
        filename[strcspn(filename, "\r\n")] = 0;
        if (strlen(filename) == 0) continue;

        loadBill(head, filename);  // učitaj pojedini račun
    }

    fclose(f);
    return 0;
}

int printBill(Bill* head) {
    char date[11];
    printf("Unesi datum (YYYY-MM-DD): ");
    scanf("%10s", date);

    // traži račun s tim datumom
    while (head) {
        if (strcmp(head->date, date) == 0) {
            printf("\nRacun %s:\n", head->date);

            Item* i = head->items;
            while (i) {
                printf("%s  %d kom  %.2f EUR\n", i->name, i->qty, i->price);
                i = i->next;
            }
            return 0;
        }
        head = head->next;
    }

    printf("Nema tog racuna.\n");
    return 0;
}

int searchRange(Bill* head, const char* name, const char* from, const char* to) {
    int totalQty = 0;
    float totalPrice = 0.0f;

    while (head) {
        // uzima samo račune koji su unutar zadanog raspona
        if (strcmp(head->date, from) >= 0 && strcmp(head->date, to) <= 0) {

            // prolazi sve artikle u računu
            Item* i = head->items;
            while (i) {
                if (strcmp(i->name, name) == 0) {
                    totalQty += i->qty;
                    totalPrice += i->qty * i->price;
                }
                i = i->next;
            }
        }
        head = head->next;
    }

    printf("\nARTIKL: %s\n", name);
    printf("Razdoblje: %s  ->  %s\n", from, to);
    printf("Ukupno kupljeno: %d kom\n", totalQty);
    printf("Ukupno potroseno: %.2f EUR\n", totalPrice);

    return 0;
}

int summary(Bill* head) {
    struct Stat { char name[50]; int qty; float total; };
    struct Stat stats[300];   // spremnik za sve artikle
    int n = 0;
    float sveukupno = 0.0f;

    while (head) {
        Item* i = head->items;

        while (i) {
            int found = 0;

            // tražimo je li artikl već u popisu
            for (int j = 0; j < n; j++) {
                if (strcmp(stats[j].name, i->name) == 0) {
                    stats[j].qty += i->qty;
                    stats[j].total += i->qty * i->price;
                    found = 1;
                    break;
                }
            }

            // ako nije, dodaj novi unos
            if (!found && n < 300) {
                strcpy(stats[n].name, i->name);
                stats[n].qty = i->qty;
                stats[n].total = i->qty * i->price;
                n++;
            }

            i = i->next;
        }

        head = head->next;
    }

    printf("\n--- OBRACUN SVIH RACUNA ---\n");
    for (int j = 0; j < n; j++) {
        printf("%s  %d kom  %.2f EUR\n", stats[j].name, stats[j].qty, stats[j].total);
        sveukupno += stats[j].total;
    }

    printf("-------------------------------\n");
    printf("UKUPNO POTROSENO: %.2f EUR\n", sveukupno);
    return 0;
}


int freeAll(Bill* head) {
    Bill* nextB;
    Item* nextI;

    while (head) {
        Item* i = head->items;

        while (i) {
            nextI = i->next;
            free(i);
            i = nextI;
        }

        nextB = head->next;
        free(head);
        head = nextB;
    }
    return 0;
}


int main() {
    Bill* bills = NULL;

    // učitamo sve račune iz datoteka
    if (loadAll(&bills) != 0) {
        printf("Greska pri ucitavanju.\n");
        return 1;
    }

    int ch;
    char name[50];
    char from[20], to[20];

    do {
        printf("\n1 - Ispis racuna\n");
        printf("2 - Pretraga artikla\n");
        printf("3 - Obracun svih racuna\n");
        printf("0 - Kraj\n");
        printf("Izbor: ");
        scanf("%d", &ch);

        if (ch == 1)
            printBill(bills);

        else if (ch == 2) {
            printf("Unesi artikl: ");
            scanf("%49s", name);

            printf("Od datuma (YYYY-MM-DD): ");
            scanf("%19s", from);

            printf("Do datuma (YYYY-MM-DD): ");
            scanf("%19s", to);

            searchRange(bills, name, from, to);
        }

        else if (ch == 3)
            summary(bills);

    } while (ch != 0);

    freeAll(bills);
    return 0;
}
