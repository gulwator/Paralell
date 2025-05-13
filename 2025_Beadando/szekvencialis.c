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
    char* filename = "midmidbigfile.txt";
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Nem sikerult megnyitni a fajlt!\n");
        return 1;
    }
    // CSV fájl megnyitása írásra
    FILE* csv_file = fopen("timing_results.csv", "w");
    if (!csv_file) {
        printf("Nem sikerult megnyitni a CSV fajlt!\n");
        return 1;
    }

    // Szöveg betöltése
    fseek(file, 0, SEEK_END);
    long txt_length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Szöveg tördelése
    int sizes[]={250000,500000,750000,1000000,1250000,1500000,1750000,2000000,
             2250000,2500000,2750000,3000000,3250000,3500000,3750000,4000000,
             4250000,4500000,4750000,5000000,5250000,5500000,5750000,6000000,6250000};


    char* text = (char*)malloc(txt_length * sizeof(char));
    if (!text) {
        printf("Nem sikerult memoriafoglalas!\n");
        fclose(file);
        return 1;
    }

    fread(text, sizeof(char), txt_length, file);
    fclose(file);

   for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        if (sizes[i] > txt_length) break;

    int counts[MAX_CHAR] = {0};
    
    // Időmérés kezdete
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    
    // Számlálás
    count_characters(text, sizes[i], counts);
    
    // Időmérés vége
    QueryPerformanceCounter(&end);
    double elapsed_time = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    
    // Eredmények kiírása
    // printf("\nKaraktergyakorisag:\n");
    // for (int i = 0; i < MAX_CHAR; ++i) {
    //     if (counts[i] > 0) {
    //         if (i >= 32 && i <= 126)
    //         printf("'%c' (%d): %d\n", i, i, counts[i]);
    //         else
    //         printf("[0x%02x] (%d): %d\n", i, i, counts[i]);
    //     }
    // }
    printf("Szoveg hossza: %ld karakter\n", sizes[i]);
    printf("Szekvencialis futasi ido: %.6f masodperc\n", elapsed_time);
     // Eredmény hozzáadása a CSV fájlhoz
        fprintf(csv_file, "%.6f%s", elapsed_time, (i == (sizeof(sizes) / sizeof(sizes[0])) - 1) ? "\n" : ",");
   }
   fclose(csv_file);
    free(text);
    return 0;
}
