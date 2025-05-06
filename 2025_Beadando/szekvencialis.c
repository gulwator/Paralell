#include <stdio.h>
#include <stdlib.h>
#include <windows.h>  // Nagy pontosságú időméréshez

#define MAX_CHAR 256

// A szövegben található karakterek gyakoriságának megszámolása
void count_characters(const char* text, int length, int* counts) {
    for (int i = 0; i < length; ++i) {
        unsigned char ch = text[i];  // Az ASCII karakterek 0-255 között vannak
        counts[ch]++;  // Növeli a megfelelő karakter előfordulását
    }
}

int main() {
    // Fájl megnyitása
    char* filename = "long-doc.txt";
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Nem sikerult megnyitni a fajlt!\n");
        return 1;
    }

    // Szöveg betöltése
    fseek(file, 0, SEEK_END);
    long txt_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* text = (char*)malloc(txt_length * sizeof(char));
    if (!text) {
        printf("Nem sikerult memoriafoglalas!\n");
        fclose(file);
        return 1;
    }

    fread(text, sizeof(char), txt_length, file);
    fclose(file);

    
    int counts[MAX_CHAR] = {0};
    
    // Időmérés kezdete
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    
    // Számlálás
    count_characters(text, txt_length, counts);
    
    // Időmérés vége
    QueryPerformanceCounter(&end);
    double elapsed_time = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    
    // Eredmények kiírása
    printf("\nKaraktergyakorisag:\n");
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (counts[i] > 0) {
            if (i >= 32 && i <= 126)
            printf("'%c' (%d): %d\n", i, i, counts[i]);
            else
            printf("[0x%02x] (%d): %d\n", i, i, counts[i]);
        }
    }
    printf("Szoveg hossza: %ld karakter\n", txt_length);
    printf("Szekvencialis futasi ido: %.6f masodperc\n", elapsed_time);
    
    free(text);
    return 0;
}
