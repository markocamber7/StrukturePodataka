#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 50  // Maksimalni broj bodova na ispitu

// Struktura za jednog studenta
typedef struct {
    char name[50];
    char surname[50];
    int points;
} Student;

int main() {
    
    int studentCount = 0;
    char temp[100];   // Pomoćni buffer za brojanje redaka
    Student* students;

    // Otvaranje datoteke 
    FILE* fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    // Brojimo koliko ima redaka (tj. koliko ima studenata)
    while (fgets(temp, sizeof(temp), fp) != NULL) {
        studentCount++;
    }

    // Ako je datoteka prazna - prekid programa
    if (studentCount == 0) {
        printf("Datoteka je prazna.\n");
        fclose(fp);
        return 0;
    }

    // Vraćamo pokazivač na početak datoteke (da možemo ponovo čitati)
    rewind(fp);

    // Dinamička alokacija memorije za sve studente
    students = (Student*)malloc(studentCount * sizeof(Student));
    if (students == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        fclose(fp);
        return 1;
    }

    // Učitavanje podataka iz datoteke
    for (int i = 0; i < studentCount; i++) {
        fscanf(fp, "%s %s %d", students[i].name, students[i].surname, &students[i].points);
    }

    // Zatvaramo datoteku 
    fclose(fp);

    // Ispis podataka o studentima
    printf("IME\t\tPREZIME\t\tAPS BOD\t\tREL BOD (%%)\n");
    for (int i = 0; i < studentCount; i++) {
        // Izračun relativnih bodova u postocima
        float relative = ((float)students[i].points / MAX_POINTS) * 100;
        printf("%s\t\t%s\t\t%d\t\t%.2f\n",
            students[i].name, students[i].surname,
            students[i].points, relative);
    }

    // Oslobađanje memorije jer nam više ne treba
    free(students);

    return 0;
}
