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

// Funkcija za brojanje studenata u datoteci
int countStudents(FILE* fp) {
    char temp[100];
    int count = 0;

    while (fgets(temp, sizeof(temp), fp) != NULL) {
        count++;
    }
    rewind(fp);  // Vraćamo pokazivač na početak datoteke
    return count;
}

// Funkcija za učitavanje studenata
Student* loadStudents(FILE* fp, int studentCount) {
    Student* students = (Student*)malloc(studentCount * sizeof(Student));
    if (students == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    for (int i = 0; i < studentCount; i++) {
        fscanf(fp, "%s %s %d", students[i].name, students[i].surname, &students[i].points);
    }

    return students;
}

// Funkcija za računanje relativnih bodova
float calculateRelativePoints(int points) {
    return ((float)points / MAX_POINTS) * 100;
}

// Funkcija za ispis tablice studenata
void printStudents(Student* students, int studentCount) {
    printf("IME\t\tPREZIME\t\tAPS BOD\t\tREL BOD (%%)\n");
    for (int i = 0; i < studentCount; i++) {
        float relative = calculateRelativePoints(students[i].points);
        printf("%s\t\t%s\t\t%d\t\t%.2f\n",
            students[i].name, students[i].surname,
            students[i].points, relative);
    }
}

int main() {
    FILE* fp = fopen("students.txt", "r");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    // 1. Brojanje studenata
    int studentCount = countStudents(fp);
    if (studentCount == 0) {
        printf("Datoteka je prazna.\n");
        fclose(fp);
        return 0;
    }

    // 2. Učitavanje studenata
    Student* students = loadStudents(fp, studentCount);
    fclose(fp);

    if (students == NULL) {
        return 1; // Greska pri alokaciji
    }

    // 3. Ispis
    printStudents(students, studentCount);

    // 4. Oslobađanje memorije
    free(students);

    return 0;
}
