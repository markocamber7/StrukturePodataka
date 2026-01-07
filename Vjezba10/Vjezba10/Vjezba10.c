#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CityTree {
    char name[50];
    int pop;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct CityList {
    char name[50];
    int pop;
    struct CityList* next;
} CityList;

typedef struct CountryList {
    char name[50];
    CityTree* cities;
    struct CountryList* next;
} CountryList;

typedef struct CountryTree {
    char name[50];
    CityList* cities;
    struct CountryTree* left;
    struct CountryTree* right;
} CountryTree;


/* gradovi */
int insertCityTree(CityTree** root, char* name, int pop);
int insertCityList(CityList** head, char* name, int pop);

/* citanje */
int readCitiesTree(CityTree** root, char* file);
int readCitiesList(CityList** head, char* file);

/* drzave */
int insertCountryList(CountryList** head, char* name, char* file);
int insertCountryTree(CountryTree** root, char* name, char* file);

/* ispis i trazenje */
int printCityTree(CityTree* root, int minPop);
int printCityList(CityList* head, int minPop);
CountryList* findCountryList(CountryList* head, char* name);


int main() {
    FILE* f = fopen("drzave.txt", "r");
    char country[50], file[50];
    int minPop;

    CountryList* list = NULL;
    CountryTree* tree = NULL;

    if (!f) {
        printf("Greska pri otvaranju drzave.txt\n");
        return 1;
    }

    while (fscanf(f, "%s %s", country, file) == 2) {
        insertCountryList(&list, country, file);
        insertCountryTree(&tree, country, file);
    }
    fclose(f);

    printf("Unesi drzavu: ");
    scanf("%s", country);

    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    CountryList* c = findCountryList(list, country);
    if (c)
        printCityTree(c->cities, minPop);
    else
        printf("Drzava ne postoji.\n");

    return 0;
}



int insertCityTree(CityTree** root, char* name, int pop) {
    if (!(*root)) {
        *root = malloc(sizeof(CityTree));
        if (!(*root)) return -1;

        strcpy((*root)->name, name);
        (*root)->pop = pop;
        (*root)->left = (*root)->right = NULL;
        return 0;
    }

    if (pop > (*root)->pop)
        return insertCityTree(&(*root)->left, name, pop);
    else
        return insertCityTree(&(*root)->right, name, pop);
}

int insertCityList(CityList** head, char* name, int pop) {
    CityList* n = malloc(sizeof(CityList));
    if (!n) return -1;

    strcpy(n->name, name);
    n->pop = pop;
    n->next = *head;
    *head = n;
    return 0;
}



int readCitiesTree(CityTree** root, char* file) {
    FILE* f = fopen(file, "r");
    char name[50];
    int pop;

    if (!f) return -1;

    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        insertCityTree(root, name, pop);

    fclose(f);
    return 0;
}

int readCitiesList(CityList** head, char* file) {
    FILE* f = fopen(file, "r");
    char name[50];
    int pop;

    if (!f) return -1;

    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        insertCityList(head, name, pop);

    fclose(f);
    return 0;
}


int insertCountryList(CountryList** head, char* name, char* file) {
    CountryList* n = malloc(sizeof(CountryList));
    if (!n) return -1;

    strcpy(n->name, name);
    n->cities = NULL;
    n->next = *head;
    *head = n;

    return readCitiesTree(&n->cities, file);
}

int insertCountryTree(CountryTree** root, char* name, char* file) {
    if (!(*root)) {
        *root = malloc(sizeof(CountryTree));
        if (!(*root)) return -1;

        strcpy((*root)->name, name);
        (*root)->cities = NULL;
        (*root)->left = (*root)->right = NULL;
        return readCitiesList(&(*root)->cities, file);
    }

    if (strcmp(name, (*root)->name) < 0)
        return insertCountryTree(&(*root)->left, name, file);
    else
        return insertCountryTree(&(*root)->right, name, file);
}



int printCityTree(CityTree* root, int minPop) {
    if (!root) return 0;

    printCityTree(root->left, minPop);
    if (root->pop > minPop)
        printf("%s (%d)\n", root->name, root->pop);
    printCityTree(root->right, minPop);
    

    return 0;
}

int printCityList(CityList* head, int minPop) {
    while (head) {
        if (head->pop > minPop)
            printf("%s (%d)\n", head->name, head->pop);
        head = head->next;
    }
    return 0;
}


CountryList* findCountryList(CountryList* head, char* name) {
    while (head) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}
