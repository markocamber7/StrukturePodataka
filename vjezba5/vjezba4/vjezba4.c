#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 

// struktura za jedan ?lan polinoma
typedef struct {
    int coef; // koeficijent
    int exp;  // eksponent
} Pol;

// deklaracije funkcija
int readPolynomial(FILE *f, Pol p[]);
int addPolynomials(Pol p1[], int n1, Pol p2[], int n2, Pol result[]);
int multiplyPolynomials(Pol p1[], int n1, Pol p2[], int n2, Pol result[]);
int printPolynomial(Pol p[], int n);

int main() {
    FILE *f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        exit(1); // odmah prekid programa
    }

    Pol p1[20], p2[20], sum[40], product[40];

    // ?itanje dva polinoma iz datoteke
    int n1 = readPolynomial(f, p1);
    int n2 = readPolynomial(f, p2);
    fclose(f);

    // ra?unanje zbroja i umnoška
    int ns = addPolynomials(p1, n1, p2, n2, sum);
    int np = multiplyPolynomials(p1, n1, p2, n2, product);

    // ispis rezultata
    printf("Prvi polinom: ");
    printPolynomial(p1, n1);
    printf("Drugi polinom: ");
    printPolynomial(p2, n2);
    printf("Zbroj: ");
    printPolynomial(sum, ns);
    printf("Umnozak: ");
    printPolynomial(product, np);

    return 0;
}

// ?ita jedan polinom iz datoteke
int readPolynomial(FILE *f, Pol p[]) {
    int n = 0;
    while (fscanf(f, "%d %d", &p[n].coef, &p[n].exp) == 2) {
        n++;
        if (fgetc(f) == '\n') break; // kraj reda = novi polinom
    }
    return n;
}

// zbraja dva polinoma
int addPolynomials(Pol p1[], int n1, Pol p2[], int n2, Pol result[]) {
    int nr = 0;
    for (int i = 0; i < n1; i++) {
        result[nr++] = p1[i];
    }
    for (int i = 0; i < n2; i++) {
        int found = 0;
        for (int j = 0; j < nr; j++) {
            if (result[j].exp == p2[i].exp) {
                result[j].coef += p2[i].coef;
                found = 1;
            }
        }
        if (!found) result[nr++] = p2[i];
    }
    return nr;
}

// množi dva polinoma
int multiplyPolynomials(Pol p1[], int n1, Pol p2[], int n2, Pol result[]) {
    int nr = 0;
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int c = p1[i].coef * p2[j].coef; // novi koeficijent
            int e = p1[i].exp + p2[j].exp;   // novi eksponent
            int found = 0;
            for (int t = 0; t < nr; t++) {
                if (result[t].exp == e) {
                    result[t].coef += c;
                    found = 1;
                }
            }
            if (!found) {
                result[nr].coef = c;
                result[nr].exp = e;
                nr++;
            }
        }
    }
    return nr;
}

// ispisuje polinom
int printPolynomial(Pol p[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%+dx^%d ", p[i].coef, p[i].exp);
    }
    printf("\n");
    return 0;
}


